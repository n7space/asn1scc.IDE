/****************************************************************************
**
** Copyright (C) 2017-2022 N7 Space sp. z o. o.
** Contact: https://n7space.com
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

#include "filemodel.h"

using namespace Asn1Acn::Internal::Libraries;

FileModel::FileModel(QObject *parent)
    : QFileSystemModel(parent)
{
    connect(this, &QFileSystemModel::directoryLoaded, this, &FileModel::onDirectoryLoaded);
}

Qt::ItemFlags FileModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return {};

    return Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
}

int FileModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant FileModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::CheckStateRole)
        return m_checkStates.contains(index) ? m_checkStates.value(index) : Qt::Unchecked;

    return QFileSystemModel::data(index, role);
}

bool FileModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::CheckStateRole) {
        if (!index.isValid())
            return false;

        setItemCheck(index, value);
        setChildrenCheck(index, value);
        setParentCheck(index);

        return true;
    }

    return QFileSystemModel::setData(index, value, role);
}

void FileModel::setItemCheck(const QModelIndex &index, const QVariant &value)
{
    changeCheckState(index, value);
    emit dataChanged(index, index);
}

void FileModel::setChildrenCheck(const QModelIndex &parent, const QVariant &value)
{
    if (!parent.isValid())
        return;

    for (int i = 0; i < rowCount(parent); ++i) {
        for (int j = 0; j < columnCount(parent); ++j) {
            const auto child = index(i, j, parent);
            changeCheckState(child, value);
            setChildrenCheck(child, value);
        }
    }

    const auto &model = parent.model();
    emit dataChanged(model->index(0, 0, parent),
                     model->index(rowCount(parent), rowCount(parent), parent));
}

void FileModel::setParentCheck(const QModelIndex &index)
{
    auto checkState = parentState(index);

    for (auto parent = index.parent(); parent.isValid(); parent = parent.parent()) {
        changeCheckState(parent, checkState);
        checkState = parentState(parent);

        emit dataChanged(parent, parent);
    }
}

void FileModel::changeCheckState(const QModelIndex &index, const QVariant &value)
{
    m_checkStates.insert(index, static_cast<Qt::CheckState>(value.toInt()));
}

QVariant FileModel::parentState(const QModelIndex &index) const
{
    const auto checkState = index.data(Qt::CheckStateRole);
    const auto parent = index.parent();
    const auto &model = index.model();

    for (int i = 0; i < rowCount(parent); ++i)
        for (int j = 0; j < columnCount(parent); ++j)
            if (model->index(i, j, parent).data(Qt::CheckStateRole) != checkState)
                return Qt::PartiallyChecked;

    return checkState;
}

void FileModel::onDirectoryLoaded(const QString &path)
{
    const auto idx = index(path);
    auto checkState = idx.data(Qt::CheckStateRole);

    if (checkState == Qt::Unchecked)
        return;

    setChildrenCheck(idx, checkState);
}
