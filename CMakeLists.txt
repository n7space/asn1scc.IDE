#############################################################################
##
## Copyright (C) 2017-2024 N7 Space sp. z o. o.
## Contact: https://n7space.com
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

cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_PREFIX_PATH "/opt/QT/Tools/QtCreator")

project(Asn1Acn)
set(QTC_PLUGIN_REVISION 6.0.2)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)

find_package(QtCreator COMPONENTS Core TextEditor ProjectExplorer REQUIRED)
find_package(Qt6 COMPONENTS Test REQUIRED)

option(WITH_TESTS "Build Tests" ON)
add_feature_info("Build tests" ${WITH_TESTS} "")

if (WITH_TESTS)
    set(IMPLICIT_DEPENDS Qt6::Test)
endif()

if (WITH_TESTS)
  enable_testing()
endif()

add_qtc_plugin(Asn1Acn
  PLUGIN_DEPENDS
    QtCreator::Core
    QtCreator::TextEditor
    QtCreator::ProjectExplorer
  DEPENDS
    QtCreator::Utils
  INCLUDES
    src
  SOURCES
    src/completion/autocompleter.cpp
    src/completion/asncompletionassist.cpp
    src/completion/asnsnippets.cpp
    src/completion/acnsnippets.cpp
    src/completion/proposalsbuilder.cpp
    src/completion/asnkeywordproposalsbuilder.cpp
    src/completion/usertypesproposalsbuilder.cpp
    src/completion/acnkeywordproposalsbuilder.cpp
    src/completion/completionassist.cpp
    src/completion/acncompletionassist.cpp
    src/completion/keywordproposalsbuilder.cpp
    src/completion/importfindingvisitor.cpp

    src/data/sourcelocation.cpp
    src/data/node.cpp
    src/data/root.cpp
    src/data/file.cpp
    src/data/project.cpp
    src/data/typeassignment.cpp
    src/data/valueassignment.cpp
    src/data/typereference.cpp
    src/data/definitions.cpp
    src/data/visitor.cpp

    src/data/types/type.cpp
    src/data/types/userdefinedtype.cpp
    src/data/types/labeltype.cpp
    src/data/types/builtintypes.cpp

    src/tree-views/outline-visitors/childrencountingvisitor.cpp
    src/tree-views/outline-visitors/childreturningvisitor.cpp
    src/tree-views/outline-visitors/indexfindingvisitor.cpp

    src/tree-views/combo-visitors/childrencountingvisitor.cpp
    src/tree-views/combo-visitors/childreturningvisitor.cpp
    src/tree-views/combo-visitors/indexfindingvisitor.cpp

    src/tree-views/typestree-visitors/parentreturningvisitor.cpp
    src/tree-views/typestree-visitors/childrencountingvisitor.cpp
    src/tree-views/typestree-visitors/childreturningvisitor.cpp
    src/tree-views/typestree-visitors/indexfindingvisitor.cpp

    src/tree-views/displayrolevisitor.cpp
    src/tree-views/decorationrolevisitor.cpp
    src/tree-views/mutablerootmodel.cpp
    src/tree-views/outlinemodel.cpp
    src/tree-views/typestreemodel.cpp
    src/tree-views/model.cpp
    src/tree-views/combomodel.cpp
    src/tree-views/treeviewwidget.cpp
    src/tree-views/activatehandler.cpp
    src/tree-views/indexupdater.cpp
    src/tree-views/outlineindexupdater.cpp
    src/tree-views/typestreeindexupdater.cpp
    src/tree-views/synchronizedindexupdater.cpp
    src/tree-views/unsynchronizedindexupdater.cpp
    src/tree-views/outlinecombo.cpp
    src/tree-views/outlinewidget.cpp
    src/tree-views/typestreewidget.cpp
    src/tree-views/expansionstaterestorer.cpp

    src/options-pages/service.cpp
    src/options-pages/libraries.cpp
    src/options-pages/fuzzer.cpp
    src/options-pages/servicewidget.cpp
    src/options-pages/librarieswidget.cpp
    src/options-pages/fuzzerwidget.cpp

    src/settings/settings.cpp
    src/settings/service.cpp
    src/settings/libraries.cpp
    src/settings/fuzzer.cpp

    src/fuzzer/fuzzerparamsdialog.cpp
    src/fuzzer/fuzzerparamswidget.cpp
    src/fuzzer/fuzzerrunner.cpp
    src/fuzzer/fuzzerparamsprovider.cpp

    src/libraries/modulemetadataparser.cpp
    src/libraries/generalmetadataparser.cpp
    src/libraries/componentimporter.cpp
    src/libraries/componentdirectorywatcher.cpp
    src/libraries/componentlibraryprocessor.cpp
    src/libraries/generalmetadataprocessor.cpp
    src/libraries/componentlibrarydispatcher.cpp
    src/libraries/librarystorage.cpp
    src/libraries/metadatamodel.cpp
    src/libraries/filemodel.cpp
    src/libraries/metadatacheckstatehandler.cpp

    src/libraries/wizard/importcomponentwizard.cpp
    src/libraries/wizard/selectsourcepage.cpp
    src/libraries/wizard/selectcomponentspage.cpp
    src/libraries/wizard/summarypage.cpp
    src/libraries/wizard/metadatacomponentselector.cpp
    src/libraries/wizard/filecomponentselector.cpp
    src/libraries/wizard/relationslabelscontroller.cpp
    src/libraries/wizard/vcshandler.cpp
    src/libraries/wizard/modulestreeview.cpp

    src/messages/messageutils.cpp
    src/messages/messagemanager.cpp

    src/icd/icdbuilder.cpp

    src/asn1acn.cpp
    src/asneditor.cpp
    src/asndocument.cpp
    src/acneditor.cpp
    src/acndocument.cpp
    src/editor.cpp
    src/asn1sccdocumentprocessor.cpp
    src/parseddatastorage.cpp
    src/astxmlparser.cpp
    src/projectwatcher.cpp
    src/document.cpp
    src/asn1acnjsextension.cpp
    src/asn1sccserviceprovider.cpp
    src/asn1sccparseddocumentbuilder.cpp
    src/projectcontenthandler.cpp
    src/indenter.cpp
    src/tools.cpp
    src/linkcreator.cpp
    src/referencefinder.cpp
    src/filesourcereader.cpp
    src/errormessageparser.cpp
    src/typeslocator.cpp
    src/modelvalidityguard.cpp
    src/usagesfinder.cpp
    src/toolsmenuimportitemcontroller.cpp
    src/projectmenuimportitemcontroller.cpp
    src/kitaspect.cpp
    src/kitaspectwidget.cpp
    src/selectionpositionresolver.cpp
    src/asn1acnerrorparser.cpp
    src/asn1acnstepscache.cpp
    src/asn1acnbuildstep.cpp
    src/asn1acnbuildsteprunner.cpp

    src/completion/autocompleter.h
    src/completion/asncompletionassist.h
    src/completion/asnsnippets.h
    src/completion/acnsnippets.h
    src/completion/proposalsbuilder.h
    src/completion/asnkeywordproposalsbuilder.h
    src/completion/usertypesproposalsbuilder.h
    src/completion/acnkeywordproposalsbuilder.h
    src/completion/completionassist.h
    src/completion/acncompletionassist.h
    src/completion/keywordproposalsbuilder.h
    src/completion/completiontypedefs.h
    src/completion/importfindingvisitor.h

    src/data/sourcelocation.h
    src/data/typeassignment.h
    src/data/valueassignment.h
    src/data/definitions.h
    src/data/typereference.h
    src/data/errormessage.h
    src/data/node.h
    src/data/root.h
    src/data/file.h
    src/data/project.h
    src/data/visitor.h
    src/data/visitorwithvalue.h
    src/data/importedtype.h
    src/data/importedvalue.h

    src/data/types/type.h
    src/data/types/userdefinedtype.h
    src/data/types/labeltype.h
    src/data/types/builtintypes.h

    src/tree-views/outline-visitors/childrencountingvisitor.h
    src/tree-views/outline-visitors/childreturningvisitor.h
    src/tree-views/outline-visitors/indexfindingvisitor.h

    src/tree-views/combo-visitors/childrencountingvisitor.h
    src/tree-views/combo-visitors/childreturningvisitor.h
    src/tree-views/combo-visitors/indexfindingvisitor.h

    src/tree-views/typestree-visitors/parentreturningvisitor.h
    src/tree-views/typestree-visitors/childrencountingvisitor.h
    src/tree-views/typestree-visitors/childreturningvisitor.h
    src/tree-views/typestree-visitors/indexfindingvisitor.h

    src/tree-views/displayrolevisitor.h
    src/tree-views/decorationrolevisitor.h
    src/tree-views/mutablerootmodel.h
    src/tree-views/outlinemodel.h
    src/tree-views/typestreemodel.h
    src/tree-views/typestreeindexupdater.h
    src/tree-views/model.h
    src/tree-views/combomodel.h
    src/tree-views/treeviewwidget.h
    src/tree-views/activatehandler.h
    src/tree-views/indexupdater.h
    src/tree-views/outlineindexupdater.h
    src/tree-views/unsynchronizedindexupdater.h
    src/tree-views/synchronizedindexupdater.h
    src/tree-views/outlinecombo.h
    src/tree-views/outlinewidget.h
    src/tree-views/typestreewidget.h
    src/tree-views/expansionstaterestorer.h
    src/tree-views/indexupdaterfactory.h
    src/tree-views/typestreeindexupdaterfactory.h
    src/tree-views/outlineindexupdaterfactory.h

    src/options-pages/service.h
    src/options-pages/libraries.h
    src/options-pages/fuzzer.h
    src/options-pages/servicewidget.h
    src/options-pages/librarieswidget.h
    src/options-pages/fuzzerwidget.h

    src/settings/settings.h
    src/settings/service.h
    src/settings/libraries.h
    src/settings/fuzzer.h

    src/fuzzer/fuzzerparamsdialog.h
    src/fuzzer/fuzzerparamswidget.h
    src/fuzzer/fuzzerrunner.h
    src/fuzzer/fuzzerparamsprovider.h

    src/libraries/modulemetadataparser.h
    src/libraries/generalmetadataparser.h
    src/libraries/componentimporter.h
    src/libraries/componentdirectorywatcher.h
    src/libraries/componentlibraryprocessor.h
    src/libraries/generalmetadataprocessor.h
    src/libraries/componentlibrarydispatcher.h
    src/libraries/librarystorage.h
    src/libraries/metadatamodel.h
    src/libraries/filemodel.h
    src/libraries/metadatacheckstatehandler.h

    src/libraries/wizard/importcomponentwizard.h
    src/libraries/wizard/selectsourcepage.h
    src/libraries/wizard/selectcomponentspage.h
    src/libraries/wizard/summarypage.h
    src/libraries/wizard/metadatacomponentselector.h
    src/libraries/wizard/componentselector.h
    src/libraries/wizard/filecomponentselector.h
    src/libraries/wizard/relationslabelscontroller.h
    src/libraries/wizard/vcshandler.h
    src/libraries/wizard/modulestreeview.h

    src/libraries/metadata/element.h
    src/libraries/metadata/reference.h
    src/libraries/metadata/submodule.h
    src/libraries/metadata/module.h
    src/libraries/metadata/library.h
    src/libraries/metadata/general.h
    src/libraries/metadata/librarynode.h

    src/messages/messageutils.h
    src/messages/messagemanager.h

    src/icd/icdbuilder.h

    src/asn1acn_global.h
    src/asn1acnconstants.h
    src/asn1acn.h
    src/asneditor.h
    src/asndocument.h
    src/acneditor.h
    src/acndocument.h
    src/editor.h
    src/asn1sccdocumentprocessor.h
    src/parseddatastorage.h
    src/astxmlparser.h
    src/projectwatcher.h
    src/document.h
    src/asn1acnjsextension.h
    src/asn1sccserviceprovider.h
    src/asn1sccparseddocumentbuilder.h
    src/projectcontenthandler.h
    src/indenter.h
    src/tr.h
    src/linkcreator.h
    src/referencefinder.h
    src/tools.h
    src/parsingserviceprovider.h
    src/parseddocumentbuilder.h
    src/documentprocessor.h
    src/sourcereader.h
    src/filesourcereader.h
    src/errormessageparser.h
    src/typeslocator.h
    src/modelvalidityguard.h
    src/usagesfinder.h
    src/toolsmenuimportitemcontroller.h
    src/projectmenuimportitemcontroller.h
    src/kitaspect.h
    src/kitaspectwidget.h
    src/selectionpositionresolver.h
    src/asn1acnerrorparser.h
    src/asn1acnstepscache.h
    src/asn1acnbuildstep.h
    src/asn1acnbuildsteprunner.h

    .github/workflows/build_cmake.yml
    .github/workflows/README.md

    asn1acn.qrc
    LICENSE
    README.md

    src/fuzzer/fuzzerparams.ui

    src/options-pages/service.ui
    src/options-pages/libraries.ui
    src/options-pages/fuzzer.ui

    src/libraries/wizard/import_component.ui
    src/libraries/wizard/select_component.ui
    src/libraries/wizard/summary.ui

    schemas/asn1-lib-general-schema.json
    schemas/asn1-lib-module-schema.json

    snippets/asn1.xml
    snippets/acn.xml
    generic-highlighter/syntax/asn1.xml
    generic-highlighter/syntax/acn.xml
    templates/wizards/files/acn/wizard.json
    templates/wizards/files/acn/template.acn
    templates/wizards/files/asn1/wizard.json
    templates/wizards/files/asn1/template.asn
    templates/wizards/files/asn1acn/wizard.json
    templates/wizards/files/asn1acn/template.asn
    templates/wizards/files/asn1acn/template.acn
    templates/wizards/projects/asn1acn/wizard.json
    templates/wizards/projects/asn1acn/template.asn
    templates/wizards/projects/asn1acn/template.acn
    templates/wizards/projects/asn1acn/file.pro
    templates/wizards/projects/asn1acn/generateFromAsn1.pri
    templates/wizards/projects/asn1acn/handleAsn1AcnBuild.pri
    templates/wizards/projects/asn1acn/updateSourcesList.pri
    templates/wizards/projects/asn1acn/CMakeLists.txt
    templates/wizards/projects/asn1acn/asn1sccSettings.cmake
    templates/wizards/projects/asn1acn/configureAsn1SccTarget.cmake
    templates/wizards/projects/asn1acn/createFilesList.cmake
)

extend_qtc_plugin(Asn1Acn
  CONDITION WITH_TESTS
  SOURCES
    src/libraries/tests/modulemetadataparser_tests.cpp
    src/libraries/tests/generalmetadataparser_tests.cpp
    src/libraries/tests/metadatamodel_tests.cpp
    src/libraries/tests/filemodel_tests.cpp
    src/libraries/tests/metadatacheckstatehandler_tests.cpp

    src/3rdparty/tests/modeltest.cpp

    src/tree-views/tests/outlinemodel_tests.cpp
    src/tree-views/tests/combomodel_tests.cpp
    src/tree-views/tests/typestreemodel_tests.cpp
    src/tree-views/tests/displayrolevisitor_tests.cpp
    src/tree-views/tests/outlineindexupdater_tests.cpp

    src/tests/astxmlparser_tests.cpp
    src/tests/errormessageparser_tests.cpp
    src/tests/parseddocumentbuilder_tests.cpp
    src/tests/documentprocessor_tests.cpp
    src/tests/projectcontenthandler_tests.cpp
    src/tests/parseddatastorage_tests.cpp
    src/tests/autocompleter_tests.cpp
    src/tests/modelvalidityguard_tests.cpp
    src/tests/linkcreator_tests.cpp
    src/tests/indenter_tests.cpp
    src/tests/selectionpositionresolver_test.cpp

    src/tests/networkreply.cpp
    src/tests/parsingserviceproviderstub.cpp
    src/tests/parseddocumentbuilderstub.cpp
    src/tests/documentprocessorstub.cpp
    src/tests/sourcereadermock.cpp

    src/libraries/tests/modulemetadataparser_tests.h
    src/libraries/tests/generalmetadataparser_tests.h
    src/libraries/tests/metadatamodel_tests.h
    src/libraries/tests/filemodel_tests.h
    src/libraries/tests/metadatacheckstatehandler_tests.h

    src/tree-views/tests/outlinemodel_tests.h
    src/tree-views/tests/combomodel_tests.h
    src/tree-views/tests/typestreemodel_tests.h
    src/tree-views/tests/displayrolevisitor_tests.h
    src/tree-views/tests/outlineindexupdater_tests.h

    src/3rdparty/tests/modeltest.h

    src/tests/astxmlparser_tests.h
    src/tests/errormessageparser_tests.h
    src/tests/parseddocumentbuilder_tests.h
    src/tests/documentprocessor_tests.h
    src/tests/projectcontenthandler_tests.h
    src/tests/parseddatastorage_tests.h
    src/tests/autocompleter_tests.h
    src/tests/modelvalidityguard_tests.h
    src/tests/linkcreator_tests.h
    src/tests/indenter_tests.h
    src/tests/selectionpositionresolver_test.h

    src/tests/networkreply.h
    src/tests/parsingserviceproviderstub.h
    src/tests/parseddocumentbuilderstub.h
    src/tests/documentprocessorstub.h
    src/tests/sourcereadermock.h
)
