#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source "${DIR}/setup-env.sh"

if [[ $CI == "true" && $BUILD_OS_NAME == 'linux' ]]; then
    . /opt/qt510/bin/qt510-env.sh
    QMAKE_PARAMS="-spec linux-g++"
fi

if [[ $ENV_WITH_TESTS == "1" ]]; then
    BUILD_TESTS="BUILD_TESTS=1"
fi

echo "Building..."
cd "${BUILD_DIR}"
$CXX --version
echo "Executing qmake..."
set -x
qmake "${PROJECT_DIR}"/asn1acn.pro -r ${QMAKE_PARAMS} \
      CONFIG+=release \
      ${BUILD_TESTS} \
      QTC_SOURCE="${DOWNLOAD_DIR}/qt-creator-opensource-src-${ENV_QTC_VERSION}" \
      QTC_BUILD="${QTC_BUILD_DIR}" || exit 1
set +x
echo "Executing make..."
make || exit 1

if [[ $ENV_WITH_TESTS == "1" && $BUILD_OS_NAME == 'linux' ]]; then
    echo "Starting xvfb..."
    export DISPLAY=:99.0
    sh -e /etc/init.d/xvfb start
    sleep 3
    echo "Executing tests..."
    ${QTC_BUILD_DIR}/bin/qtcreator -test "ASN.1/ACN" || exit 1
fi
