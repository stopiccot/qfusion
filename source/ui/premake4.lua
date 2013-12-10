project "ui"

    kind      "SharedLib"
    language  "C++"
    
    qf_targetdir "base"

    -- pchheader "ui_precompiled.h"
    -- pchsource "ui_precompiled.cpp"

    includedirs { "." }
    defines     { "STATIC_LIB" }

    files { 
        "*.h",
        "as/*.h",
        "datasources/*.h",
        "decorators/*.h",
        "formatters/*.h",
        "kernel/*.h",
        "widgets/*.h",
        "../gameshared/config.h",
        "../gameshared/gs_qrespath.h",
        "../gameshared/gs_ref.h",
        "../gameshared/q_arch.h",
        "../gameshared/q_cvar.h",
        "../gameshared/q_dynvar.h",
        "../gameshared/q_keycodes.h",
        "../gameshared/q_math.h",
        "../gameshared/q_shared.h",
        "*.cpp",
        "as/*.cpp",
        "datasources/*.cpp",
        "decorators/*.cpp",
        "formatters/*.cpp",
        "kernel/*.cpp",
        "widgets/*.cpp",
        "../gameshared/q_math.c",
        "../gameshared/q_shared.c",
    }

    configuration "windows"
        links { "RocketCore", "RocketControls" }

    configuration "macosx"
        --links { "Rocket.framework" }
        links { "RocketCoreOSX.framework", "RocketControlsOSX.framework" }