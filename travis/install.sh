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
        QT_BIN_LINK="https://n7space-my.sharepoint.com/:u:/p/kgrochowski/EejPG4wmUcpPu8M0dedOglkBiaiEG1qUlpMlSqdG-oXzHw?download=1"
    else
        QT_BIN_LINK="http://download.qt.io/official_releases/qtcreator/${ENV_QTC_VERSION_SHORT}/${ENV_QTC_VERSION}/installer_source/linux_gcc_64_rhel72/qtcreator.7z"
    fi
fi

echo "Getting QtCreator's source..."
cd "${DOWNLOAD_DIR}"
wget "http://download.qt-project.org/official_releases/qtcreator/${ENV_QTC_VERSION_SHORT}/${ENV_QTC_VERSION}/qt-creator-opensource-src-${ENV_QTC_VERSION}.tar.gz"
tar xzf qt-creator-opensource-src-${ENV_QTC_VERSION}.tar.gz || exit 1

echo "Getting QtCreator's binaries..."
cd "${DOWNLOAD_DIR}"
wget "${QT_BIN_LINK}" -O qtcreator.7z
7zr x -o${QTC_BUILD_DIR} qtcreator.7z || exit 1

if [[ "$BUILD_OS_NAME" == "osx" ]]; then
    cd ${QTC_BUILD_DIR} && ln -s ./ bin
fi
