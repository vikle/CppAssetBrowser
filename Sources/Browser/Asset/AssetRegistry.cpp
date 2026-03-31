#include <algorithm>
#include "AssetRegistry.h"

void AssetRegistry::AddOrUpdate(AssetInfo info)
{
    std::lock_guard lock(m_mutex);
    const auto it = m_pathIndex.find(info.path);

    if (it != m_pathIndex.end())
    {
        m_assets[it->second] = std::move(info);
    }
    else
    {
        m_pathIndex[info.path] = (int)m_assets.size();
        m_assets.push_back(std::move(info));
    }
}

std::vector<const AssetInfo*> AssetRegistry::Filter(
    const std::string_view nameFilter,
    const EAssetType typeFilter) const
{
    std::lock_guard lock(m_mutex);
    std::vector<const AssetInfo*> result;

    result.reserve(m_assets.size());

    for (const auto& a : m_assets)
    {
        if (typeFilter != EAssetType::Unknown && a.type != typeFilter)
            continue;

        if (!nameFilter.empty()
            && a.name.find(nameFilter) == std::string::npos)
            continue;

        result.push_back(&a);
    }
    return result;
}

int AssetRegistry::Count() const
{
    std::lock_guard lock(m_mutex);
    return static_cast<int>(m_assets.size());
}

AssetRegistry::Stats AssetRegistry::GetStats() const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    Stats s;
    for (const auto& a : m_assets)
    {

        s.totalBytes += a.sizeBytes;

        switch (a.type)
        {
            case EAssetType::Mesh: s.mesh++; break;
            case EAssetType::Texture: s.texture++; break;
            case EAssetType::Shader: s.shader++; break;
            case EAssetType::Audio: s.audio++; break;
            default: break;
        }
    }
    return s;
}
