workspace "CryptoComImpl"
    architecture "x64"
    startproject "CryptoComImpl"

    configurations
    {
        "Debug",
        "Release"
    }

    outputdir = "%{cfg.buildcfg}"

    IncludeDir = {}
    IncludeDir["boost"] = "/usr/include/boost"
    IncludeDir["crypto_com"] = "include"
    IncludeDir["cryptlite"] = "vendor/cpp-cryptlite/include"
    IncludeDir["http_request"] = "vendor/HTTPRequest/include"
    IncludeDir["json"] = "vendor/json/single_include"
    IncludeDir["websocketpp"] = "/usr/include/websocketpp"

    CppVersion = "C++20"

    project "CryptoComImpl"
        location "src"
        kind "ConsoleApp"
        language "C++"

        targetdir ("bin/" .. outputdir)
        objdir ("bin/int/" .. outputdir  .. "%{prj.name}")

        files
        {
            "include/crypto_com/**.hpp",
            "include/crypto_com/**.cpp",
            "src/**.asm",
            "src/**.hpp",
            "src/**.cpp",
            "src/**.h",
        }

        includedirs
        {
            "%{IncludeDir.boost}",
            "%{IncludeDir.crypto_com}",
            "%{IncludeDir.cryptlite}",
            "%{IncludeDir.http_request}",
            "%{IncludeDir.json}",
            "%{IncludeDir.websocketpp}",
            "include",
            "src"
        }

        libdirs
        {
            "bin/lib"
        }

        links
        {
            "crypto",
            "pthread",
            "ssl"
        }

        flags
        {
            "LinkTimeOptimization",
            "NoImportLib",
            "Maps",
            "MultiProcessorCompile"
        }

        filter "configurations:Debug"
            flags {  }
            defines { "DEBUG", "_DEBUG" }
            symbols "On"

        filter "configurations:Release"
            flags { "NoManifest" }
            defines { "RELEASE", "NDEBUG" }
            optimize "speed"