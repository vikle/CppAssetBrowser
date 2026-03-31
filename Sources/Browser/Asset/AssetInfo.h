#pragma once
#include <string>

enum class EAssetType { Unknown, Mesh, Texture, Shader, Audio };
enum class EAssetStatus { Unknown, Importing, Ready, Failed };

struct AssetInfo
{
    static EAssetType TypeFromExtension(const std::string& ext);
    static const char* TypeName(EAssetType type);
    static const char* StatusName(EAssetStatus status);

    std::string path;
    std::string name;
    EAssetType type = EAssetType::Unknown;
    EAssetStatus status = EAssetStatus::Unknown;
    uint64_t sizeBytes = 0;
    bool isDirty = false;
};
