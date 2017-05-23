#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source "${DIR}/setup-env.sh"

if [[ $CI == "true" && $BUILD_OS_NAME == 'linux' ]]; then
    . /opt/qt58/bin/qt58-env.sh
    QMAKE_PARAMS="-spec linux-g++"
fi

echo "Building..."
cd "${BUILD_DIR}"
echo "Executing qmake..."
set -x
qmake "${PROJECT_DIR}"/asn1acn.pro -r ${QMAKE_PARAMS} \
      CONFIG+=release \
      BUILD_TESTS="${ENV_WITH_TESTS}" \
      QTC_SOURCE="${DOWNLOAD_DIR}/qt-creator-opensource-src-${ENV_QTC_VERSION}" \
      QTC_BUILD="${DOWNLOAD_DIR}/qtcbuild/${ENV_QTC_VERSION}"
set +x
echo "Executing make..."
make
