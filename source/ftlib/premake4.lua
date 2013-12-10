project "ftlib"

    kind      "SharedLib"
    language  "C"
    qf_targetdir "libs"
    
    files    { 
        "*.h",
        "../gameshared/q_math.h",
        "../gameshared/q_shared.h",
        "*.c",
        "../gameshared/q_math.c",
        "../gameshared/q_shared.c",
    }

    configuration "windows"
        links { "libfreetypestat" }

    configuration "macosx"
        links { "FreeType.framework" }
        includedirs { "../mac/Frameworks/FreeType.framework/Headers"}
            