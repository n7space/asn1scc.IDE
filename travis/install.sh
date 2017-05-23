#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source "${DIR}/setup-env.sh"

if [[ "$BUILD_OS_NAME" == "osx" ]]; then
  brew update
  brew install p7zip qt5
  brew link --force qt5

  QT_BIN_LINK="http://download.qt.io/official_releases/qtcreator/${ENV_QTC_VERSION_SHORT}/${ENV_QTC_VERSION}/installer_source/mac_x64/qtcreator.7z"

elif [[ $CI == "true" && "$BUILD_OS_NAME" == "linux" ]]; then
    . /opt/qt58/bin/qt58-env.sh

    QT_BIN_LINK="http://download.qt.io/official_releases/qtcreator/${ENV_QTC_VERSION_SHORT}/${ENV_QTC_VERSION}/installer_source/linux_gcc_64_rhel72/qtcreator.7z"
fi

echo "Getting QtCreator's source..."
cd "${DOWNLOAD_DIR}"
wget "http://download.qt-project.org/official_releases/qtcreator/${ENV_QTC_VERSION_SHORT}/${ENV_QTC_VERSION}/qt-creator-opensource-src-${ENV_QTC_VERSION}.tar.gz"
tar xzf qt-creator-opensource-src-${ENV_QTC_VERSION}.tar.gz

if [ ! -f ${QTC_BUILD_DIR}/bin/qtcreator ]; then
    echo "Preparing QtCreator's build..."
    if [[ $ENV_WITH_TESTS == "1" ]]; then
        cd ${QTC_BUILD_DIR}
        qmake -r CONFIG+=release BUILD_TESTS=1 "${DOWNLOAD_DIR}/qt-creator-opensource-src-${ENV_QTC_VERSION}"
        make
        make clean
    else
        cd "${DOWNLOAD_DIR}"
        wget "${QT_BIN_LINK}"
        7zr x -o${QTC_BUILD_DIR} qtcreator.7z

        if [[ "$BUILD_OS_NAME" == "osx" ]]; then
            cd ${QTC_BUILD_DIR} && ln -s ./ bin
        fi
    fi
else
    echo "Using cached QtCreator's build"
fi
