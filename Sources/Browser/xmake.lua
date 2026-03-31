-- xmake.lua

target("CppAssetBrowser")
    set_kind("binary")
    if is_plat("windows") then
        add_rules("utils.symbols.export_all", {export_classes = true})
        add_syslinks("ole32", "comdlg32")
    end

    if is_plat("macosx") then
        add_frameworks("OpenGL", "Cocoa", "IOKit", "CoreVideo")
    end

    set_group("Browser")

    add_includedirs(".", {public = true})

    add_files("**.cpp")

    add_deps("GLFW","imgui")
