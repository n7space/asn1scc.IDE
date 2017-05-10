#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source "${DIR}/setup-env.sh"

if [[ "$BUILD_OS_NAME" == "osx" ]]; then
  brew update
  brew install p7zip

  QT_BIN_LINK="http://download.qt.io/official_releases/qtcreator/4.2/4.2.2/installer_source/mac_x64/qtcreator.7z"

elif [[ $CI == "true" && "$BUILD_OS_NAME" == "linux" ]]; then
  QT_BIN_LINK="http://download.qt.io/official_releases/qtcreator/4.2/4.2.2/installer_source/linux_gcc_64_rhel72/qtcreator.7z"

fi

mkdir -p "${DOWNLOAD_DIR}" && cd "${DOWNLOAD_DIR}"
wget "http://download.qt-project.org/official_releases/qtcreator/4.2/4.2.2/qt-creator-opensource-src-4.2.2.tar.gz"
tar xzf qt-creator-opensource-src-4.2.2.tar.gz
wget "${QT_BIN_LINK}"
7zr x -oqtcbuild/4.2.2/ qtcreator.7z

if [[ "$BUILD_OS_NAME" == "osx" ]]; then
  cd qtcbuild/4.2.2/ && ln -s ./ bin
fi
