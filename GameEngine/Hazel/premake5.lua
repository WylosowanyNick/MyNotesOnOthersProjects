workspace "Hazel"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }

-- Przykład tokenu https://github.com/premake/premake-core/wiki/Tokens
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Hazel" -- nazwa projektu
  location "Hazel" -- względna lokalizacja projektu
  kind "SharedLib" -- biblioteka dynamiczna
  language "C++" -- język C++

  targetdir ("bin/" .. outputdir .. "/%{prj.name}") -- pliki docelowe
  objdir ("bin-int/" .. outputdir .. "/%{prj.name}") -- pliki pośrednie kompilacji

  files -- uwzględnienie wszystkich plików nagłówkowych i źródłowych
  {
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp"
  }

  -- !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  -- AKTUALIZOWAĆ POWYŻSZE I PONIŻSZE WRAZ Z DODAWANIEM BIBLIOTEK
  includedirs -- ścieżki do plików nagłówkowych wykorzystywanych bibliotek
  {
    "%{prj.name}/vendor/spdlog/include"
  }

  -- SKŁADNIA FILTRA DZIAŁA DO KOLEJNEGO FILTRA LUB DO NASTĘPNEGO PROJEKTU
  filter "system:windows" -- Jeżeli system to windows
    cppdialect "C++17"
    staticruntime "On" -- Statyczne linkowanie bibliotek runtime
    systemversion "10.0.19041.0" -- Windows SDK Version, ewentualnie można użyć "latest"

    defines -- Definicja makr preprocesora
    {
      "HZ_PLATFORM_WINDOWS",
      "HZ_BUILD_DLL"
    }

    postbuildcommands -- Komenda cmd wykonywana po budowie
    {
      "{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox"
    }

    filter "configurations:Debug"
      defines "HZ_DEBUG"
      symbols "On" -- Symbole debugowania włączone

    filter "configurations:Release"
      defines "HZ_RELEASE"
      optimize "On" -- Optymalizacja włączona

    filter "configurations:Dist"
      defines "HZ_DIST"
      optimize "On"

  -- Przykład łączenia filtrów
  -- filter { "system:windows", "configurations:Relase" }
  --   buildoptions "/MT" -- Użyj Multithreaded Runtime Library

project "Sandbox"
  location "Sandbox"
  kind "ConsoleApp" -- aplikacja konsolowa
  language "C++"

  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
  
  files
  {
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp",
  }

  -- !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  -- AKTUALIZOWAĆ POWYŻSZE I PONIŻSZE WRAZ Z DODAWANIEM BIBLIOTEK
  includedirs
  {
    "Hazel/vendor/spdlog/include",
    "Hazel/src"
  }

  links -- Linkowanie z projektem Hazel
  {
    "Hazel"
  }

  filter "system:windows"
    cppdialect "C++17"
    staticruntime "On"
    systemversion "10.0.19041.0"

    defines
    {
      "HZ_PLATFORM_WINDOWS"
    }

    filter "configurations:Debug"
      defines "HZ_DEBUG"
      symbols "On"

    filter "configurations:Release"
      defines "HZ_RELEASE"
      optimize "On"

    filter "configurations:Dist"
      defines "HZ_DIST"
      optimize "On"
