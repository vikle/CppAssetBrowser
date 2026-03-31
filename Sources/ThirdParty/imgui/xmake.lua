-- xmake.lua

target("imgui")
    set_kind("static")
    set_group("ThirdParty")

    add_includedirs("include", {public = true})
    add_includedirs("backends", {public = true})

    add_files("**.cpp")

    add_deps("GLFW")

