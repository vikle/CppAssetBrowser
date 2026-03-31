#include <imgui.h>
#include <algorithm>
#include "AssetBrowserWindow.h"

AssetBrowserWindow::AssetBrowserWindow(AssetRegistry& reg)
    : m_registry(reg)
    , m_worker(reg)
{
    strncpy(m_scanDirBuf, m_scanDir.c_str(), sizeof(m_scanDirBuf) - 1);
    m_scanDirBuf[sizeof(m_scanDirBuf) - 1] = '\0';
}

void AssetBrowserWindow::Render()
{
    ImGui::Begin("Asset Browser");

    RenderToolbar();
    ImGui::Separator();
    RenderStats();
    ImGui::Separator();
    RenderAssetList();
    RenderStatusBar();

    ImGui::End();
}

void AssetBrowserWindow::RenderToolbar()
{
    ImGui::SetNextItemWidth(200);
    ImGui::InputText("##scandir", m_scanDirBuf, sizeof(m_scanDirBuf));
    ImGui::SameLine();

    if (ImGui::Button("Scan"))
    {
        m_scanDir = m_scanDirBuf;
        m_registry.~AssetRegistry();
        new (&m_registry) AssetRegistry();
        m_worker.ScanAndImport(m_scanDir);
    }

    ImGui::SameLine();

    if (m_worker.IsRunning())
    {
        ImGui::ProgressBar(m_worker.GetProgress(), ImVec2(120, 0));
        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
        {
            m_worker.Cancel();
        }
    }

    ImGui::SetNextItemWidth(160);
    ImGui::InputText("Filter##name", m_filterBuf, sizeof(m_filterBuf));
    ImGui::SameLine();

    const char* types[] = { "All", "Mesh", "Texture", "Shader", "Audio" };
    ImGui::SetNextItemWidth(90);
    ImGui::Combo("Type##filter", &m_typeFilter, types, IM_ARRAYSIZE(types));
    ImGui::SameLine();

    const char* sorts[] = { "Name", "Size", "Type" };
    ImGui::SetNextItemWidth(80);
    ImGui::Combo("Sort##mode", &m_sortMode, sorts, IM_ARRAYSIZE(sorts));
}

void AssetBrowserWindow::RenderStats() const
{
    const auto s = m_registry.GetStats();
    const float total_mb = (s.totalBytes / (1024.0f * 1024.0f));

    ImGui::Text("Total: %d", m_registry.Count());
    ImGui::SameLine(0, 20);
    ImGui::TextDisabled("Mesh: %d", s.mesh);
    ImGui::SameLine(0, 20);
    ImGui::TextDisabled("Tex: %d", s.texture);
    ImGui::SameLine(0, 20);
    ImGui::TextDisabled("Shader: %d", s.shader);
    ImGui::SameLine(0, 20);
    ImGui::TextDisabled("Audio: %d", s.audio);
    ImGui::SameLine(0, 20);
    ImGui::TextDisabled("%.1f MB", total_mb);
}

void AssetBrowserWindow::RenderAssetList()
{
    auto type_filter = static_cast<EAssetType>(m_typeFilter);
    auto assets = m_registry.Filter(m_filterBuf, type_filter);

    switch (m_sortMode)
    {
        case 1: // Size
            std::sort(assets.begin(), assets.end(),
                [](const AssetInfo* a, const AssetInfo* b)
                { return (a->sizeBytes > b->sizeBytes); });
            break;
        case 2: // Type
            std::sort(assets.begin(), assets.end(),
                [](const AssetInfo* a, const AssetInfo* b)
                { return static_cast<int>(a->type) < static_cast<int>(b->type); });
            break;
        default: // Name
            std::sort(assets.begin(), assets.end(),
                [](const AssetInfo* a, const AssetInfo* b)
                { return (a->name < b->name); });
            break;
    }

    if (ImGui::BeginTable("##assets", 4,
            ImGuiTableFlags_Borders       |
            ImGuiTableFlags_RowBg         |
            ImGuiTableFlags_ScrollY       |
            ImGuiTableFlags_Resizable,
            ImVec2(0, -ImGui::GetFrameHeightWithSpacing() * 2)))
    {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 70);
        ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, 80);
        ImGui::TableSetupColumn("Status", ImGuiTableColumnFlags_WidthFixed, 80);
        ImGui::TableHeadersRow();

        for (const auto* a : assets)
        {
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);

            const bool selected = (m_selectedPath == a->path);
            if (ImGui::Selectable(a->name.c_str(), selected,
                    ImGuiSelectableFlags_SpanAllColumns))
            {
                m_selectedPath = a->path;
            }

            ImGui::TableSetColumnIndex(1);
            ImGui::TextDisabled("%s", AssetInfo::TypeName(a->type));

            ImGui::TableSetColumnIndex(2);
            if (a->sizeBytes >= 1024 * 1024)
                ImGui::Text("%.1f MB", static_cast<float>(a->sizeBytes) / (1024.0f * 1024.0f));
            else if (a->sizeBytes >= 1024)
                ImGui::Text("%.0f KB", static_cast<float>(a->sizeBytes) / 1024.0f);
            else
                ImGui::Text("%llu B", a->sizeBytes);

            ImGui::TableSetColumnIndex(3);

            switch (a->status)
            {
                case EAssetStatus::Ready:
                    ImGui::TextColored(ImVec4(0.2f,0.8f,0.4f,1.0f), "Ready");
                    break;
                case EAssetStatus::Importing:
                    ImGui::TextColored(ImVec4(1.0f,0.7f,0.0f,1.0f), "Importing");
                    break;
                case EAssetStatus::Failed:
                    ImGui::TextColored(ImVec4(1.0f,0.3f,0.3f,1.0f), "Failed");
                    break;
                default:
                    ImGui::TextDisabled("Unknown");
            }
        }

        ImGui::EndTable();
    }
}

void AssetBrowserWindow::RenderStatusBar() const
{
    if (!m_selectedPath.empty())
        ImGui::TextDisabled("Selected: %s", m_selectedPath.c_str());
    else
        ImGui::TextDisabled("Click an asset to select");
}
