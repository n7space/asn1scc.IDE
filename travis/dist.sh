#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source "${DIR}/setup-env.sh"

if [[ $BUILD_OS_NAME == 'linux' ]]; then
    mkdir -p ${DIST_DIR}/lib/qtcreator/plugins
    cp ${QTC_BUILD_DIR}/lib/qtcreator/plugins/lib${PLUGIN_NAME}.so ${DIST_DIR}/lib/qtcreator/plugins
    strip ${DIST_DIR}/lib/qtcreator/plugins/lib${PLUGIN_NAME}.so
    RESOURCE_DIST_DIR=${DIST_DIR}/share/qtcreator
    OS_TAR_SUFFIX="linux-x64"
elif [[ $BUILD_OS_NAME == 'osx' ]]; then
    mkdir -p ${DIST_DIR}/Contents/PlugIns
    RESOURCE_DIST_DIR=${DIST_DIR}/Contents/Resources
    cp "${QTC_BUILD_DIR}/Qt Creator.app/Contents/PlugIns/lib${PLUGIN_NAME}.dylib" ${DIST_DIR}/Contents/PlugIns
    OS_TAR_SUFFIX="osx"
fi

mkdir -p ${RESOURCE_DIST_DIR}
cp -r ${PROJECT_DIR}/generic-highlighter ${RESOURCE_DIST_DIR}
cp -r ${PROJECT_DIR}/templates ${RESOURCE_DIST_DIR}
cp -r ${PROJECT_DIR}/snippets ${RESOURCE_DIST_DIR}

pushd ${DIST_DIR}
tar czf ${PROJECT_DIR}/${PLUGIN_NAME}-${VERSION}-${OS_TAR_SUFFIX}.tar.gz *
popd
