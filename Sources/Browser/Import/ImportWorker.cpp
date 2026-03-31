#include <vector>
#include "ImportWorker.h"

ImportWorker::~ImportWorker()
{
    m_running = false;

    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

void ImportWorker::ScanAndImport(const fs::path& rootDir)
{
    if (m_running) return;

    if (m_thread.joinable())
    {
        m_thread.join();
    }

    std::vector<fs::path> files;

    if (fs::exists(rootDir))
    {
        for (const auto& entry : fs::recursive_directory_iterator(
                rootDir,fs::directory_options::skip_permission_denied))
        {
            if (entry.is_regular_file())
            {
                files.push_back(entry.path());
            }
        }
    }

    if (files.empty()) return;

    m_running = true;
    m_progress = 0.0f;

    m_thread = std::thread([this, files = std::move(files)]()
    {
        const int total = static_cast<int>(files.size());

        for (int i = 0; i < total; i++)
        {
            if (!m_running) break;

            const auto& p = files[i];
            AssetInfo info;
            info.path = p.string();
            info.name = p.filename().string();
            info.type = AssetInfo::TypeFromExtension(p.extension().string());
            info.status = EAssetStatus::Ready;

            try { info.sizeBytes = fs::file_size(p); }
            catch (...) { info.sizeBytes = 0; }

            m_registry.AddOrUpdate(std::move(info));

            m_progress = ((i + 1.0f) / static_cast<float>(total));
        }
        m_running = false;
    });
}