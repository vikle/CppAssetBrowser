#pragma once

#include <string>
#include "Asset/AssetRegistry.h"
#include "Import/ImportWorker.h"

class AssetBrowserWindow
{
public:
    explicit AssetBrowserWindow(AssetRegistry& reg);

    void Render();

private:
    void RenderToolbar();
    void RenderStats() const;
    void RenderAssetList();
    void RenderStatusBar() const;

private:
    AssetRegistry& m_registry;
    ImportWorker m_worker;

    char m_filterBuf[256] = {};
    int m_typeFilter = 0;
    int m_sortMode = 0;
    std::string m_selectedPath;
    std::string m_scanDir = "./assets";
    char m_scanDirBuf[512]{};
};
