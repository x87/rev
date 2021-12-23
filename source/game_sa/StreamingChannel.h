#pragma once
#include <Base.h>
#include <constants.h>

#include <atomic>
#include <thread>
#include <filesystem>

#include <windef.h>

#include "ResourceStreamer.h"

class StreamingChannel {
public:
    using Sector = ResourceStreamer::Sector;
    using ReadResourceInfo = ResourceStreamer::ReadResourceInfo;

    enum class Status
    {
        IDLE,
        READING,
        STARTED,
        ERR
    };

public:
    StreamingChannel() :
        m_thrd([this] { DoWork(); })
    {
    }

    StreamingChannel(StreamingChannel&& other) noexcept :
        m_mtx{},
        m_readyInfos{ std::move(other.m_readyInfos) },
        m_bufferBegin{ other.m_bufferBegin },
        m_bufferEnd{ other.m_bufferEnd },
        m_archieves{ std::move(other.m_archieves) },
        m_thrd{ std::move(other.m_thrd) }
    {

    }

    ~StreamingChannel();

    //StreamingChannel& operator=(StreamingChannel&&) = default;

    // Main worker function, runs in a separate thread
    void DoWork() noexcept;

    // Add a new directory (aka img file)
    void AddDirectory(const std::filesystem::path& path);

    // 
    auto GetReadItem() noexcept -> std::optional<std::pair<ReadResourceInfo, size_t>>;

    void PopReadItem(size_t resIdx) noexcept;

    //
    void SetBuffer(Sector* p, Sector* end) noexcept;

    //
    HANDLE GetHandleOfArchive(uint32 id) noexcept;

private:
    // Buffer pointer should be set to point to where this resource's data begins.
    void PushReadItem(class CStreamingInfo* info, Sector* pos);
private:
    Status m_status{ Status::IDLE };

    // Mutex guarding all member field accesses
    std::mutex m_mtx;

    // Resources read, waiting to be processed by the main thread.
    // Sorted by priority in reverse order, eg.: last item is the highest priority
    std::vector<ReadResourceInfo> m_readyInfos;

    Sector* m_bufferBegin{};                // Buffer for models. If full, loading will be halted until it there's enough free memory for a model to be loaded.
    //std::atomic<Sector*> m_bufferPtr{ m_bufferBegin };   // Current position in buffer
    Sector* m_bufferEnd{};                  // End of buffer

    std::vector<bool> m_sectorsState;

    std::vector<HANDLE> m_archieves{};

    std::thread	m_thrd{};		    // Thread `DoWork` runs on
};

