#include "StdInc.h"
#include "StreamingChannel.h"
#include "ResourceStreamer.h"

#include <fileapi.h>

#include <ranges>

namespace rng = std::ranges;

using namespace ResourceStreamer;

StreamingChannel::~StreamingChannel() {
    for (const HANDLE f : m_archieves) {
        CloseHandle(f);
    }
    m_thrd.join();
}

void StreamingChannel::DoWork() noexcept {
    while (s_running)
    {
        CStreamingInfo* item{};

        // Wait for a request to come in..
        {
            std::unique_lock lock{ s_mtx };
            s_cv.wait(lock,
                [this] { return !s_running || !s_requests.empty(); }
            );
            if (!s_running)
                break;

            if (s_requests.empty())
                continue; // Since `notify_all` was used another channel might have processed this request already

            item = s_requests.back();
            s_requests.pop_back();
        }

        // Automatically increment / decrement `s_numBeingRead`
        struct ReadCountGuard {
            ReadCountGuard(std::mutex& mtx) :
                m_mtx(mtx)
            {
                std::scoped_lock lck{ m_mtx };
                s_numBeingRead++;
            }

            ~ReadCountGuard() {
                std::scoped_lock lck{ m_mtx };
                s_numBeingRead--;
            }
        private:
            std::mutex& m_mtx;
        } const rcguard{m_mtx};

        // Find position in buffer for element
        Sector* bufferPos{nullptr};
        {
            std::lock_guard gurard{ m_mtx };
            if (!m_readyInfos.empty()) {
                for (auto it = m_readyInfos.begin(); it != m_readyInfos.end(); it++) {
                    const auto next = std::next(it);
                    const auto nextPos = (next != m_readyInfos.end()) ? next->buffPos : m_bufferEnd;
                    const auto thisEnd = it->buffPos + it->buffSz;

                    if (static_cast<uint32>(thisEnd - nextPos) >= item->GetCdSize()) {
                        bufferPos = thisEnd;
                        break;
                    }
                }
            } else {
                bufferPos = m_bufferBegin;
            }
        }

        if (!bufferPos)
        {
            std::lock_guard lock{ s_mtx };
            s_requests.push_back(item);
            continue; // Couldn't find free space in buffer, let another channel process it.
        }

        if (item->GetLoadState() != eStreamingLoadState::REQUESTED)
            continue; // Request revoked, so just ignore

        item->SetLoadState(eStreamingLoadState::READING);
        
        // Read item from disk
        {
            const HANDLE fh = GetHandleOfArchive(item->GetImgId());
            SetFilePointer(fh, static_cast<LONG>(item->GetCdPosn() * sizeof(Sector)), nullptr, FILE_BEGIN);
            if (!ReadFile(fh, bufferPos, item->GetCdSize() * sizeof(Sector), nullptr, nullptr)) {
                assert(0 && "Read failed");
            }
        }

        if (item->GetLoadState() != eStreamingLoadState::READING)
            continue; // Request revoked, so just ignore

        //m_bufferPtr += static_cast<ptrdiff_t>(item->GetCdSize());

        item->SetLoadState(eStreamingLoadState::READING_FINISHED);

        PushReadItem(item, bufferPos);
    }
}

void StreamingChannel::AddDirectory(const std::filesystem::path& path) {
    SetLastError(0);
    m_archieves.emplace_back(CreateFileW(
        path.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    ));
    assert(!GetLastError());
}

auto StreamingChannel::GetReadItem() noexcept -> std::optional<std::pair<ReadResourceInfo, size_t>> {
    std::lock_guard guard{ m_mtx };
    if (!m_readyInfos.empty()) {
        return { { m_readyInfos.back(), m_readyInfos.size() - 1 } };
    }
    return std::nullopt;
}

void StreamingChannel::PopReadItem(size_t resIdx) noexcept {
    std::lock_guard guard{ m_mtx };
    m_readyInfos.erase(m_readyInfos.begin() + resIdx);
}

void StreamingChannel::SetBuffer(Sector* begin, Sector* end) noexcept {
    std::lock_guard guard{ m_mtx };
    m_bufferBegin = begin;
    m_bufferEnd = end;
}

HANDLE StreamingChannel::GetHandleOfArchive(uint32 id) noexcept {
    std::lock_guard guard{ m_mtx };
    return m_archieves[id];
}

void StreamingChannel::PushReadItem(CStreamingInfo* info, Sector* pos) {
    ReadResourceInfo rri{ pos, info->GetCdSize(), info };
    const auto it = rng::lower_bound(m_readyInfos | rng::views::reverse, rri,
        [](auto a, auto b) {
            if (a.info->IsPriorityRequest() == b.info->IsPriorityRequest())
                return false;  // Same priority
            return !a.info->IsPriorityRequest();
        }
    );
    m_readyInfos.emplace(it.base(), rri);
}


