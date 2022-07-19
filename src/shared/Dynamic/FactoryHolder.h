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

#ifndef FIRELANDS_FACTORY_HOLDER
#define FIRELANDS_FACTORY_HOLDER

#include "Platform/Define.h"
#include "Utilities/TypeList.h"
#include "ObjectRegistry.h"
#include "Policies/Singleton.h"

template < class T, class Key = std::string >
/**
 * @brief FactoryHolder holds a factory object of a specific type
 *
 */
class FactoryHolder
{
    public:
        /**
         * @brief
         *
         */
        typedef ObjectRegistry<FactoryHolder<T, Key >, Key > FactoryHolderRegistry;
        /**
         * @brief
         *
         */
        typedef Firelands::Singleton<FactoryHolderRegistry > FactoryHolderRepository;

        /**
         * @brief
         *
         * @param k
         */
        FactoryHolder(Key k) : i_key(k) {}
        /**
         * @brief
         *
         */
        virtual ~FactoryHolder() {}
        /**
         * @brief
         *
         * @return Key
         */
        inline Key key() const { return i_key; }

        /**
         * @brief
         *
         */
        void RegisterSelf(void) { FactoryHolderRepository::Instance().InsertItem(this, i_key); }
        /**
         * @brief
         *
         */
        void DeregisterSelf(void) { FactoryHolderRepository::Instance().RemoveItem(this, false); }

        /**
         * @brief Abstract Factory create method
         *
         * @param data
         * @return T
         */
        virtual T* Create(void* data = NULL) const = 0;
    private:
        Key i_key; /**< TODO */
};

template<class T>
/**
 * @brief Permissible is a classic way of letting the object decide whether how good they handle things.
 *
 * This is not retricted to factory selectors.
 */
class Permissible
{
    public:
        /**
         * @brief
         *
         */
        virtual ~Permissible() {}
        /**
         * @brief
         *
         * @param
         * @return int
         */
        virtual int Permit(const T*) const = 0;
};
#endif
