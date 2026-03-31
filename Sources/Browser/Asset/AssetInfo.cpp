#include "AssetInfo.h"

EAssetType AssetInfo::TypeFromExtension(const std::string& ext)
{
    if (ext == ".fbx" || ext == ".obj" || ext == ".gltf") return EAssetType::Mesh;
    if (ext == ".png" || ext == ".jpg" || ext == ".dds")  return EAssetType::Texture;
    if (ext == ".hlsl" || ext == ".glsl" || ext == ".vert") return EAssetType::Shader;
    if (ext == ".wav" || ext == ".ogg" || ext == ".mp3")  return EAssetType::Audio;
    return EAssetType::Unknown;
}

const char* AssetInfo::TypeName(const EAssetType type)
{
    switch (type)
    {
        case EAssetType::Mesh: return "Mesh";
        case EAssetType::Texture: return "Texture";
        case EAssetType::Shader: return "Shader";
        case EAssetType::Audio: return "Audio";
        default: return "Unknown";
    }
}

const char* AssetInfo::StatusName(const EAssetStatus status)
{
    switch (status)
    {
        case EAssetStatus::Importing: return "Importing";
        case EAssetStatus::Ready: return "Ready";
        case EAssetStatus::Failed: return "Failed";
        default: return "Unknown";
    }
}
