#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_DIR="$( cd "${DIR}"/.. && pwd )"
BUILD_OS_NAME=${TRAVIS_OS_NAME:-linux}
: ${BUILD_DIR:="$(test -n "${TRAVIS_BUILD_DIR}" && echo "${TRAVIS_BUILD_DIR}"/build || echo "${DIR}"/build)"}
: ${DOWNLOAD_DIR:="${DIR}/downloads/"}
: ${PLUGIN_OUT_PATH:="${BUILD_DIR}/plugins/${ENV_QTC_VERSION}/"}

echo "Project directory: ${PROJECT_DIR}"
echo "OS: ${BUILD_OS_NAME}"
echo "Build directory: ${BUILD_DIR}"
echo "Download directory: ${DOWNLOAD_DIR}"
echo "Output directory: ${PLUGIN_OUT_PATH}"

