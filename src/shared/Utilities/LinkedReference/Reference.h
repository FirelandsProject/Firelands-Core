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

#ifndef FIRELANDS_H_REFERENCE
#define FIRELANDS_H_REFERENCE

#include "Utilities/LinkedList.h"

//=====================================================

template<class TO, class FROM>
/**
 * @brief
 *
 */
class Reference : public LinkedListElement
{
    private:

        TO* iRefTo; /**< TODO */
        FROM* iRefFrom; /**< TODO */

    protected:

        /**
         * @brief Tell our refTo (target) object that we have a link
         *
         */
        virtual void targetObjectBuildLink() = 0;

        /**
         * @brief Tell our refTo (taget) object, that the link is cut
         *
         */
        virtual void targetObjectDestroyLink() = 0;

        /**
         * @brief Tell our refFrom (source) object, that the link is cut (Target destroyed)
         *
         */
        virtual void sourceObjectDestroyLink() = 0;

    public:

        /**
         * @brief
         *
         */
        Reference()
            : iRefTo(NULL), iRefFrom(NULL)
        {
        }

        /**
         * @brief
         *
         */
        virtual ~Reference() {}

        /**
         * @brief Create new link
         *
         * @param toObj
         * @param fromObj
         */
        void link(TO* toObj, FROM* fromObj)
        {
            assert(fromObj);                                // fromObj MUST not be NULL
            if (isValid())
            {
                unlink();
            }

            if (toObj != NULL)
            {
                iRefTo = toObj;
                iRefFrom = fromObj;
                targetObjectBuildLink();
            }
        }

        /**
         * @brief We don't need the reference anymore.
         *
         * Call comes from the refFrom object. Tell our refTo object, that the
         * link is cut.
         *
         */
        void unlink()
        {
            targetObjectDestroyLink();
            delink();
            iRefTo = NULL;
            iRefFrom = NULL;
        }

        /**
         * @brief Link is invalid due to destruction of referenced target object.
         *
         * Call comes from the refTo object. Tell our refFrom object, that the
         * link is cut.
         *
         */
        void invalidate()                                   // the iRefFrom MUST remain!!
        {
            sourceObjectDestroyLink();
            delink();
            iRefTo = NULL;
        }

        /**
         * @brief
         *
         * @return bool
         */
        bool isValid() const                                // Only check the iRefTo
        {
            return iRefTo != NULL;
        }

        /**
         * @brief
         *
         * @return Reference<TO, FROM>
         */
        Reference<TO, FROM>*       next()       { return((Reference<TO, FROM>*) LinkedListElement::next()); }
        /**
         * @brief
         *
         * @return const Reference<TO, FROM>
         */
        Reference<TO, FROM> const* next() const { return((Reference<TO, FROM> const*) LinkedListElement::next()); }
        /**
         * @brief
         *
         * @return Reference<TO, FROM>
         */
        Reference<TO, FROM>*       prev()       { return((Reference<TO, FROM>*) LinkedListElement::prev()); }
        /**
         * @brief
         *
         * @return const Reference<TO, FROM>
         */
        Reference<TO, FROM> const* prev() const { return((Reference<TO, FROM> const*) LinkedListElement::prev()); }

        /**
         * @brief
         *
         * @return Reference<TO, FROM>
         */
        Reference<TO, FROM>*       nocheck_next()       { return((Reference<TO, FROM>*) LinkedListElement::nocheck_next()); }
        /**
         * @brief
         *
         * @return const Reference<TO, FROM>
         */
        Reference<TO, FROM> const* nocheck_next() const { return((Reference<TO, FROM> const*) LinkedListElement::nocheck_next()); }
        /**
         * @brief
         *
         * @return Reference<TO, FROM>
         */
        Reference<TO, FROM>*       nocheck_prev()       { return((Reference<TO, FROM>*) LinkedListElement::nocheck_prev()); }
        /**
         * @brief
         *
         * @return const Reference<TO, FROM>
         */
        Reference<TO, FROM> const* nocheck_prev() const { return((Reference<TO, FROM> const*) LinkedListElement::nocheck_prev()); }

        /**
         * @brief
         *
         * @return TO *operator ->
         */
        TO* operator->() const { return iRefTo; }
        /**
         * @brief
         *
         * @return TO
         */
        TO* getTarget() const { return iRefTo; }

        /**
         * @brief
         *
         * @return FROM
         */
        FROM* getSource() const { return iRefFrom; }
};

//=====================================================

#endif