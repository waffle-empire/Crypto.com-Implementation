workspace "CryptoCom"
  architecture "x64"
  startproject "CryptoCom"

  configurations
  {
    "Debug",
    "Release"
  }

  outputdir = "%{cfg.buildcfg}"

  IncludeDir = {}
	IncludeDir["boost"] = "/usr/include/boost"
  IncludeDir["json"] = "vendor/json/single_include"
	IncludeDir["websocketpp"] = "/usr/include/websocketpp"
  
  CppVersion = "C++17"

   
  function DeclareDebugOptions()
    filter "configurations:Debug"
      defines { "_DEBUG" }
      symbols "On"
    filter "not configurations:Debug"
      defines { "NDEBUG" }
  end

  project "CryptoCom"
    location "CryptoCom"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir)
    objdir ("bin/int/" .. outputdir .. "/%{prj.name}")

    PrecompiledHeaderInclude = "common.hpp"
    PrecompiledHeaderSource = "common.cpp"
 
    files
    {
      "%{prj.name}/src/**.hpp",
      "%{prj.name}/src/**.h",
      "%{prj.name}/src/**.cpp",
      "%{prj.name}/src/**.asm"
    }

    includedirs
    {
			"%{IncludeDir.boost}",
      "%{IncludeDir.json}",
			"%{IncludeDir.websocketpp}",
      "%{prj.name}/src"
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

    pchheader "%{PrecompiledHeaderInclude}"
    pchsource "%{PrecompiledHeaderSource}"

    forceincludes
    {
      "%{PrecompiledHeaderInclude}"
    }

    DeclareDebugOptions()

    flags { "NoImportLib", "Maps" }

    filter "configurations:Debug"
	  flags { "LinkTimeOptimization", "MultiProcessorCompile" }
	  editandcontinue "Off"
      defines { "DEBUG" }

    filter "configurations:Release"
	  flags { "LinkTimeOptimization", "NoManifest", "MultiProcessorCompile" }
      defines { "RELEASE" }
      optimize "speed"
    filter "configurations:Dist"
      flags { "LinkTimeOptimization", "FatalWarnings", "NoManifest", "MultiProcessorCompile" }
      defines { "DIST" }
      optimize "speed"
