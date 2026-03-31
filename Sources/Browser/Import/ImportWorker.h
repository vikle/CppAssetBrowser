#pragma once

#include <thread>
#include <atomic>
#include <filesystem>
#include "Asset/AssetRegistry.h"

namespace fs = std::filesystem;

class ImportWorker
{
    AssetRegistry& m_registry;
    std::thread m_thread;
    std::atomic<bool>  m_running{false};
    std::atomic<float> m_progress{0.f};

public:
    explicit ImportWorker(AssetRegistry& registry)
        : m_registry(registry) {}

    ImportWorker(const ImportWorker&) = delete;
    ImportWorker& operator=(const ImportWorker&) = delete;

    ~ImportWorker();

    void ScanAndImport(const fs::path& rootDir);

    float GetProgress() const { return m_progress.load(); }
    bool IsRunning() const { return m_running.load(); }
    void Cancel() { m_running = false; }
};
