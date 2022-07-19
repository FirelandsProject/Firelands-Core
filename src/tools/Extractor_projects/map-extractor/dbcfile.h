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

#ifndef DBCFILE_H
#define DBCFILE_H

#include <cassert>
#include <string>
#include "StormLib.h"

/**
 * @brief
 *
 */
class DBCFile
{
    public:
        /**
         * @brief
         *
         * @param filename
         */
        DBCFile(const std::string& filename);
        DBCFile(HANDLE file);
        /**
         * @brief
         *
         */
        ~DBCFile();

        /**
         * @brief Open database. It must be openened before it can be used.
         *
         * @return bool
         */
        bool open();

        /**
         * @brief Database exceptions
         *
         */
        class Exception
        {
            public:
                /**
                 * @brief
                 *
                 * @param message
                 */
                Exception(const std::string& message): message(message)
                { }
                /**
                 * @brief
                 *
                 */
                virtual ~Exception()
                { }
                /**
                 * @brief
                 *
                 * @return const std::string
                 */
                const std::string& getMessage() {return message;}
            private:
                std::string message; /**< TODO */
        };
        /**
         * @brief
         *
         */
        class NotFound: public Exception
        {
            public:
                /**
                 * @brief
                 *
                 */
                NotFound(): Exception("Key was not found")
                { }
        };
        class Iterator;
        /**
         * @brief Iteration over database
         *
         */
        class Record
        {
            public:
                /**
                 * @brief
                 *
                 * @param field
                 * @return float
                 */
                float getFloat(size_t field) const
                {
                    assert(field < file.fieldCount);
                    return *reinterpret_cast<float*>(offset + (field * 4));
                }
                /**
                 * @brief
                 *
                 * @param field
                 * @return unsigned int
                 */
                unsigned int getUInt(size_t field) const
                {
                    assert(field < file.fieldCount);
                    return *reinterpret_cast<unsigned int*>(offset + (field * 4));
                }
                /**
                 * @brief
                 *
                 * @param field
                 * @return int
                 */
                int getInt(size_t field) const
                {
                    assert(field < file.fieldCount);
                    return *reinterpret_cast<int*>(offset + (field * 4));
                }
                /**
                 * @brief
                 *
                 * @param field
                 * @return const char
                 */
                const char* getString(size_t field) const
                {
                    assert(field < file.fieldCount);
                    size_t stringOffset = getUInt(field);
                    assert(stringOffset < file.stringSize);
                    return reinterpret_cast<char*>(file.stringTable + stringOffset);
                }
            private:
                /**
                 * @brief
                 *
                 * @param file
                 * @param offset
                 */
                Record(DBCFile& file, unsigned char* offset): file(file), offset(offset) {}
                DBCFile& file; /**< TODO */
                unsigned char* offset; /**< TODO */

                friend class DBCFile;
                friend class DBCFile::Iterator;
        };

        /**
         * @brief Iterator that iterates over records
         *
         */
        class Iterator
        {
            public:
                /**
                 * @brief
                 *
                 * @param file
                 * @param offset
                 */
                Iterator(DBCFile& file, unsigned char* offset):
                    record(file, offset) {}
                /**
                 * @brief Advance (prefix only)
                 *
                 * @return Iterator &operator
                 */
                Iterator& operator++()
                {
                    record.offset += record.file.recordSize;
                    return *this;
                }
                /**
                 * @brief Return address of current instance
                 *
                 * @return const Record &operator
                 */
                Record const& operator*() const { return record; }
                /**
                 * @brief
                 *
                 * @return const Record *operator ->
                 */
                const Record* operator->() const
                {
                    return &record;
                }
                /**
                 * @brief Comparison
                 *
                 * @param b
                 * @return bool operator
                 */
                bool operator==(const Iterator& b) const
                {
                    return record.offset == b.record.offset;
                }
                /**
                 * @brief
                 *
                 * @param b
                 * @return bool operator
                 */
                bool operator!=(const Iterator& b) const
                {
                    return record.offset != b.record.offset;
                }
            private:
                Record record; /**< TODO */
        };

        /**
         * @brief Get record by id
         *
         * @param id
         * @return Record
         */
        Record getRecord(size_t id);
        /**
         * @brief Get begin iterator over records
         *
         * @return Iterator
         */
        Iterator begin();
        /**
         * @brief Get begin iterator over records
         *
         * @return Iterator
         */
        Iterator end();
        /**
         * @brief Trivial
         *
         * @return size_t
         */
        size_t getRecordCount() const { return recordCount;}
        /**
         * @brief
         *
         * @return size_t
         */
        size_t getFieldCount() const { return fieldCount; }
        /**
         * @brief
         *
         * @return size_t
         */
        size_t getMaxId();
    private:
        std::string filename; /**< TODO */
        HANDLE fileHandle; /**< TODO */
        size_t recordSize; /**< TODO */
        size_t recordCount; /**< TODO */
        size_t fieldCount; /**< TODO */
        size_t stringSize; /**< TODO */
        unsigned char* data; /**< TODO */
        unsigned char* stringTable; /**< TODO */
};

#endif
