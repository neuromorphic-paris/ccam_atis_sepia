solution 'ccamAtisSepia'
    configurations {'Release', 'Debug'}
    location 'build'

    newaction {
        trigger = "install",
        description = "Install the library",
        execute = function()
            os.copyfile('source/sepia.hpp', '/usr/local/include/ccamAtisSepia.hpp')
            print(string.char(27) .. '[32mCCam ATIS Sepia library installed.' .. string.char(27) .. '[0m')
            os.exit()
        end
    }

    newaction {
        trigger = 'uninstall',
        description = 'Remove all the files installed during build processes',
        execute = function()
            os.execute('rm -f /usr/local/include/ccamAtisSepia.hpp')
            print(string.char(27) .. '[32mCCam ATIS Sepia library uninstalled.' .. string.char(27) .. '[0m')
            os.exit()
        end
    }

    newoption {
        trigger = 'skip-dependencies',
        description = 'Do not install the library dependencies'
    }

    project 'ccamAtisSepiaTest'
        -- General settings
        kind 'ConsoleApp'
        language 'C++'
        location 'build'
        files {'source/**.hpp', 'test/**.hpp', 'test/**.cpp'}

        -- Define the include paths
        includedirs {'/usr/local/include'}
        libdirs {'/usr/local/lib'}

        -- Link to the required libraries
        links {'usb-1.0'}

        -- Declare the configurations
        configuration 'Release'
            targetdir 'build/Release'
            defines {'NDEBUG'}
            flags {'OptimizeSpeed'}
        configuration 'Debug'
            targetdir 'build/Debug'
            defines {'DEBUG'}
            flags {'Symbols'}

        -- Linux specific settings
        configuration 'linux'
            buildoptions {'-std=c++11'}
            linkoptions {'-std=c++11'}
            links {'pthread'}
            postbuildcommands {'cp ../source/ccamAtisSepia.hpp /usr/local/include/ccamAtisSepia.hpp'}

        -- Mac OS X specific settings
        configuration 'macosx'
            buildoptions {'-std=c++11', '-stdlib=libc++'}
            linkoptions {'-std=c++11', '-stdlib=libc++'}
            postbuildcommands {'cp ../source/ccamAtisSepia.hpp /usr/local/include/ccamAtisSepia.hpp'}
