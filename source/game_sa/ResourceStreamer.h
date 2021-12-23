#pragma once

#include <Base.h>

#include <condition_variable>
#include <mutex>
#include <vector>

namespace ResourceStreamer {
    inline std::mutex                            s_mtx{};
    inline std::condition_variable               s_cv{};
    inline std::vector<class CStreamingInfo*>    s_requests{};     // Resources waiting to be read
    inline uint32                                s_numBeingRead{}; // Resources currently being read
    inline bool                                  s_running{ true };
    inline size_t                                s_bufferSz{}; 

    constexpr     uint32                                STREAMING_SECTOR_SIZE{ 2048 };

    
    using Sector = uint8[STREAMING_SECTOR_SIZE];

    // Info about a read resource
    class ReadResourceInfo {
    public:
        Sector* buffPos{}; // Pointer to file data
        uint32_t buffSz;
        CStreamingInfo* info;
    };


    // Should be called before any operations are done
    auto Init(uint32 numChannels)->void;

    // Adds a directory (aka img file)
    // Returns it's ID
    auto AddDirectory(const char* path)->uint32;

    // Request the reading of a resource
    auto RequestResource(CStreamingInfo& info)->void;

    struct ReadResourceStackCtx {
        size_t chIdx, resIdx;
    };

    // Pop info of a loaded resource.
    // Returns nullopt if there's none.
    auto GetReadResource(ReadResourceStackCtx& ctx /*out*/)->std::optional<ReadResourceInfo>;

    //
    void PopReadResoure(ReadResourceStackCtx ctx /*in*/);


    // Blocks caller until all models are finished loading
    void Sync();

    bool HasFinished();
};
