# Building

Below are the instructions for both developers or users that, for any reason, need to rebuild plugin from the source and not rely on the distributed packages.

## Requirements
 * Clone of the plugin source `https://github.com/n7space/asn1scc.IDE.git`
 * C++14 compatible compiler
 * Qt 5.12
 * Qt Creator v4.9.0 built from source (instructions [here](https://doc-snapshots.qt.io/qtcreator-extending/getting-and-building.html))
 * [for debugging] Qt Creator built from source with `CONFIG+=debug`
 * [for development] Separate Qt Creator for editing source
 
## Compilation
 1. Checkout the desired plugin version
 2. Create folder for the build
 3. Go into the build directory and execute `qmake -r <path to plugin source>/asn1acn.pro <QMake options, OPTION=VALUE>`, see below for available options
 4. Build plugin (`make` on Linux, `bjam`/`nmake` on Windows etc.)
 5. Qt Creator selected during `qmake` run should have plugin installed

## Building using Qt Creator
 1. Open `asn1acn.pro` in Qt Creator
 2. Configure the build using options below
 
## QMake options
While configuring the build two options need to be set properly for plugin to build properly:
 * `QTC_SOURCE` - path to Qt Creator's source
 * `QTC_BUILD` - path to Qt Creator's build

If not provided, Linux defaults will be used (`/opt/qt-creator-dev/qt-creator` and `/opt/qt-creator-dev/build-debug`), which might result in multiple compilation errrors (missing includes etc.).
 
## Executing tests
To run unit tests of the plugin, execute Qt Creator with `-test "ASN.1/ACN"` option.
Executing tests in "release" mode requires Qt Creator to be built with `CONFIG+=release BUILD_TESTS=1` QMake options.

## Updating Travis
Travis task executes plugin's unit tests, which requires specific build of Qt Creator, which is not distributed.
Developers need to update Travis environment each time supported Qt Creator version is changed.
 1. Create Ubuntu 14.04 machine with g++, make and 7z packages
 2. Install proper Qt package (recommended - [ppa:beineri](https://launchpad.net/~beineri))
 3. Clone required Qt Creator version and checkout tag
 4. Configure Qt Creator build: `qmake -r ../qtcreator CONFIG+=release BUILD_TESTS=1`
 5. Build Qt Creator: `make`
 6. Prepare Qt Creator distribution: `make INSTALL_ROOT=~/qtcreator install`
 7. Go to `~/qtcreator` and compress it: `7z a ../qtcreator.7z *`
 8. Make new archive available for Travis machine
