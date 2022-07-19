/*
 * This file is part of the Firelands Core Project. See AUTHORS file for Copyright information
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

#if !defined(QUERYRESULTMYSQL_H)
#define QUERYRESULTMYSQL_H

#include "Common/Common.h"

#ifdef WIN32
#include <winsock2.h>
#endif

#include <mysql.h>

/**
 * @brief
 *
 */
class QueryResultMysql : public QueryResult
{
    public:
        /**
         * @brief
         *
         * @param result
         * @param fields
         * @param rowCount
         * @param fieldCount
         */
        QueryResultMysql(MYSQL_RES* result, MYSQL_FIELD* fields, uint64 rowCount, uint32 fieldCount);

        /**
         * @brief
         *
         */
        ~QueryResultMysql();

        /**
         * @brief
         *
         * @return bool
         */
        bool NextRow() override;

        /**
         * @brief
         *
         * @param type
         * @return Field::SimpleDataTypes
         */
    private:
        enum Field::DataTypes ConvertNativeType(enum_field_types mysqlType) const;
        /**
         * @brief
         *
         */
        void EndQuery();

        MYSQL_RES* mResult; /**< TODO */
};
#endif
