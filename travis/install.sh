#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source "${DIR}/setup-env.sh"

if [[ "$BUILD_OS_NAME" == "osx" ]]; then
  brew update
  brew install p7zip qt5
  brew link --force qt5

  QT_BIN_LINK="http://download.qt.io/official_releases/qtcreator/${ENV_QTC_VERSION_SHORT}/${ENV_QTC_VERSION}/installer_source/mac_x64/qtcreator.7z"

elif [[ $CI == "true" && "$BUILD_OS_NAME" == "linux" ]]; then
    if [[ $ENV_WITH_TESTS == "1" ]]; then
        QT_BIN_LINK="https://github.com/n7mobile/3rdparty-deps/releases/download/qt-creator-${ENV_QTC_VERSION}-with-tests/qt-creator-linux-x86_64-${ENV_QTC_VERSION}.7z"
    else
        QT_BIN_LINK="http://download.qt.io/official_releases/qtcreator/${ENV_QTC_VERSION_SHORT}/${ENV_QTC_VERSION}/installer_source/linux_gcc_64_rhel72/qtcreator.7z"
    fi
    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-6 100 --slave /usr/bin/g++ g++ /usr/bin/g++-6
fi

echo "Getting QtCreator's source..."
cd "${DOWNLOAD_DIR}"
wget "http://download.qt-project.org/official_releases/qtcreator/${ENV_QTC_VERSION_SHORT}/${ENV_QTC_VERSION}/qt-creator-opensource-src-${ENV_QTC_VERSION}.tar.gz"
tar xzf qt-creator-opensource-src-${ENV_QTC_VERSION}.tar.gz

echo "Getting QtCreator's binaries..."
cd "${DOWNLOAD_DIR}"
wget "${QT_BIN_LINK}" -O qtcreator.7z
7zr x -o${QTC_BUILD_DIR} qtcreator.7z

if [[ "$BUILD_OS_NAME" == "osx" ]]; then
    cd ${QTC_BUILD_DIR} && ln -s ./ bin
fi
