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

#ifndef FIRELANDS_H_GRIDREFERENCE
#define FIRELANDS_H_GRIDREFERENCE

#include "Utilities/LinkedReference/Reference.h"

template<class OBJECT> class GridRefManager;

template<class OBJECT>
/**
 * @brief
 *
 */
class GridReference : public Reference<GridRefManager<OBJECT>, OBJECT>
{
    protected:

        /**
         * @brief
         *
         */
        void targetObjectBuildLink() override
        {
            // called from link()
            this->getTarget()->insertFirst(this);
            this->getTarget()->incSize();
        }

        /**
         * @brief
         *
         */
        void targetObjectDestroyLink() override
        {
            // called from unlink()
            if (this->isValid())
            {
                this->getTarget()->decSize();
            }
        }

        /**
         * @brief
         *
         */
        void sourceObjectDestroyLink() override
        {
            // called from invalidate()
            this->getTarget()->decSize();
        }

    public:
        /**
         * @brief
         *
         */
        GridReference()
            : Reference<GridRefManager<OBJECT>, OBJECT>()
        {
        }

        /**
         * @brief
         *
         */
        ~GridReference()
        {
            this->unlink();
        }

        /**
         * @brief
         *
         * @return GridReference
         */
        GridReference* next()
        {
            return (GridReference*)Reference<GridRefManager<OBJECT>, OBJECT>::next();
        }
};

#endif
