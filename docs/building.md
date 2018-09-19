# Building

Below are instructions for both developers or users, that, for any reason, need to rebuild plugin from source and not rely on distributed packages.

## Requirements
 * Clone of plugin source `https://github.com/n7space/asn1scc.IDE.git`
 * C++14 compatible compiler
 * Qt 5.11.1
 * Qt Creator v4.7.0 built from source (instructions [here](https://doc-snapshots.qt.io/qtcreator-extending/getting-and-building.html))
 * [for debugging] Qt Creator built from source with `CONFIG+=debug`
 * [for development] Separate Qt Creator for editing source
 
## Compilation
 1. Checkout desired plugin version
 2. Create folder for build
 3. Go into build dir and execute `qmake -r <path to plugin source>/asn1acn.pro <qmake options>`, see below for available options
 4. Build plugin (`make` on Linux, `bjam`/`nmake` on Windows etc.)
 5. Qt Creator selected during `qmake` run should have plugin installed

## Building using Qt Creator
 1. Open `asn1acn.pro` in Qt Creator
 2. Configure build using options below
 
## QMake options
While configuring build, two options needs to be set properly, for plugin to build:
 * `QTC_SOURCE` - path to Qt Creator source (default `/opt/qt-creator-dev/qt-creator`)
 * `QTC_BUILD` - path to build Qt Creator (default `/opt/qt-creator-dev/build-debug`)
 
## Executing tests
To run unit tests of plugin, execute Qt Creator with `-test "ASN.1/ACN"` option.
Executing tests in "release" mode requires Qt Creator to be built with `CONFIG+=release BUILD_TESTS=1` qmake options.

## Updating Travis
Travis task executes plugin's unit tests, which requires proper build of Qt Creator, which is not distributed.
Developers need to update Travis environment each time supported Qt Creator version is changed.
 1. Create Ubuntu 14.04 machine with g++, make and 7z packages
 2. Install proper Qt package (probably from [ppa:beineri](https://launchpad.net/~beineri))
 3. Clone required Qt Creator version and checkout tag
 4. Configure Qt Creator build: `qmake -r ../qtcreator CONFIG+=release BUILD_TESTS=1`
 5. Build Qt Creator: `make`
 6. Prepare Qt Creator distribution: `make INSTALL_ROOT=~/qtcreator install`
 7. Go to `~/qtcreator` and compress it: `7z a ../qtcreator.7z *`
 8. Make new archive available for Travis machine