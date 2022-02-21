/****************************************************************************
**
** Copyright (C) 2018-2019 N7 Space sp. z o. o.
** Contact: http://n7space.com
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

#include "asn1acnerrorparser.h"

#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/task.h>

#include <data/errormessage.h>

using namespace Asn1Acn::Internal;

// TODO sprawdzic
Utils::OutputLineParser::Result Asn1AcnErrorParser::handleLine(const QString &line,
                                                               Utils::OutputFormat type)
{
    if (type == Utils::OutputFormat::StdErrFormat) {
        if (line.isEmpty())
            return Status::NotHandled;

        const auto error = m_parser.parse(line);
        if (!error.isValid())
            return Status::NotHandled;

        ProjectExplorer::Task task(ProjectExplorer::Task::Error,
                                   error.message(),
                                   Utils::FilePath::fromString(error.location().path()),
                                   error.location().line(),
                                   ProjectExplorer::Constants::TASK_CATEGORY_COMPILE);

        scheduleTask(task, 1);

        return Status::Done;
    }

    return Status::NotHandled;
}
