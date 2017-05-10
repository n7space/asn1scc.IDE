#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source "${DIR}/setup-env.sh"

if [[ $CI == "true" && $BUILD_OS_NAME == 'linux' ]]; then
  . /opt/qt58/bin/qt58-env.sh
fi

echo "Building..."
cd "${BUILD_DIR}"
echo "Executing qmake..."
qmake "${PROJECT_DIR}"/asn1acn.pro -r ${ENV_QMAKE_PARAMS} \
      CONFIG+=release \
      QTC_SOURCE="${DOWNLOAD_DIR}/qt-creator-opensource-src-${ENV_QTC_VERSION}"\
      QTC_BUILD="${DOWNLOAD_DIR}/qtcbuild/${ENV_QTC_VERSION}"\
      OUTPUT_PATH="${PLUGIN_OUT_DIR}"\
      SET_VERSION_MINOR="${ENV_QTC_MV}"
echo "Executing make..."
make
