#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source "${DIR}/setup-env.sh"

if [[ $CI == "true" && $BUILD_OS_NAME == 'linux' ]]; then
    . /opt/qt58/bin/qt58-env.sh
    QMAKE_PARAMS="-spec linux-g++"
fi

echo "Making clean copy of QtCreator binaries"
cp -fr "${QTC_BUILD_DIR}" "${DOWNLOAD_DIR}/qtcbuild"

echo "Building..."
cd "${BUILD_DIR}"
echo "Executing qmake..."
set -x
qmake "${PROJECT_DIR}"/asn1acn.pro -r ${QMAKE_PARAMS} \
      CONFIG+=release \
      BUILD_TESTS="${ENV_WITH_TESTS}" \
      QTC_SOURCE="${DOWNLOAD_DIR}/qt-creator-opensource-src-${ENV_QTC_VERSION}" \
      QTC_BUILD="${DOWNLOAD_DIR}/qtcbuild"
set +x
echo "Executing make..."
make

if [[ $ENV_WITH_TESTS == "1" ]]; then
    echo "Starting xvfb..."
    export DISPLAY=:99.0
    sh -e /etc/init.d/xvfb start
    sleep 3
    echo "Executing tests..."
    ${DOWNLOAD_DIR}/qtcbuild/${ENV_QTC_VERSION}/bin/qtcreator -test "ASN.1/ACN"
fi
