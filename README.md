![ccamAtisSepia](banner.png "The CCam ATIS Sepia banner")

CCam ATIS Sepia is an extension to the [Sepia](https://github.com/neuromorphic-paris/sepia) library to read events from a CCam ATIS.

# install

## clone

Within a Git repository, run the commands:

```sh
mkdir -p third_party
cd third_party
git submodule add https://github.com/neuromorphic-paris/ccam_atis_sepia.git
git submodule update --init --recursive
```

## dependencies

CCam ATIS Sepia depends on [libusb](http://libusb.info). Follow these steps to install it:
- __Debian / Ubuntu__: Open a terminal and execute the command `sudo apt-get install libusb-1.0`.
- __macOS__: Open a terminal and execute the command `brew install libusb`. If the command is not found, you need to install Homebrew first with the command<br />
  `ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`.

An application using CCam ATIS Sepia must link to usb-1.0.

# contribute

## development dependencies

CCam ATIS Sepia relies on [Premake 4.x](https://github.com/premake/premake-4.x) (x ≥ 3) to generate build configurations. Follow these steps to install it:
- __Debian / Ubuntu__: Open a terminal and execute the command `sudo apt-get install premake4`.
- __macOS__: Open a terminal and execute the command `brew install premake`. If the command is not found, you need to install Homebrew first with the command<br />
`ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`.

[ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) is used to unify coding styles. Follow these steps to install it:
- __Debian / Ubuntu__: Open a terminal and execute the command `sudo apt-get install clang-format`.
- __macOS__: Open a terminal and execute the command `brew install clang-format`. If the command is not found, you need to install Homebrew first with the command<br />
`ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`.

## test

To test the library, run from the *ccam_atis_sepia* directory:
```sh
premake4 gmake
cd build
make
cd release
./ccam_atis_sepia
```

After changing the code, format the source files by running from the *ccam_atis_sepia* directory:
```sh
clang-format -i source/ccam_atis_sepia.hpp
clang-format -i test/ccam_atis_sepia.cpp
```

# user guides and documentation

User guides and code documentation are held in the [wiki](https://github.com/neuromorphic-paris/ccamAtisSepia/wiki).

# license

See the [LICENSE](LICENSE.txt) file for license rights and limitations (GNU GPLv3).
