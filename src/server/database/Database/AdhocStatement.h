/*
 * Copyright (C) 2022 Firelands Project <https://github.com/FirelandsProject>
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _ADHOCSTATEMENT_H
#define _ADHOCSTATEMENT_H

#include "DatabaseEnvFwd.h"
#include "Define.h"
#include "SQLOperation.h"

class FC_DATABASE_API BasicStatementTask : public SQLOperation
{
public:
    BasicStatementTask(std::string_view sql, bool async = false);
    ~BasicStatementTask();

    bool Execute() override;
    QueryResultFuture GetFuture() const { return m_result->get_future(); }

private:
    std::string m_sql; //- Raw query to be executed
    bool m_has_result;
    QueryResultPromise* m_result;
};

#endif
