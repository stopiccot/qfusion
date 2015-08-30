set(THEORA_PUBLIC_HEADERS
    ../include/theora/codec.h
    ../include/theora/theora.h
    ../include/theora/theoraenc.h
    ../include/theora/theoradec.h
)

set(THEORA_HEADERS
    apiwrapper.h
    bitpack.h
    dct.h
    decint.h
    dequant.h
    encint.h
    enquant.h
    huffdec.h
    huffenc.h
    huffman.h
    internal.h
    mathops.h
    modedec.h
    ocintrin.h
    quant.h
    state.h
    
    x86/mmxloop.h
    x86/sse2trans.h
    x86/x86cpu.h
    x86/x86enc.h
    x86/x86int.h
    x86/x86zigzag.h
)

set(THEORA_SHARED_SOURCES
    x86/x86cpu.c
    x86/mmxidct.c
    x86/mmxfrag.c
    x86/mmxstate.c
    x86/sse2idct.c
    x86/x86state.c

    apiwrapper.c
    bitpack.c
    dequant.c
    fragment.c
    idct.c
    internal.c
    state.c
    quant.c
)

set(THEORAENC_SOURCES
    x86/mmxencfrag.c
    x86/mmxfdct.c
    x86/sse2encfrag.c
    x86/x86enquant.c
    x86/x86enc.c
    x86/sse2fdct.c

    analyze.c
    fdct.c
    encfrag.c
    encapiwrapper.c
    encinfo.c
    encode.c
    enquant.c
    huffenc.c
    mathops.c
    mcenc.c
    rate.c
    tokenize.c
)

set(THEORADEC_SOURCES
    decapiwrapper.c
    decinfo.c
    decode.c
    huffdec.c
    info.c
)

include_directories(../include)
include_directories(.)
include_directories(${OGG_INCLUDE_DIRS})

add_definitions(-DOC_X86_ASM)
add_definitions(-DOC_X86_64_ASM)

if (NOT BUILD_FRAMEWORK)
    add_library(theora ${THEORA_HEADERS} ${THEORAENC_SOURCES} ${THEORADEC_SOURCES} ${THEORA_SHARED_SOURCES})
    add_library(theoraenc ${THEORAENC_SOURCES} ${THEORA_SHARED_SOURCES})
    add_library(theoradec ${THEORADEC_SOURCES} ${THEORA_SHARED_SOURCES})

    get_version_info(THEORA_VERSION_INFO "TH_LIB_CURRENT" "TH_LIB_AGE" "TH_LIB_REVISION")
    set_target_properties(theora PROPERTIES SOVERSION ${THEORA_VERSION_INFO})
    get_version_info(THEORAENC_VERSION_INFO "THENC_LIB_CURRENT" "THENC_LIB_AGE" "THENC_LIB_REVISION")
    set_target_properties(theoraenc PROPERTIES SOVERSION ${THEORAENC_VERSION_INFO})
    get_version_info(THEORADEC_VERSION_INFO "THDEC_LIB_CURRENT" "THDEC_LIB_AGE" "THDEC_LIB_REVISION")
    set_target_properties(theoradec PROPERTIES SOVERSION ${THEORADEC_VERSION_INFO})

    install(FILES ${THEORA_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/theora)

    install(TARGETS theora    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR})
    install(TARGETS theoraenc RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR})
    install(TARGETS theoradec RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR})
else()
    add_library(theora ${THEORA_PUBLIC_HEADERS} ${THEORA_HEADERS} ${THEORAENC_SOURCES} ${THEORADEC_SOURCES} ${THEORA_SHARED_SOURCES})
    set_target_properties(theora PROPERTIES
        FRAMEWORK TRUE
        FRAMEWORK_VERSION ${PROJECT_VERSION}
        MACOSX_FRAMEWORK_IDENTIFIER org.xiph.theora
        MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${PROJECT_VERSION}
        MACOSX_FRAMEWORK_BUNDLE_VERSION ${PROJECT_VERSION}
        XCODE_ATTRIBUTE_INSTALL_PATH "@rpath"
        PUBLIC_HEADER "${THEORA_PUBLIC_HEADERS}"
        OUTPUT_NAME Theora
    )
endif()
