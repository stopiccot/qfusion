project "snd_qf"

    kind      "SharedLib"
    language  "C"
    qf_targetdir "libs"
    
    files    { 
        "*.h",
        "../gameshared/q_math.h",
        "../gameshared/q_shared.h",
        "../client/snd_public.h",
        "*.c",
        "../gameshared/q_math.c",
        "../gameshared/q_shared.c",
    }

    qf_links { "ogg", "vorbis", "SDL2" }

    configuration "windows"
        files { "../win32/win_snd.c" }
        links { "winmm" }

    configuration "linux"
        files { "../unix/unix_snd.c"}

    configuration "macosx"
        files { "../unix/unix_snd.c" }
