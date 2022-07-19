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

#ifndef FIRELANDS_THREADINGMODEL_H
#define FIRELANDS_THREADINGMODEL_H

/**
 * @class ThreadingModel<T>
 *
 */

#include "Platform/Define.h"

namespace Firelands
{
    template<typename MUTEX>
    /**
     * @brief
     *
     */
    class GeneralLock
    {
        public:

            /**
             * @brief
             *
             * @param m
             */
            GeneralLock(MUTEX& m)
                : i_mutex(m)
            {
                i_mutex.acquire();
            }

            /**
             * @brief
             *
             */
            ~GeneralLock()
            {
                i_mutex.release();
            }

        private:

            /**
             * @brief
             *
             * @param
             */
            GeneralLock(const GeneralLock&);
            /**
             * @brief
             *
             * @param
             * @return GeneralLock &operator
             */
            GeneralLock& operator=(const GeneralLock&);
            MUTEX& i_mutex; /**< TODO */
    };

    template<class T>
    /**
     * @brief
     *
     */
    class SingleThreaded
    {
        public:

            /**
             * @brief empty object
             *
             */
            struct Lock
            {
                /**
                 * @brief
                 *
                 */
                Lock()
                {
                }
                /**
                 * @brief
                 *
                 * @param
                 */
                Lock(const T&)
                {
                }

                /**
                 * @brief for single threaded we ignore this
                 *
                 * @param
                 */
                Lock(const SingleThreaded<T>&)
                {
                }
            };
    };

    template<class T, class MUTEX>
    /**
     * @brief
     *
     */
    class ObjectLevelLockable
    {
        public:

            /**
             * @brief
             *
             */
            ObjectLevelLockable()
                : i_mtx()
            {
            }

            friend class Lock;

            /**
             * @brief
             *
             */
            class Lock
            {
                public:

                    /**
                     * @brief
                     *
                     * @param ObjectLevelLockable<T
                     * @param host
                     */
                    Lock(ObjectLevelLockable<T, MUTEX>& host)
                        : i_lock(host.i_mtx)
                    {
                    }

                private:

                    GeneralLock<MUTEX> i_lock; /**< TODO */
            };

        private:

            /**
             * @brief prevent the compiler creating a copy construct
             *
             * @param ObjectLevelLockable<T
             * @param
             */
            ObjectLevelLockable(const ObjectLevelLockable<T, MUTEX>&);
            /**
             * @brief
             *
             * @param ObjectLevelLockable<T
             * @param
             * @return ObjectLevelLockable<T, MUTEX>
             */
            ObjectLevelLockable<T, MUTEX>& operator=(const ObjectLevelLockable<T, MUTEX>&);

            MUTEX i_mtx; /**< TODO */
    };

    template<class T, class MUTEX>
    /**
     * @brief
     *
     */
    class ClassLevelLockable
    {
        public:

            /**
             * @brief
             *
             */
            ClassLevelLockable()
            {
            }

            friend class Lock;

            /**
             * @brief
             *
             */
            class Lock
            {
                public:

                    /**
                     * @brief
                     *
                     * @param
                     */
                    Lock(const T& /*host*/)
                    {
                        ClassLevelLockable<T, MUTEX>::si_mtx.acquire();
                    }

                    /**
                     * @brief
                     *
                     * @param ClassLevelLockable<T
                     * @param
                     */
                    Lock(const ClassLevelLockable<T, MUTEX>&)
                    {
                        ClassLevelLockable<T, MUTEX>::si_mtx.acquire();
                    }

                    /**
                     * @brief
                     *
                     */
                    Lock()
                    {
                        ClassLevelLockable<T, MUTEX>::si_mtx.acquire();
                    }

                    /**
                     * @brief
                     *
                     */
                    ~Lock()
                    {
                        ClassLevelLockable<T, MUTEX>::si_mtx.release();
                    }
            };

        private:

            static MUTEX si_mtx; /**< TODO */
    };

}

template<class T, class MUTEX> MUTEX Firelands::ClassLevelLockable<T, MUTEX>::si_mtx; /**< TODO */

#define INSTANTIATE_CLASS_MUTEX(CTYPE, MUTEX) \
    template class Firelands::ClassLevelLockable<CTYPE, MUTEX>

#endif
