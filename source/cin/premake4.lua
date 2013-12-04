project "cin"

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
        links { "libogg_static", "libvorbis_static", "libvorbisfile_static", "libtheora_static" }

    configuration "macosx"
        links { "Ogg.framework", "Vorbis.framework", "Theora.framework" }

