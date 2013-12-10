project "ref_gl"

    kind      "SharedLib"
    language  "C"
    qf_targetdir "libs"
    
    files    { 
        "*.h",
        "../cgame/ref.h",
        "../gameshared/anorms.h",
        "../gameshared/config.h",
        "../gameshared/q_arch.h",
        "../gameshared/q_cvar.h",
        "../gameshared/q_math.h",
        "../gameshared/q_shared.h",
        "../qcommon/patch.h",
        "../gameshared/q_trie.h",
        "*.c",
        "../gameshared/q_math.c",
        "../gameshared/q_shared.c",
        "../qcommon/bsp.c",
        "../qcommon/patch.c",
        "../gameshared/q_trie.c",
        "../sdl/sdl_glw.h",
        "../sdl/sdl_glw.c",
        "../sdl/sdl_qgl.c",
    }

    configuration "windows"
        links { "libpngstat", "libjpegstat", "zlibstat", "SDL2" }

    configuration "macosx"
        links { "libpng.framework", "jpeg.framework", "SDL.framework" }