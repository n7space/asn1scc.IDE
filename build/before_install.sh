#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source "${DIR}/setup-env.sh"

if [[ "$BUILD_OS_NAME" == "osx" ]]; then
  brew update
  brew install p7zip

  QT_BIN_LINK="http://download.qt.io/official_releases/qtcreator/${ENV_QTC_VERSION_SHORT}/${ENV_QTC_VERSION}/installer_source/mac_x64/qtcreator.7z"

elif [[ $CI == "true" && "$BUILD_OS_NAME" == "linux" ]]; then
  QT_BIN_LINK="http://download.qt.io/official_releases/qtcreator/${ENV_QTC_VERSION_SHORT}/${ENV_QTC_VERSION}/installer_source/linux_gcc_64_rhel72/qtcreator.7z"

fi

mkdir -p "${DOWNLOAD_DIR}" && cd "${DOWNLOAD_DIR}"
wget "http://download.qt-project.org/official_releases/qtcreator/${ENV_QTC_VERSION_SHORT}/${ENV_QTC_VERSION}/qt-creator-opensource-src-${ENV_QTC_VERSION}.tar.gz"
tar xzf qt-creator-opensource-src-${ENV_QTC_VERSION}.tar.gz
wget "${QT_BIN_LINK}"
7zr x -oqtcbuild/${ENV_QTC_VERSION} qtcreator.7z

if [[ "$BUILD_OS_NAME" == "osx" ]]; then
  cd qtcbuild/${ENV_QTC_VERSION} && ln -s ./ bin
fi
