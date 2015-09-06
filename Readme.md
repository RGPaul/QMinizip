# QMinizip
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/Manromen/QMinizip/blob/master/LICENSE)

## Installation
1. Init git submodules (to install minizip).
2. Open QMinizip.pro and compile the shared library.
3. Add libQMinizip to your project.
4. Add zlib to your project.

If you are on Linux / Mac OS X you can install the library over the command line.

```shell
qmake -o Makefile QMinizip.pro
make
sudo make install
```

## Usage
For now there is an example Project.

## License
QMinizip is licensed under the [MIT license](https://github.com/Manromen/QMinizip/blob/master/LICENSE) and [Minizip](http://www.winimage.com/zLibDll/minizip.html) is licensed under the [Zlib license](http://www.zlib.net/zlib_license.html).
