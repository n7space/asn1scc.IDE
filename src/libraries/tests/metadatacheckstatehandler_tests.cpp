/****************************************************************************
**
** Copyright (C) 2017 N7 Mobile sp. z o. o.
** Contact: http://n7mobile.pl/Space
**
** This file is part of ASN.1/ACN Plugin for QtCreator.
**
** Plugin was developed under a programme and funded by
** European Space Agency.
**
** This Plugin is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This Plugin is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "metadatacheckstatehandler_tests.h"

#include <memory>

#include <QtTest/QtTest>

#include <libraries/metadata/library.h>
#include <libraries/metadata/module.h>
#include <libraries/metadata/submodule.h>
#include <libraries/metadata/element.h>

#include <libraries/metadatamodel.h>
#include <libraries/metadatacheckstatehandler.h>

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::Libraries;
using namespace Asn1Acn::Internal::Libraries::Tests;

namespace {
std::unique_ptr<Metadata::Submodule> createSubmodule(const int elementsCnt, const QString &name)
{
    auto submodule = std::make_unique<Metadata::Submodule>(name, name);
    for (int i = 0; i < elementsCnt; i++) {
        const QString elementName = name + QString("_element") + QString::number(i);
        submodule->addElement(std::make_unique<Metadata::Element>(elementName, elementName));
    }

    return submodule;
}

std::unique_ptr<Metadata::Module> createModule(const int submodulesCnt, const int elementsCnt, const QString &name)
{
    auto module = std::make_unique<Metadata::Module>(name, name);
    for (int i = 0; i < submodulesCnt; i++) {
        const QString submoduleName = name + QString("_submodule") + QString::number(i);
        module->addSubmodule(createSubmodule(elementsCnt, submoduleName));
    }

    return module;
}

std::unique_ptr<Metadata::Library> createLibrary(const int modulesCnt, const int submodulesCnt, const int elementsCnt)
{
    QString name = QString("library");
    auto library = std::make_unique<Metadata::Library>(name);

    for (int i = 0; i < modulesCnt; i++) {
        const QString moduleName = name + QString("_module") + QString::number(i);
        library->addModule(createModule(submodulesCnt, elementsCnt, moduleName));
    }

    return library;
}

QModelIndex getIndexByName(const MetadataModel *model, const QModelIndex &index, const QString &name)
{
    for (int i = 0; i < model->rowCount(index); ++i) {
        for (int j = 0; j < model->columnCount(index); ++j) {
            auto child = index.child(i, j);
            if (model->dataNode(child)->name() == name)
                return child;

            auto foundIndex = getIndexByName(model, child, name);
            if (foundIndex.isValid())
                return foundIndex;
        }
    }

    return QModelIndex();
}

void setDependecies(const MetadataModel *model, const QString &owner, const QStringList &requires, const QStringList &conflicts)
{
    const auto ownerIndex = getIndexByName(model, model->rootIndex(), owner);
    QVERIFY(ownerIndex.isValid());

    auto ownerNode = static_cast<Metadata::LibraryNode *>(ownerIndex.internalPointer());
    Metadata::Element *element = dynamic_cast<Metadata::Element *>(ownerNode);
    QVERIFY(element != nullptr);

    for (const auto &r : requires)
        element->addRequirement(r);

    for (const auto &c : conflicts)
        element->addConflict(c);
}

} // namespace Anonymous

MetadataCheckStateHandlerTests::MetadataCheckStateHandlerTests(QObject *parent)
    : QObject(parent)
{}

void MetadataCheckStateHandlerTests::test_singleElementChecked()
{
    auto library = createLibrary(2, 2, 2);
    const auto model = std::make_unique<MetadataModel>(library.get());
    MetadataCheckStateHandler handler(model.get());

    auto index = getIndexByName(model.get(), model->rootIndex(), "library_module0_submodule0_element0");
    bool ret = handler.changeCheckStates(index, Qt::Checked);
    QVERIFY(ret);

    const auto &changed = handler.changedIndexes();
    QCOMPARE(changed.size(), 3);

    QVERIFY(changed.contains(index));
    QCOMPARE(changed.value(index), Qt::Checked);

    for (QModelIndex parent = index.parent(); parent.isValid(); parent = parent.parent()) {
        QVERIFY(changed.contains(parent));
        QCOMPARE(changed.value(parent), Qt::PartiallyChecked);
    }
}

void MetadataCheckStateHandlerTests::test_allParentChildrenChecked()
{
    auto library = createLibrary(2, 2, 2);
    const auto model = std::make_unique<MetadataModel>(library.get());
    MetadataCheckStateHandler handler(model.get());

    auto index = getIndexByName(model.get(), model->rootIndex(), "library_module0_submodule0_element0");
    bool ret = handler.changeCheckStates(index, Qt::Checked);
    QVERIFY(ret);

    index = getIndexByName(model.get(), model->rootIndex(), "library_module0_submodule0_element1");
    ret = handler.changeCheckStates(index, Qt::Checked);
    QVERIFY(ret);

    const auto &changed = handler.changedIndexes();
    QCOMPARE(changed.size(), 4);

    QVERIFY(changed.contains(index));
    QCOMPARE(changed.value(index), Qt::Checked);

    QVERIFY(changed.contains(index.parent()));
    QCOMPARE(changed.value(index.parent()), Qt::Checked);

    for (QModelIndex parent = index.parent().parent(); parent.isValid(); parent = parent.parent()) {
        QVERIFY(changed.contains(parent));
        QCOMPARE(changed.value(parent), Qt::PartiallyChecked);
    }
}

void MetadataCheckStateHandlerTests::test_elementWithSingleRequirementChecked()
{
    auto library = createLibrary(3, 3, 3);
    const auto model = std::make_unique<MetadataModel>(library.get());
    MetadataCheckStateHandler handler(model.get());

    setDependecies(model.get(), "library_module0_submodule0_element0", QStringList() << "library_module0_submodule0_element1", {});

    auto index = getIndexByName(model.get(), model->rootIndex(), "library_module0_submodule0_element0");
    bool ret = handler.changeCheckStates(index, Qt::Checked);
    QVERIFY(ret);

    const auto &changed = handler.changedIndexes();
    QCOMPARE(changed.size(), 4);

    QVERIFY(changed.contains(index));
    QCOMPARE(changed.value(index), Qt::Checked);

    auto required = getIndexByName(model.get(), model->rootIndex(), "library_module0_submodule0_element1");
    QVERIFY(changed.contains(required));
    QCOMPARE(changed.value(required), Qt::Checked);

    for (QModelIndex parent = index.parent(); parent.isValid(); parent = parent.parent()) {
        QVERIFY(changed.contains(parent));
        QCOMPARE(changed.value(parent), Qt::PartiallyChecked);
    }
}

void MetadataCheckStateHandlerTests::test_parentWithDependendtChildrenChecked()
{
    auto library = createLibrary(3, 3, 3);
    const auto model = std::make_unique<MetadataModel>(library.get());
    MetadataCheckStateHandler handler(model.get());

    setDependecies(model.get(), "library_module0_submodule0_element0", QStringList() << "library_module0_submodule0_element1", {});

    auto index = getIndexByName(model.get(), model->rootIndex(), "library_module0_submodule0");
    bool ret = handler.changeCheckStates(index, Qt::Checked);
    QVERIFY(ret);

    const auto &changed = handler.changedIndexes();
    QCOMPARE(changed.size(), 5);

    for (int i = 0; i < model->rowCount(index); ++i) {
        for (int j = 0; j < model->columnCount(index); ++j) {
            const auto child = index.child(i, j);
            QVERIFY(changed.contains(child));
            QCOMPARE(changed.value(child), Qt::Checked);
        }
    }

    for (QModelIndex parent = index.parent(); parent.isValid(); parent = parent.parent()) {
        QVERIFY(changed.contains(parent));
        QCOMPARE(changed.value(parent), Qt::PartiallyChecked);
    }
}

void MetadataCheckStateHandlerTests::test_elementWithRequirementInOtherModule()
{
    auto library = createLibrary(3, 3, 3);
    const auto model = std::make_unique<MetadataModel>(library.get());
    MetadataCheckStateHandler handler(model.get());

    setDependecies(model.get(), "library_module0_submodule0_element0", QStringList() << "library_module1_submodule0_element0", {});

    auto index = getIndexByName(model.get(), model->rootIndex(), "library_module0_submodule0_element0");
    bool ret = handler.changeCheckStates(index, Qt::Checked);
    QVERIFY(ret);

    const auto &changed = handler.changedIndexes();
    QCOMPARE(changed.size(), 6);

    QVERIFY(changed.contains(index));
    QCOMPARE(changed.value(index), Qt::Checked);

    for (QModelIndex parent = index.parent(); parent.isValid(); parent = parent.parent()) {
        QVERIFY(changed.contains(parent));
        QCOMPARE(changed.value(parent), Qt::PartiallyChecked);
    }

    auto required = getIndexByName(model.get(), model->rootIndex(), "library_module1_submodule0_element0");

    QVERIFY(changed.contains(required));
    QCOMPARE(changed.value(required), Qt::Checked);

    for (QModelIndex parent = required.parent(); parent.isValid(); parent = parent.parent()) {
        QVERIFY(changed.contains(parent));
        QCOMPARE(changed.value(parent), Qt::PartiallyChecked);
    }
}

void MetadataCheckStateHandlerTests::test_elementRequiresOtherModule()
{
    auto library = createLibrary(3, 3, 3);
    const auto model = std::make_unique<MetadataModel>(library.get());
    MetadataCheckStateHandler handler(model.get());

    setDependecies(model.get(), "library_module0_submodule0_element0", QStringList() << "library_module1", {});

    auto index = getIndexByName(model.get(), model->rootIndex(), "library_module0_submodule0_element0");
    bool ret = handler.changeCheckStates(index, Qt::Checked);
    QVERIFY(ret);

    const auto &changed = handler.changedIndexes();
    QCOMPARE(changed.size(), 16);

    QVERIFY(changed.contains(index));
    QCOMPARE(changed.value(index), Qt::Checked);

    for (QModelIndex parent = index.parent(); parent.isValid(); parent = parent.parent()) {
        QVERIFY(changed.contains(parent));
        QCOMPARE(changed.value(parent), Qt::PartiallyChecked);
    }

    auto requiredIndex = getIndexByName(model.get(), model->rootIndex(), "library_module1");
    QVERIFY(changed.contains(requiredIndex));
    QCOMPARE(changed.value(requiredIndex), Qt::Checked);

    for (int i = 0; i < model->rowCount(requiredIndex); ++i) {
        for (int j = 0; j < model->columnCount(requiredIndex); ++j) {
            const auto child = requiredIndex.child(i, j);
            QVERIFY(changed.contains(child));
            QCOMPARE(changed.value(child), Qt::Checked);
        }
    }

    for (QModelIndex parent = requiredIndex.parent(); parent.isValid(); parent = parent.parent()) {
        QVERIFY(changed.contains(parent));
        QCOMPARE(changed.value(parent), Qt::PartiallyChecked);
    }
}

void MetadataCheckStateHandlerTests::test_conflictingElementChecked()
{
    auto library = createLibrary(3, 3, 3);
    const auto model = std::make_unique<MetadataModel>(library.get());
    MetadataCheckStateHandler handler(model.get());

    setDependecies(model.get(), "library_module0_submodule0_element0", {}, QStringList() << "library_module0_submodule0_element1");
    setDependecies(model.get(), "library_module0_submodule0_element1", {}, QStringList() << "library_module0_submodule0_element0");

    auto index = getIndexByName(model.get(), model->rootIndex(), "library_module0_submodule0_element0");
    bool ret = handler.changeCheckStates(index, Qt::Checked);
    QVERIFY(ret);

    const auto &changed = handler.changedIndexes();
    QVERIFY(changed.contains(index));
    QCOMPARE(changed.value(index), Qt::Checked);

    auto conflictingIndex = getIndexByName(model.get(), model->rootIndex(), "library_module0_submodule0_element1");
    ret = handler.changeCheckStates(conflictingIndex, Qt::Checked);

    QVERIFY(!ret);
}

void MetadataCheckStateHandlerTests::test_parentWithConflictingChildrenChecked()
{
    auto library = createLibrary(3, 3, 3);
    const auto model = std::make_unique<MetadataModel>(library.get());
    MetadataCheckStateHandler handler(model.get());

    setDependecies(model.get(), "library_module0_submodule0_element0", {}, QStringList() << "library_module0_submodule0_element1");
    setDependecies(model.get(), "library_module0_submodule0_element1", {}, QStringList() << "library_module0_submodule0_element0");

    auto index = getIndexByName(model.get(), model->rootIndex(), "library_module0_submodule0");
    bool ret = handler.changeCheckStates(index, Qt::Checked);
    QVERIFY(!ret);

    index = getIndexByName(model.get(), model->rootIndex(), "library_module0");
    ret = handler.changeCheckStates(index, Qt::Checked);
    QVERIFY(!ret);
}

void MetadataCheckStateHandlerTests::test_cyclicDependedncy()
{
    auto library = createLibrary(3, 3, 3);
    const auto model = std::make_unique<MetadataModel>(library.get());
    MetadataCheckStateHandler handler(model.get());

    setDependecies(model.get(), "library_module0_submodule0_element0", QStringList() << "library_module0_submodule0_element1", {});
    setDependecies(model.get(), "library_module0_submodule0_element1", QStringList() << "library_module0_submodule0_element0", {});

    auto index = getIndexByName(model.get(), model->rootIndex(), "library_module0_submodule0");
    bool ret = handler.changeCheckStates(index, Qt::Checked);
    QVERIFY(ret);

    const auto &changed = handler.changedIndexes();
    QCOMPARE(changed.size(), 5);

    for (int i = 0; i < model->rowCount(index); ++i) {
        for (int j = 0; j < model->columnCount(index); ++j) {
            const auto child = index.child(i, j);
            QVERIFY(changed.contains(child));
            QCOMPARE(changed.value(child), Qt::Checked);
        }
    }

    for (QModelIndex parent = index.parent(); parent.isValid(); parent = parent.parent()) {
        QVERIFY(changed.contains(parent));
        QCOMPARE(changed.value(parent), Qt::PartiallyChecked);
    }
}