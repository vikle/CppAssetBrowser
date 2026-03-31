#pragma once

#include <vector>
#include <unordered_map>
#include <mutex>
#include <string_view>
#include "AssetInfo.h"

class AssetRegistry
{
    std::vector<AssetInfo> m_assets;
    std::unordered_map<std::string, int> m_pathIndex;
    mutable std::mutex m_mutex;

public:
    void AddOrUpdate(AssetInfo info);

    std::vector<const AssetInfo*> Filter(
        std::string_view nameFilter,
        EAssetType typeFilter) const;

    int Count() const;

    struct Stats
    {
        int mesh = 0, texture = 0, shader = 0, audio = 0;
        uint64_t totalBytes = 0;
    };

    Stats GetStats() const;
};
