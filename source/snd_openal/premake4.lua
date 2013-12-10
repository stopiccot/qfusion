project "snd_openal"

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

    links {
        "libogg_static",
        "libvorbis_static",
        "libvorbisfile_static",
    }