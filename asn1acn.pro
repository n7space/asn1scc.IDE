#############################################################################
##
## Copyright (C) 2017 N7 Mobile sp. z o. o.
## Contact: http://n7mobile.pl/Space
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
isEmpty(IDE_SOURCE_TREE): IDE_SOURCE_TREE = "/opt/qt-creator-dev/qt-creator"

IDE_BUILD_TREE = $$QTC_BUILD
isEmpty(IDE_BUILD_TREE): IDE_BUILD_TREE = "/opt/qt-creator-dev/build-debug"

QTC_PLUGIN_NAME = Asn1Acn
QTC_LIB_DEPENDS += \
    utils

QTC_PLUGIN_DEPENDS += \
    texteditor \
    coreplugin \
    projectexplorer

QTC_PLUGIN_RECOMMENDS += \
    # optional plugin dependencies. nothing here at this time

include($$IDE_SOURCE_TREE/src/qtcreatorplugin.pri)

### Plugin ###

CONFIG += object_parallel_to_source

DEFINES += ASN1ACN_LIBRARY
INCLUDEPATH += $$PWD

# asn1acn plugin files

SOURCES += \
    completion/autocompleter.cpp \
    completion/asncompletionassist.cpp \
    completion/asnsnippetprovider.cpp \
    completion/acnsnippetprovider.cpp \
    completion/proposalsprovider.cpp \
    completion/asnbuiltinsproposalsprovider.cpp \
    completion/usertypesproposalsprovider.cpp \
    completion/acnbuiltinsproposalsprovider.cpp \
    completion/completionassist.cpp \
    completion/acncompletionassist.cpp \
    completion/builtinsproposalsprovider.cpp \
    \
    data/node.cpp \
    data/root.cpp \
    data/file.cpp \
    data/project.cpp \
    data/typeassignment.cpp \
    data/typereference.cpp \
    data/definitions.cpp \
    data/visitor.cpp \
    data/source.cpp \
    \
    tree-views/outline-visitors/childrencountingvisitor.cpp \
    tree-views/outline-visitors/childreturningvisitor.cpp \
    tree-views/outline-visitors/indexfindingvisitor.cpp \
    \
    tree-views/typestree-visitors/parentreturningvisitor.cpp \
    tree-views/typestree-visitors/childrencountingvisitor.cpp \
    tree-views/typestree-visitors/childreturningvisitor.cpp \
    tree-views/typestree-visitors/indexfindingvisitor.cpp \
    \
    tree-views/displayrolevisitor.cpp \
    tree-views/decorationrolevisitor.cpp \
    tree-views/outlinemodel.cpp \
    tree-views/typestreemodel.cpp \
    tree-views/model.cpp \
    tree-views/treeviewwidget.cpp \
    tree-views/activatehandler.cpp \
    tree-views/indexupdater.cpp \
    tree-views/outlineindexupdater.cpp \
    tree-views/typestreeindexupdater.cpp \
    tree-views/outlinecombo.cpp \
    tree-views/outlinewidget.cpp \
    tree-views/typestreewidget.cpp \
    \
    options-pages/general.cpp \
    options-pages/service.cpp \
    options-pages/servicewidget.cpp \
    options-pages/generalwidget.cpp \
    \
    settings/settings.cpp \
    settings/general.cpp \
    settings/service.cpp \
    \
    libraries/metadataparser.cpp \
    \
    asn1acn.cpp \
    asneditor.cpp \
    asndocument.cpp \
    acneditor.cpp \
    acndocument.cpp \
    editor.cpp \
    overviewmodel.cpp \
    asn1sccdocumentprocessor.cpp \
    parseddatastorage.cpp \
    parseddocument.cpp \
    astxmlparser.cpp \
    modeltreenode.cpp \
    projectwatcher.cpp \
    overviewwidget.cpp \
    document.cpp \
    modeltree.cpp \
    asn1acnjsextension.cpp \
    asn1sccserviceprovider.cpp \
    asn1sccparseddocumentbuilder.cpp \
    projectcontenthandler.cpp \
    indenter.cpp \
    tools.cpp \
    icons.cpp \
    linkcreator.cpp \
    overviewindexupdater.cpp \
    structuresviewindexupdater.cpp \
    filesourcereader.cpp \
    errormessageparser.cpp \
    sourcemapper.cpp \
    typeslocator.cpp \
    editoroutline.cpp \
    typereferencesholder.cpp

HEADERS += \
    completion/autocompleter.h \
    completion/asncompletionassist.h \
    completion/asnsnippetprovider.h \
    completion/acnsnippetprovider.h \
    completion/proposalsprovider.h \
    completion/asnbuiltinsproposalsprovider.h \
    completion/usertypesproposalsprovider.h \
    completion/acnbuiltinsproposalsprovider.h \
    completion/completionassist.h \
    completion/acncompletionassist.h \
    completion/builtinsproposalsprovider.h \
    completion/completiontypedefs.h \
    \
    data/sourcelocation.h \
    data/typeassignment.h \
    data/definitions.h \
    data/typereference.h \
    data/type.h \
    data/errormessage.h \
    data/node.h \
    data/root.h \
    data/file.h \
    data/project.h \
    data/visitor.h \
    data/visitorwithvalue.h \
    data/source.h \
    \
    tree-views/outline-visitors/childrencountingvisitor.h \
    tree-views/outline-visitors/childreturningvisitor.h \
    tree-views/outline-visitors/indexfindingvisitor.h \
    \
    tree-views/typestree-visitors/parentreturningvisitor.h \
    tree-views/typestree-visitors/childrencountingvisitor.h \
    tree-views/typestree-visitors/childreturningvisitor.h \
    tree-views/typestree-visitors/indexfindingvisitor.h \
    \
    tree-views/displayrolevisitor.h \
    tree-views/decorationrolevisitor.h \   
    tree-views/outlinemodel.h \
    tree-views/typestreemodel.h \
    tree-views/typestreeindexupdater.h \
    tree-views/model.h \
    tree-views/treeviewwidget.h \
    tree-views/activatehandler.h \
    tree-views/indexupdater.h \
    tree-views/outlineindexupdater.h \
    tree-views/outlinecombo.h \
    tree-views/outlinewidget.h \
    tree-views/typestreewidget.h \
    \
    options-pages/general.h \
    options-pages/service.h \
    options-pages/servicewidget.h \
    options-pages/generalwidget.h \
    \
    settings/settings.h\
    settings/general.h \
    settings/service.h \
    \
    libraries/metadataparser.h \
    \
    libraries/metadata/element.h \
    libraries/metadata/import.h \
    libraries/metadata/submodule.h \
    libraries/metadata/module.h \
    \
    asn1acn_global.h \
    asn1acnconstants.h \
    asn1acn.h \
    asneditor.h \
    asndocument.h \
    acneditor.h \
    acndocument.h \
    editor.h \
    overviewmodel.h \
    asn1sccdocumentprocessor.h \
    parseddatastorage.h \
    parseddocument.h \
    astxmlparser.h \
    modeltreenode.h \
    projectwatcher.h \
    overviewwidget.h \
    document.h \
    modeltree.h \
    asn1acnjsextension.h \
    asn1sccserviceprovider.h \
    asn1sccparseddocumentbuilder.h \
    projectcontenthandler.h \
    indenter.h \
    tr.h \
    linkcreator.h \
    tools.h \
    icons.h \
    overviewindexupdater.h \
    structuresviewindexupdater.h \
    parsingserviceprovider.h \
    parseddocumentbuilder.h \
    documentprocessor.h \
    sourcereader.h \
    filesourcereader.h \
    parseddatastorageproxy.h \
    modeltreeproxy.h \
    errormessageparser.h \
    editoroutline.h \
    sourcemapper.h \
    overviewactivatehandler.h \
    typeslocator.h \
    typereferencesholder.h

FORMS += \
    options-pages/general.ui \
    options-pages/service.ui

RESOURCES += \
    asn1acn.qrc

DISTFILES += \
    LICENSE \
    README.md

# test files

equals(TEST, 1) {

SOURCES += \
    libraries/tests/metadataparser_tests.cpp \
    \
    tree-views/tests/3rdparty/modeltest.cpp \
    tree-views/tests/outlinemodel_tests.cpp \
    tree-views/tests/typestreemodel_tests.cpp \
    tree-views/tests/displayrolevisitor_tests.cpp \
    tree-views/tests/outlineindexupdater_tests.cpp \
    \
    tests/astxmlparser_tests.cpp \
    tests/errormessageparser_tests.cpp \
    tests/overviewindexupdater_tests.cpp \
    tests/structuresviewindexupdater_tests.cpp \
    tests/parseddocumentbuilder_tests.cpp \
    tests/documentprocessor_tests.cpp \
    tests/projectcontenthandler_tests.cpp \
    tests/parseddatastorage_tests.cpp \
    tests/modeltree_tests.cpp \
    tests/autocompleter_tests.cpp \
    tests/sourcemapper_tests.cpp \
    \
    tests/networkreply.cpp \
    tests/parsingserviceproviderstub.cpp \
    tests/parseddocumentbuilderstub.cpp \
    tests/documentprocessorstub.cpp \
    tests/sourcereadermock.cpp

HEADERS += \
    libraries/tests/metadataparser_tests.h \
    \
    tree-views/tests/3rdparty/modeltest.h \
    tree-views/tests/outlinemodel_tests.h \
    tree-views/tests/typestreemodel_tests.h \
    tree-views/tests/displayrolevisitor_tests.h \
    tree-views/tests/outlineindexupdater_tests.h \
    \
    tests/astxmlparser_tests.h \
    tests/errormessageparser_tests.h \
    tests/overviewindexupdater_tests.h \
    tests/structuresviewindexupdater_tests.h \
    tests/parseddocumentbuilder_tests.h \
    tests/documentprocessor_tests.h \
    tests/projectcontenthandler_tests.h \
    tests/parseddatastorage_tests.h \
    tests/modeltree_tests.h \
    tests/autocompleter_tests.h \
    tests/sourcemapper_tests.h \
    \
    tests/networkreply.h \
    tests/parsingserviceproviderstub.h \
    tests/parseddocumentbuilderstub.h \
    tests/documentprocessorstub.h \
    tests/sourcereadermock.h
}

### Static files ###

STATIC_FILES += \
    snippets/asn1.xml \
    snippets/acn.xml \
    generic-highlighter/asn1.xml \
    generic-highlighter/acn.xml \
    templates/wizards/files/acn/wizard.json \
    templates/wizards/files/acn/template.acn \
    templates/wizards/files/asn1/wizard.json \
    templates/wizards/files/asn1/template.asn \
    templates/wizards/files/asn1acn/wizard.json \
    templates/wizards/files/asn1acn/template.asn \
    templates/wizards/files/asn1acn/template.acn \
    templates/wizards/projects/asn1acn/wizard.json \
    templates/wizards/projects/asn1acn/template.asn \
    templates/wizards/projects/asn1acn/template.acn \
    templates/wizards/projects/asn1acn/file.pro \
    templates/wizards/projects/asn1acn/createSources.pri \
    templates/wizards/projects/asn1acn/handleAsn1AcnBuild.pri \
    templates/wizards/projects/asn1acn/createSourcesList.pri \
    templates/wizards/projects/asn1acn/CMakeLists.txt \
    templates/wizards/projects/asn1acn/asn1sccSettings.cmake \
    templates/wizards/projects/asn1acn/configureAsn1SccTarget.cmake \
    templates/wizards/projects/asn1acn/createFilesList.cmake

STATIC_BASE = $$PWD
STATIC_OUTPUT_BASE = $$IDE_DATA_PATH
STATIC_INSTALL_BASE = $$INSTALL_DATA_PATH

include($$IDE_SOURCE_TREE/qtcreatordata.pri)
