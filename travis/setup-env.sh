#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_DIR="$( cd "${DIR}"/.. && pwd )"
BUILD_OS_NAME=${TRAVIS_OS_NAME:-linux}
BUILD_DIR_REL="${PROJECT_DIR}/../build"
BUILD_DIR="$( mkdir -p ${BUILD_DIR_REL} && cd ${BUILD_DIR_REL} && pwd )"
DOWNLOAD_DIR_REL="${PROJECT_DIR}/../downloads"
DOWNLOAD_DIR="$( mkdir -p ${DOWNLOAD_DIR_REL} && cd ${DOWNLOAD_DIR_REL} && pwd )"
PLUGIN_OUT_DIR_REL="${PROJECT_DIR}/../plugins"
PLUGIN_OUT_DIR="$( mkdir -p cd ${PLUGIN_OUT_DIR_REL} && cd ${PLUGIN_OUT_DIR_REL} && pwd )"

echo "OS: ${BUILD_OS_NAME}"
echo "Project directory: ${PROJECT_DIR}"
echo "Build directory: ${BUILD_DIR}"
echo "Download directory: ${DOWNLOAD_DIR}"
echo "Output directory: ${PLUGIN_OUT_DIR}"
