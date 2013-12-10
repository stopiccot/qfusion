solution "qfusion"
    configurations { "Debug", "Release" }
    if os.is("macosx") then
        platforms { "native" }
    else
        platforms { "x32", "x64" }
    end

    function qf_targetdir(dir)
        cfg = configuration()
        
        configuration { cfg.terms, "Debug" }
        targetdir ("../build/debug/" .. dir)

        configuration { cfg.terms, "Release" }
        targetdir ("../build/release/" .. dir)

        configuration(cfg.terms)
    end

    configuration "Debug"
        targetdir "build/debug"
        objdir    "build/debug/obj"
        defines   { "_DEBUG", "DEBUG" }
        
    configuration "Release"
        targetdir "build/release"
        objdir    "build/release/obj"
        defines   { "NDEBUG" }

    configuration "vs*"
        flags { "StaticRuntime" }
        targetsuffix "_$(PlatformShortName)"
        -- buildoptions { "/NODEFAULTLIB:\"libcmt\"" }
        defines     { "WIN32", "_WINDOWS", "_CRT_SECURE_NO_WARNINGS", "_SECURE_SCL=0", "CURL_STATICLIB" }

        includedirs {
            "../libsrcs/libogg/include",
            "../libsrcs/libvorbis/include",
            "../libsrcs/libtheora/include",
            "../libsrcs/libcurl/include",
            "../libsrcs/angelscript/sdk/angelscript/include",
            "../libsrcs/zlib",
            "../libsrcs/libfreetype/include",
            "../libsrcs/libpng",
            "../libsrcs/libjpeg",
            "../libsrcs/libRocket/libRocket/Include",
            "../libsrcs/SDL2/include",
            "win32/include/msvc",
            "win32/include",
        }

    configuration {"vs*", "Debug"}
        flags   { "Symbols" }
        libdirs { "win32/x86/lib/debug" }

    configuration {"vs*", "Release"}
        flags   { "Optimize" }
        libdirs { "win32/x86/lib/release" }

    configuration "macosx"
        targetsuffix "_mac"
        libdirs { "mac/Frameworks" }

    include "angelwrap"
    include "cgame"
    include "cin"
    include "ftlib"
    include "game"
    include "irc"
    include "ref_gl"
    include "snd_openal"
    include "snd_qf"
    include "tv_server"
    include "ui"

    project "qfusion"

        kind     "WindowedApp"
        language "C++"

        flags   { "WinMain"}

        configuration "vs*"
            libdirs { "$(DXSDK_DIR)/Lib/x86" }
        configuration {}
        
        files {
            "cin/cin_public.h",
            "client/l10n.h",
            "ftlib/ftlib_public.h",
            "qcommon/anticheat.h",
            "qcommon/asyncstream.h",
            "qcommon/base64.h",
            "cgame/cg_public.h",
            "client/cin.h",
            "qcommon/bsp.h",
            "qcommon/cjson.h",
            "client/client.h",
            "qcommon/cm_local.h",
            "qcommon/cmodel.h",
            "gameshared/config.h",
            "qcommon/patch.h",
            "ref_gl/r_image.h",
            "ref_gl/r_program.h",
            "ref_gl/r_surface.h",
            "ref_gl/r_trace.h",
            "ref_gl/r_vattribs.h",
            "client/console.h",
            "qcommon/cvar.h",
            "qcommon/dynvar.h",
            "client/ftlib.h",
            "qcommon/glob.h",
            "client/input.h",
            "ref_gl/iqm.h",
            "client/keys.h",
            "client/libmumblelink.h",
            "qcommon/md5.h",
            "matchmaker/mm_common.h",
            "matchmaker/mm_query.h",
            "matchmaker/mm_rating.h",
            "gameshared/q_arch.h",
            "gameshared/q_collision.h",
            "gameshared/q_comref.h",
            "gameshared/q_cvar.h",
            "gameshared/q_dynvar.h",
            "gameshared/q_keycodes.h",
            "gameshared/q_math.h",
            "gameshared/q_shared.h",
            "qcommon/qcommon.h",
            "qcommon/qfiles.h",
            "ref_gl/qgl.h",
            "ref_gl/r_backend.h",
            "ref_gl/r_backend_local.h",
            "ref_gl/r_glimp.h",
            "ref_gl/r_local.h",
            "ref_gl/r_math.h",
            "ref_gl/r_mesh.h",
            "ref_gl/r_model.h",
            "ref_gl/r_public.h",
            "ref_gl/r_shader.h",
            "ref_gl/r_shadow.h",
            "cgame/ref.h",
            "win32/resource.h",
            "server/server.h",
            "qcommon/snap_read.h",
            "qcommon/snap_write.h",
            "client/snd_public.h",
            "qcommon/svnrev.h",
            "qcommon/sys_fs.h",
            "qcommon/sys_library.h",
            "qcommon/sys_net.h",
            "gameshared/q_trie.h",
            "ui/ui_public.h",
            "qcommon/version.h",
            "client/vid.h",
            "qcommon/webdownload.h",
            "qcommon/wswcurl.h",
            "client/l10n.c",
            "qcommon/anticheat.c",
            "qcommon/ascript.c",
            "qcommon/asyncstream.c",
            "qcommon/base64.c",
            "client/cin.c",
            "qcommon/bsp.c",
            "qcommon/cjson.c",
            "client/cl_cin.c",
            "client/cl_demo.c",
            "client/cl_game.c",
            "client/cl_input.c",
            "client/cl_main.c",
            "client/cl_mm.c",
            "client/cl_parse.c",
            "client/cl_screen.c",
            "client/cl_serverlist.c",
            "client/cl_sound.c",
            "client/cl_ui.c",
            "client/cl_vid.c",
            "qcommon/cm_main.c",
            "qcommon/cm_q3bsp.c",
            "qcommon/cm_trace.c",
            "qcommon/cmd.c",
            "qcommon/common.c",
            "server/sv_web.c",
            "client/console.c",
            "qcommon/cvar.c",
            "qcommon/dynvar.c",
            "qcommon/files.c",
            "client/ftlib.c",
            "qcommon/glob.c",
            "qcommon/irc.c",
            "client/keys.c",
            "client/libmumblelink.c",
            "qcommon/library.c",
            "qcommon/md5.c",
            "qcommon/mem.c",
            "qcommon/mlist.c",
            "matchmaker/mm_common.c",
            "matchmaker/mm_query.c",
            "matchmaker/mm_rating.c",
            "qcommon/msg.c",
            "qcommon/net.c",
            "qcommon/net_chan.c",
            "qcommon/patch.c",
            "gameshared/q_math.c",
            "gameshared/q_shared.c",
            "qcommon/snap_demos.c",
            "qcommon/snap_read.c",
            "qcommon/snap_write.c",
            "server/sv_ccmds.c",
            "server/sv_client.c",
            "server/sv_demos.c",
            "server/sv_game.c",
            "server/sv_init.c",
            "server/sv_main.c",
            "server/sv_mm.c",
            "server/sv_motd.c",
            "server/sv_oob.c",
            "server/sv_send.c",
            "qcommon/svnrev.c",
            "gameshared/q_trie.c",
            "qcommon/webdownload.c",
            "qcommon/wswcurl.c",
            "sdl/sdl_lib.c",
            "sdl/sdl_sys.c",
            "sdl/sdl_vid.c",
            "sdl/sdl_glw.h",
            "sdl/sdl_input.c",
        }

        configuration "windows"
            files {
                "sdl/sdl_sys_win32.c",
                --"win32/winquake.h",
                --"win32/conproc.c",
                "win32/win_fs.c",
                "win32/win_net.c",
                "win32/qfusion.rc",
            }

            links { "cin", "libcurlstat", "zlibstat", "winmm", "ws2_32", "dxguid", "SDL2", "SDL2main" }

        configuration "macosx"
            links { "cin", "libcurl.framework", "SDL2.framework", "IOKit.framework", "libpng.framework", "jpeg.framework", "Cocoa.framework", "z"}

    project "qfusion_server"

        kind     "WindowedApp"
        language "C++"

        flags   { "WinMain"}
        defines { "DEDICATED_ONLY" }

        files {
            "qcommon/anticheat.c",
            "qcommon/ascript.c",
            "qcommon/base64.c",
            "qcommon/bsp.c",
            "qcommon/cjson.c",
            "null/cl_null.c",
            "qcommon/cm_main.c",
            "qcommon/cm_q3bsp.c",
            "qcommon/cm_trace.c",
            "qcommon/cmd.c",
            "qcommon/common.c",
            "server/sv_web.c",
            "win32/conproc.c",
            "qcommon/cvar.c",
            "qcommon/dynvar.c",
            "qcommon/files.c",
            "qcommon/glob.c",
            "qcommon/irc.c",
            "qcommon/md5.c",
            "qcommon/mem.c",
            "qcommon/mlist.c",
            "matchmaker/mm_common.c",
            "matchmaker/mm_query.c",
            "matchmaker/mm_rating.c",
            "qcommon/msg.c",
            "qcommon/net.c",
            "qcommon/net_chan.c",
            "qcommon/patch.c",
            "gameshared/q_math.c",
            "gameshared/q_shared.c",
            "qcommon/snap_demos.c",
            "qcommon/snap_write.c",
            "server/sv_ccmds.c",
            "server/sv_client.c",
            "server/sv_demos.c",
            "server/sv_game.c",
            "server/sv_init.c",
            "server/sv_main.c",
            "server/sv_mm.c",
            "server/sv_motd.c",
            "server/sv_oob.c",
            "server/sv_send.c",
            "qcommon/svnrev.c",
            "gameshared/q_trie.c",
            "qcommon/webdownload.c",
            "win32/win_fs.c",
            "win32/win_net.c",
            "win32/win_sys.c",
            "qcommon/wswcurl.c",
            "qcommon/library.c",
            "win32/win_lib.c",
            "gameshared/config.h",
            "qcommon/anticheat.h",
            "qcommon/base64.h",
            "qcommon/bsp.h",
            "qcommon/cjson.h",
            "qcommon/cm_local.h",
            "qcommon/cmodel.h",
            "win32/conproc.h",
            "qcommon/cvar.h",
            "qcommon/dynvar.h",
            "game/g_public.h",
            "qcommon/glob.h",
            "qcommon/md5.h",
            "matchmaker/mm_common.h",
            "matchmaker/mm_query.h",
            "matchmaker/mm_rating.h",
            "gameshared/q_angelwrap.h",
            "gameshared/q_arch.h",
            "gameshared/q_collision.h",
            "gameshared/q_comref.h",
            "gameshared/q_cvar.h",
            "gameshared/q_dynvar.h",
            "gameshared/q_keycodes.h",
            "gameshared/q_math.h",
            "gameshared/q_shared.h",
            "qcommon/qcommon.h",
            "qcommon/qfiles.h",
            "server/server.h",
            "qcommon/snap_read.h",
            "qcommon/snap_write.h",
            "qcommon/svnrev.h",
            "qcommon/sys_fs.h",
            "qcommon/sys_library.h",
            "qcommon/sys_net.h",
            "gameshared/q_trie.h",
            "qcommon/version.h",
            "qcommon/webdownload.h",
            "win32/winquake.h",
        }

        links {
            "libcurlstat",
            "zlibstat",
            "winmm",
            "ws2_32",
        }
