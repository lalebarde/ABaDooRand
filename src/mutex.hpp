/*
    Copyright (c) 2014 Contributors as noted in the AUTHORS file
    
    This file is part of ABaDooRand.
    
    ABaDooRand is available under two different licensing options designed to accommodate the needs of our various users.
    
    ABaDooRand licensed under our commercial license agreement is appropriate for development of proprietary/commercial 
    software where you do not want to share any source code with third parties or otherwise cannot comply with the terms 
    of the [GNU General Public License (GPL) version 3.0](GNU-GPL-v3.0). 
    If you are under this use case, please contact the author.
    
    ABaDooRand licensed under the [GNU GPL version 3.0](GNU-GPL-v3.0) is appropriate for the development of 
    ABaDooRand applications where you wish to use such applications in combination with software subject to the terms 
    of the [GNU GPL version 3.0](GNU-GPL-v3.0) or where you are otherwise willing to comply with the terms of the 
    [GNU GPL version 3.0](GNU-GPL-v3.0).
    
    ABaDooRand documentation is licensed under the terms of the [GNU Free Documentation License (FDL) version 1.3](GNU-FDL-v1.3),
    as published by the Free Software Foundation.
    
    ABaDooRand is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
    
    You should have received a copy of the licences along with this program, LICENCE.txt and the GNU licenses.  If not,
    see <http://www.gnu.org/licenses/> for the GNU ones.
*/

#ifndef __ABADOORAND_MUTEX_HPP_INCLUDED__
#define __ABADOORAND_MUTEX_HPP_INCLUDED__

#include "platform.hpp"
#include "err.hpp"

//  Mutex class encapsulates OS mutex in a platform-independent way.

#ifdef ABADOORAND_HAVE_WINDOWS

#include "windows.hpp"

namespace abadoorand
{

    class mutex_t
    {
    public:
        inline mutex_t ()
        {
            InitializeCriticalSection (&cs);
        }

        inline ~mutex_t ()
        {
            DeleteCriticalSection (&cs);
        }

        inline void lock ()
        {
            EnterCriticalSection (&cs);
        }

        inline bool try_lock ()
        {
            return (TryEnterCriticalSection (&cs)) ? true : false;
        }

        inline void unlock ()
        {
            LeaveCriticalSection (&cs);
        }

    private:

        CRITICAL_SECTION cs;

        //  Disable copy construction and assignment.
        mutex_t (const mutex_t&);
        void operator = (const mutex_t&);
    };

}

#else

#include <pthread.h>

namespace abadoorand
{

    class mutex_t
    {
    public:
        inline mutex_t ()
        {
            int rc = pthread_mutex_init (&mutex, NULL);
            posix_assert (rc);
        }

        inline ~mutex_t ()
        {
            int rc = pthread_mutex_destroy (&mutex);
            posix_assert (rc);
        }

        inline void lock ()
        {
            int rc = pthread_mutex_lock (&mutex);
            posix_assert (rc);
        }

        inline bool try_lock ()
        {
            int rc = pthread_mutex_trylock (&mutex);
            if (rc == EBUSY)
                return false;

            posix_assert (rc);
            return true;
        }

        inline void unlock ()
        {
            int rc = pthread_mutex_unlock (&mutex);
            posix_assert (rc);
        }

    private:

        pthread_mutex_t mutex;

        // Disable copy construction and assignment.
        mutex_t (const mutex_t&);
        const mutex_t &operator = (const mutex_t&);
    };

}

#endif


namespace abadoorand
{
    struct scoped_lock_t
    {
        scoped_lock_t (mutex_t& mutex_)
            : mutex (mutex_)
        {
            mutex.lock ();
        }

        ~scoped_lock_t ()
        {
            mutex.unlock ();
        }

    private:

        mutex_t& mutex;

        // Disable copy construction and assignment.
        scoped_lock_t (const scoped_lock_t&);
        const scoped_lock_t &operator = (const scoped_lock_t&);
    };
}

#endif
