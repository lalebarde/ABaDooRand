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

#ifndef __ABADOORAND_ERR_HPP_INCLUDED__
#define __ABADOORAND_ERR_HPP_INCLUDED__

//  ABaDooRand-specific error codes are defined in abadoorand.h
#include "../include/abadoorand.h"

#include <assert.h>
#if defined _WIN32_WCE
#include "..\builds\msvc\errno.hpp"
#else
#include <errno.h>
#endif
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "platform.hpp"
#include "likely.hpp"

#ifdef ABADOORAND_HAVE_WINDOWS
#include "windows.hpp"
#else
#include <netdb.h>
#endif

// EPROTO is not used by OpenBSD and maybe other platforms.
#ifndef EPROTO
#define EPROTO 0
#endif

namespace abadoorand
{
    const char *errno_to_string (int errno_);
    void abadoorand_abort (const char *errmsg_);
}

#ifdef ABADOORAND_HAVE_WINDOWS

namespace abadoorand
{
    const char *wsa_error ();
    const char *wsa_error_no (int no_);
    void win_error (char *buffer_, size_t buffer_size_);
    int wsa_error_to_errno (int errcode);
}

//  Provides convenient way to check WSA-style errors on Windows.
#define wsa_assert(x) \
    do {\
        if (unlikely (!(x))) {\
            const char *errstr = abadoorand::wsa_error ();\
            if (errstr != NULL) {\
                fprintf (stderr, "Assertion failed: %s (%s:%d)\n", errstr, \
                    __FILE__, __LINE__);\
                abadoorand::abadoorand_abort (errstr);\
            }\
        }\
    } while (false)

//  Provides convenient way to assert on WSA-style errors on Windows.
#define wsa_assert_no(no) \
    do {\
        const char *errstr = abadoorand::wsa_error_no (no);\
        if (errstr != NULL) {\
            fprintf (stderr, "Assertion failed: %s (%s:%d)\n", errstr, \
                __FILE__, __LINE__);\
            abadoorand::abadoorand_abort (errstr);\
        }\
    } while (false)

// Provides convenient way to check GetLastError-style errors on Windows.
#define win_assert(x) \
    do {\
        if (unlikely (!(x))) {\
            char errstr [256];\
            abadoorand::win_error (errstr, 256);\
            fprintf (stderr, "Assertion failed: %s (%s:%d)\n", errstr, \
                __FILE__, __LINE__);\
            abadoorand::abadoorand_abort (errstr);\
        }\
    } while (false)

#endif

//  This macro works in exactly the same way as the normal assert. It is used
//  in its stead because standard assert on Win32 in broken - it prints nothing
//  when used within the scope of JNI library.
#define abadoorand_assert(x) \
    do {\
        if (unlikely (!(x))) {\
            fprintf (stderr, "Assertion failed: %s (%s:%d)\n", #x, \
                __FILE__, __LINE__);\
            abadoorand::abadoorand_abort (#x);\
        }\
    } while (false) 

//  Provides convenient way to check for errno-style errors.
#define errno_assert(x) \
    do {\
        if (unlikely (!(x))) {\
            const char *errstr = strerror (errno);\
            fprintf (stderr, "%s (%s:%d)\n", errstr, __FILE__, __LINE__);\
            abadoorand::abadoorand_abort (errstr);\
        }\
    } while (false)

//  Provides convenient way to check for POSIX errors.
#define posix_assert(x) \
    do {\
        if (unlikely (x)) {\
            const char *errstr = strerror (x);\
            fprintf (stderr, "%s (%s:%d)\n", errstr, __FILE__, __LINE__);\
            abadoorand::abadoorand_abort (errstr);\
        }\
    } while (false)

//  Provides convenient way to check for errors from getaddrinfo.
#define gai_assert(x) \
    do {\
        if (unlikely (x)) {\
            const char *errstr = gai_strerror (x);\
            fprintf (stderr, "%s (%s:%d)\n", errstr, __FILE__, __LINE__);\
            abadoorand::abadoorand_abort (errstr);\
        }\
    } while (false)

//  Provides convenient way to check whether memory allocation have succeeded.
#define alloc_assert(x) \
    do {\
        if (unlikely (!x)) {\
            fprintf (stderr, "FATAL ERROR: OUT OF MEMORY (%s:%d)\n",\
                __FILE__, __LINE__);\
            abadoorand::abadoorand_abort ("FATAL ERROR: OUT OF MEMORY");\
        }\
    } while (false)

#endif


