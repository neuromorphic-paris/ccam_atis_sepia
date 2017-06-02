![ccamAtisSepia](ccamAtisSepiaBanner.png "The CCam ATIS Sepia banner")

# CCam ATIS Sepia

Ccam ATIS Sepia is an extension to the [Sepia](https://github.com/neuromorphic-paris/sepia) library to read events from a CCam ATIS.

# Installation

## Dependencies

CCam ATIS Sepia depends on the [Sepia](https://github.com/neuromorphic-paris/sepia) library, which can be installed by executing the following commands from a terminal:
```sh
git clone https://github.com/neuromorphic-paris/sepia.git
cd sepia && premake4 install && cd .. && rm -rf sepia
```
Moreover, CCam ATIS Sepia depends on [libusb](http://libusb.info), which can be installed by running:
  - __Debian / Ubuntu__: `sudo apt-get install libusb-1.0`.
  - __Fedora__: `sudo dnf install libusb`.
  - __OS X__: `brew install libusb`.

## Install

To install the source, go to the *ccamAtisSepia* directory and run `premake4 install`. The source is copied to */usr/local/include*.
The firmwares are installed in */usr/local/share*. You need to be connected to the Vision Institute local network for this step to work, as it implies downloading close-source resources.

## Uninstall

To uninstall the library, run `premake4 uninstall` from the *ccamAtisSepia* directory.

## Test

To test the library, run the following commands:
  - Go to the *ccamAtisSepia* directory and run `premake4 gmake && cd build && make`.
  - Run the executable *Release/ccamAtisSepiaTest*.

# User guides and documentation

User guides and code documentation are held in the [wiki](https://github.com/neuromorphic-paris/ccamAtisSepia/wiki).

# License

See the [LICENSE](LICENSE.txt) file for license rights and limitations (GNU GPLv3).
