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
        "../win32/win_glw.h",
        "*.c",
        "../gameshared/q_math.c",
        "../gameshared/q_shared.c",
        "../qcommon/bsp.c",
        "../qcommon/patch.c",
        "../gameshared/q_trie.c",
        "../win32/win_glw.c",
        "../win32/win_qgl.c",
    }

    links {
        "libpngstat",
        "libjpegstat",
        "zlibstat"
    }