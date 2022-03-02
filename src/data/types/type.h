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
#pragma once

#include <QIcon>
#include <QString>
#include <utils/filepath.h>

#include <utils/theme/theme.h>

namespace Asn1Acn {
namespace Internal {
namespace Data {
namespace Types {

class Type
{
public:
    virtual ~Type() = default;

    QIcon typeIcon() const;
    QIcon valueIcon() const;

    virtual QString name() const = 0;
    virtual QString label() const = 0;

private:
    virtual QIcon createIcon(Utils::Theme::Color color) const;
    virtual Utils::FilePath baseIconFile() const = 0;
};

} // namespace Types
} // namespace Data
} // namespace Internal
} // namespace Asn1Acn
