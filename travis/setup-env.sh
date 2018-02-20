#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_DIR="$( cd "${DIR}"/.. && pwd )"
BUILD_OS_NAME=${TRAVIS_OS_NAME:-linux}
BUILD_DIR_REL="${PROJECT_DIR}/../asn1scc.IDE-build"
BUILD_DIR="$( mkdir -p ${BUILD_DIR_REL} && cd ${BUILD_DIR_REL} && pwd )"
DOWNLOAD_DIR_REL="${PROJECT_DIR}/../downloads"
DOWNLOAD_DIR="$( mkdir -p ${DOWNLOAD_DIR_REL} && cd ${DOWNLOAD_DIR_REL} && pwd )"
QTC_BUILD_DIR_REL="${PROJECT_DIR}/../qtcbuild/"
QTC_BUILD_DIR="$( mkdir -p ${QTC_BUILD_DIR_REL} && cd ${QTC_BUILD_DIR_REL} && pwd )"
DIST_DIR_REL="${PROJECT_DIR}/../asn1scc.IDE-dist"
DIST_DIR="$( mkdir -p ${DIST_DIR_REL} && cd ${DIST_DIR_REL} && pwd )"
PLUGIN_NAME="Asn1Acn"

echo "OS: ${BUILD_OS_NAME}"
echo "Project directory: ${PROJECT_DIR}"
echo "Build directory: ${BUILD_DIR}"
echo "Download directory: ${DOWNLOAD_DIR}"
echo "QtCreator build directory: ${QTC_BUILD_DIR}"
