#############################################################################
##
## Copyright (C) 2017 N7 Mobile sp. z o. o.
## Contact: http://n7mobile.com/Space
##
## This file is part of ASN.1/ACN Plugin for QtCreator.
##
## Plugin was developed under a programme and funded by
## European Space Agency.
##
## This Plugin is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This Plugin is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.
##
#############################################################################

# Qt Creator linking

IDE_SOURCE_TREE = $$QTC_SOURCE
isEmpty(IDE_SOURCE_TREE): IDE_SOURCE_TREE = "/opt/qt-creator-dev/src"

IDE_BUILD_TREE = $$QTC_BUILD
isEmpty(IDE_BUILD_TREE): IDE_BUILD_TREE = "/opt/qt-creator-dev/build-debug"

QTC_PLUGIN_NAME = Asn1Acn
QTC_LIB_DEPENDS += \
    utils

QTC_PLUGIN_DEPENDS += \
    texteditor \
    coreplugin

QTC_PLUGIN_RECOMMENDS += \
    # optional plugin dependencies. nothing here at this time

include($$IDE_SOURCE_TREE/src/qtcreatorplugin.pri)

### Plugin ###

DEFINES += ASN1ACN_LIBRARY

# asn1acn plugin files

SOURCES += \
    asn1acn.cpp \
    asneditor.cpp \
    asndocument.cpp \
    asnautocompleter.cpp \
    asncompletionassist.cpp \
    asnoutline.cpp \
    asnhighlighter.cpp \
    asnsnippetprovider.cpp \
    asnoverviewmodel.cpp \
    asndocumentprocessor.cpp \
    asnparseddatastorage.cpp \
    asnparseddocument.cpp

HEADERS += \
    asn1acn_global.h \
    asn1acnconstants.h \
    asn1acn.h \
    asneditor.h \
    asndocument.h \
    asnautocompleter.h \
    asncompletionassist.h \
    asnoutline.h \
    asnhighlighter.h \
    asnsnippetprovider.h \
    asnoverviewmodel.h \
    asndocumentprocessor.h \
    asnparseddatastorage.h \
    asnparseddocument.h

DISTFILES += \
    LICENSE \
    README.md

RESOURCES += \
    asn1acn.qrc

# test files

equals(TEST, 1) {

SOURCES += \
    tests/sanity_test.cpp

HEADERS += \
    tests/sanity_test.h

}

### Static files ###

STATIC_FILES += snippets/asn.xml
STATIC_BASE = $$PWD
STATIC_OUTPUT_BASE = $$IDE_DATA_PATH
STATIC_INSTALL_BASE = $$INSTALL_DATA_PATH

include($$IDE_SOURCE_TREE/qtcreatordata.pri)
