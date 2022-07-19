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

#ifndef FIRELANDS_SINGLETON_H
#define FIRELANDS_SINGLETON_H

/**
 * @brief class Singleton
 */

#include "CreationPolicy.h"
#include "ThreadingModel.h"
#include "ObjectLifeTime.h"

namespace Firelands
{
    template
    <
    typename T,
             class ThreadingModel = Firelands::SingleThreaded<T>,
             class CreatePolicy = Firelands::OperatorNew<T>,
             class LifeTimePolicy = Firelands::ObjectLifeTime<T>
             >
    /**
     * @brief
     *
     */
    class Singleton
    {
        public:

            /**
             * @brief
             *
             * @return T
             */
            static T& Instance();

        protected:

            /**
             * @brief
             *
             */
            Singleton()
            {
            }

        private:

            /**
             * @brief Prohibited actions...this does not prevent hijacking.
             *
             * @param
             */
            Singleton(const Singleton&);
            /**
             * @brief
             *
             * @param
             * @return Singleton &operator
             */
            Singleton& operator=(const Singleton&);

            /**
             * @brief Singleton Helpers
             *
             */
            static void DestroySingleton();

            /**
             * @brief data structure
             *
             */
            typedef typename ThreadingModel::Lock Guard;
            static T* si_instance; /**< TODO */
            static bool si_destroyed; /**< TODO */
    };

    template<typename T, class ThreadingModel, class CreatePolicy, class LifeTimePolicy>
    T* Singleton<T, ThreadingModel, CreatePolicy, LifeTimePolicy>::si_instance = NULL; /**< TODO */

    template<typename T, class ThreadingModel, class CreatePolicy, class LifeTimePolicy>
    bool Singleton<T, ThreadingModel, CreatePolicy, LifeTimePolicy>::si_destroyed = false; /**< TODO */

    template<typename T, class ThreadingModel, class CreatePolicy, class LifeTimePolicy>
    /**
     * @brief
     *
     * @return T &Firelands::Singleton<T, ThreadingModel, CreatePolicy, LifeTimePolicy>
     */
    T& Firelands::Singleton<T, ThreadingModel, CreatePolicy, LifeTimePolicy>::Instance()
    {
        if (!si_instance)
        {
            // double-checked Locking pattern
            Guard();

            if (!si_instance)
            {
                if (si_destroyed)
                {
                    si_destroyed = false;
                    LifeTimePolicy::OnDeadReference();
                }

                si_instance = CreatePolicy::Create();
                LifeTimePolicy::ScheduleCall(&DestroySingleton);
            }
        }

        return *si_instance;
    }

    template<typename T, class ThreadingModel, class CreatePolicy, class LifeTimePolicy>
    /**
     * @brief
     *
     */
    void Firelands::Singleton<T, ThreadingModel, CreatePolicy, LifeTimePolicy>::DestroySingleton()
    {
        CreatePolicy::Destroy(si_instance);
        si_instance = NULL;
        si_destroyed = true;
    }
}

#define INSTANTIATE_SINGLETON_1(TYPE) \
    template class Firelands::Singleton<TYPE, Firelands::SingleThreaded<TYPE>, Firelands::OperatorNew<TYPE>, Firelands::ObjectLifeTime<TYPE> >;

#define INSTANTIATE_SINGLETON_2(TYPE, THREADINGMODEL) \
    template class Firelands::Singleton<TYPE, THREADINGMODEL, Firelands::OperatorNew<TYPE>, Firelands::ObjectLifeTime<TYPE> >;

#define INSTANTIATE_SINGLETON_3(TYPE, THREADINGMODEL, CREATIONPOLICY ) \
    template class Firelands::Singleton<TYPE, THREADINGMODEL, CREATIONPOLICY, Firelands::ObjectLifeTime<TYPE> >;

#define INSTANTIATE_SINGLETON_4(TYPE, THREADINGMODEL, CREATIONPOLICY, OBJECTLIFETIME) \
    template class Firelands::Singleton<TYPE, THREADINGMODEL, CREATIONPOLICY, OBJECTLIFETIME >;

#endif
