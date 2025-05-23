set_xmakever("2.8.2")
set_project("frozen")

set_version("1.1.0", { build = "%Y%m%d%H%M" })

option("modules", { defines = "FROZEN_USE_MODULE", default = false })
option("std_module", { defines = "FROZEN_USE_STD_MODULE", default = false })
option("tests", { default = false })
option("benchmark", { default = false })

local kind = "static"
if xmake:version():ge("2.8.7") then
    kind = "moduleonly"
end

target("frozen", function()
    if get_config("modules") then
        set_kind(kind)
        if get_config("std_module") then
            set_languages("c++23")
        else
            set_languages("c++20")
        end
    else
        set_kind("headeronly")
        set_languages("c++17")
    end

    add_includedirs("include", { public = true })
    add_headerfiles("include/(frozen/**.h)")

    add_options("modules", "std_module")

    if get_config("modules") then
        add_files("module/frozen.cppm", { public = true })
    end
end)

if get_config("benchmark") then
    add_requires("benchmark")
    target("frozen.benchmark", function()
        set_kind("binary")
        set_languages("c++17")

        add_files("benchmarks/**.cpp")

        if not get_config("benchmark_str_search") then
            remove_files("benchmarks/bench_str_search.cpp")
        end

        add_packages("benchmark")

        add_deps("frozen")
    end)
end

if get_config("tests") then
    target("frozen.tests", function()
        set_kind("binary")
        if get_config("modules") then
            if get_config("std_module") then
                set_languages("c++23")
            else
                set_languages("c++20")
            end
        else
            set_languages("c++17")
        end
        add_rules("mode.coverage")
        add_files("tests/**.cpp")
        remove_files("tests/no_exceptions.cpp")

        add_cxxflags("/W3", "/WX", { tools = { "cl", "clang_cl" } })
        add_cxxflags("-Wall", "-Wextra", "-Wpedantic", "-Werror", "-Wshadow", { tools = { "cl", "clang_cl" } })

        if is_plat("windows") then
            add_cxxflags("/W3", "/WX", { tools = "icc" })
        else
            add_cxxflags("-Wall", "-Werror", { tools = "icc" })
        end

        add_options("modules", "std_module")
        add_deps("frozen")
    end)

    target("frozen.tests.noexcept", function()
        set_kind("binary")
        if get_config("modules") then
            if get_config("std_module") then
                set_languages("c++23")
            else
                set_languages("c++20")
            end
        else
            set_languages("c++17")
        end
        add_rules("mode.coverage")
        set_basename("test_no_exceptions")

        add_files("tests/no_exceptions.cpp")

        add_cxxflags("/W3", "/WX", { tools = { "cl", "clang_cl" } })
        add_cxxflags("-Wall", "-Wextra", "-Wpedantic", "-Werror", "-Wshadow", { tools = { "cl", "clang_cl" } })

        if is_plat("windows") then
            add_cxxflags("/W3", "/WX", { tools = { "icc" } })
        else
            add_cxxflags("-Wall", "-Werror", { tools = { "icc" } })
        end

        add_options("modules", "std_module")
        add_deps("frozen")
    end)

    for _, example in ipairs(os.files("examples/*.cpp")) do
        target("frozen.example." .. path.basename(example), function()
            set_kind("binary")
            if get_config("modules") then
                if get_config("std_module") then
                    set_languages("c++23")
                else
                    set_languages("c++20")
                end
            else
                set_languages("c++17")
            end

            add_files(example)

            add_deps("frozen")

            add_options("modules", "std_module")

            if path.basename(example) == "html_entities_map" then
                add_cxxflags("-fconstexpr-steps=123456789", { tools = "clang" })
                add_cxxflags("-fconstexpr-ops-limit=12345678910", { tools = "gcc" })
            end
        end)
    end
end
