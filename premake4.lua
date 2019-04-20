solution 'ccam_atis_sepia'
    configurations {'release', 'debug'}
    location 'build'
    project 'ccam_atis_sepia'
        kind 'ConsoleApp'
        language 'C++'
        location 'build'
        files {'source/*.hpp', 'test/*.cpp'}
        defines {'SEPIA_COMPILER_WORKING_DIRECTORY="' .. project().location .. '"'}
        configuration 'release'
            targetdir 'build/release'
            defines {'NDEBUG'}
            flags {'OptimizeSpeed'}
        configuration 'debug'
            targetdir 'build/debug'
            defines {'DEBUG'}
            flags {'Symbols'}
        configuration 'linux'
            links {'pthread', 'usb-1.0'}
            buildoptions {'-std=c++11'}
            linkoptions {'-std=c++11'}
        configuration 'macosx'
            includedirs {'/usr/local/include'}
            libdirs {'/usr/local/lib'}
            links {'usb-1.0'}
            buildoptions {'-std=c++11'}
            linkoptions {'-std=c++11'}
        configuration 'windows'
            files {'.clang-format'}
            includedirs {'C:\\Include'}
            links {'C:\\Windows\\SysWOW64\\libusb-1.0'}
