#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source "${DIR}/setup-env.sh"

if [[ $CI == "true" && $BUILD_OS_NAME == 'linux' ]]; then
  . /opt/qt58/bin/qt58-env.sh
fi

echo "Creating build directory ${BUILD_DIR}"
mkdir -p "${BUILD_DIR}" && cd "${BUILD_DIR}"
mkdir -p "${PLUGIN_OUT_PATH}"

echo "Building..."
echo "Executing qmake..."
qmake "${PROJECT_DIR}"/asn1acn.pro -r ${ENV_QMAKE_PARAMS} CONFIG+=release \
      QTC_SOURCE="${ENV_QTC_SOURCE}"\
      QTC_BUILD="${ENV_QTC_BUILD}"\
      OUTPUT_PATH="${PLUGIN_OUT_PATH}"\
      SET_VERSION_MINOR="${ENV_QTC_MV}"
echo "Executing make..."
make
