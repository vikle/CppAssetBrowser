-- xmake.lua

add_rules("mode.debug", "mode.release", "mode.releasedbg")

-- Basics
set_languages("c++20", "c99")
set_rundir(".")
add_includedirs("Sources", "Sources/ThirdParty", { public = true })

-- Platform defines
if is_plat("windows") then
    add_defines("_WINDOWS", { public = true })
elseif is_plat("linux") then
    add_defines("_LINUX", { public = true })
    add_rpathdirs("Binaries/Linux", "$(builddir)/$(plat)/$(arch)/$(mode)/", { public = true })
    add_cflags("-fPIC", { public = true })
    add_cxxflags("-fPIC", { public = true })
elseif is_plat("macosx") then
    add_defines("_MACOS", { public = true })
    add_rpathdirs("Binaries/Mac", "$(builddir)/$(plat)/$(arch)/$(mode)/", { public = true })
end

-- Build config
if is_mode("debug") then
    add_defines("_DEBUG", {public = true})
    set_symbols("debug")
    set_optimize("none")
elseif is_mode("releasedbg") then
    add_defines("_DEBUG", {public = true})
    set_symbols("debug")
    set_optimize("faster")
else
    set_symbols("hidden")
    set_optimize("fastest")
    if is_plat("linux") then
        set_strip("all")
    end
end

-- Other defines
--add_defines("",{ public = true })

includes("Sources")