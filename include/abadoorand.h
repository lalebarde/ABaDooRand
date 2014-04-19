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

#ifndef __ABADOORAND_H_INCLUDED__
#define __ABADOORAND_H_INCLUDED__

/*  Version macros for compile-time API version detection                     */
/*  Version macros for compile-time API version detection                     */
#define ABADOORAND_VERSION_MAJOR 0
#define ABADOORAND_VERSION_MINOR 1
#define ABADOORAND_VERSION_PATCH 0

#define ABADOORAND_MAKE_VERSION(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))
#define ABADOORAND_VERSION \
    ABADOORAND_MAKE_VERSION(ABADOORAND_VERSION_MAJOR, ABADOORAND_VERSION_MINOR, ABADOORAND_VERSION_PATCH)

#ifdef __cplusplus
extern "C" {
#endif

#if !defined _WIN32_WCE
#include <errno.h>
#endif
#include <stddef.h>
#include <stdio.h>
#if defined _WIN32
#include <winsock2.h>
#endif

/*  Handle DSO symbol visibility                                             */
/*  Handle DSO symbol visibility                                             */
#if defined _WIN32
#   if defined ABADOORAND_STATIC
#       define ABADOORAND_EXPORT
#   elif defined DLL_EXPORT
#       define ABADOORAND_EXPORT __declspec(dllexport)
#   else
#       define ABADOORAND_EXPORT __declspec(dllimport)
#   endif
#else
#   if defined __SUNPRO_C  || defined __SUNPRO_CC
#       define ABADOORAND_EXPORT __global
#   elif (defined __GNUC__ && __GNUC__ >= 4) || defined __INTEL_COMPILER
#       define ABADOORAND_EXPORT __attribute__ ((visibility("default")))
#   else
#       define ABADOORAND_EXPORT
#   endif
#endif

/*  Define integer types needed for event interface                          */
/*  Define integer types needed for event interface                          */
#if defined ABADOORAND_HAVE_SOLARIS || defined ABADOORAND_HAVE_OPENVMS
#   include <inttypes.h>
#elif defined _MSC_VER && _MSC_VER < 1600
#   ifndef int32_t
typedef __int32 int32_t;
#   endif
#   ifndef uint16_t
typedef unsigned __int16 uint16_t;
#   endif
#   ifndef uint8_t
typedef unsigned __int8 uint8_t;
#   endif
#else
#   include <stdint.h>
#endif


/******************************************************************************/
/*  ABaDooRand errors.                                                        */
/*  ABaDooRand errors.                                                        */
/******************************************************************************/

/*  A number random enough not to collide with different errno ranges on      */
/*  A number random enough not to collide with different errno ranges on      */
/*  different OSes. The assumption is that error_t is at least 32-bit type.   */
/*  different OSes. The assumption is that error_t is at least 32-bit type.   */
#define ABADOORAND_HAUSNUMERO 225464856

/*  On Windows platform some of the standard POSIX errnos are not defined.    */
/*  On Windows platform some of the standard POSIX errnos are not defined.    */
#ifndef ENOTSUP
#define ENOTSUP (ABADOORAND_HAUSNUMERO + 1)
#endif
#ifndef EPROTONOSUPPORT
#define EPROTONOSUPPORT (ABADOORAND_HAUSNUMERO + 2)
#endif
#ifndef ENOBUFS
#define ENOBUFS (ABADOORAND_HAUSNUMERO + 3)
#endif
#ifndef ENETDOWN
#define ENETDOWN (ABADOORAND_HAUSNUMERO + 4)
#endif
#ifndef EADDRINUSE
#define EADDRINUSE (ABADOORAND_HAUSNUMERO + 5)
#endif
#ifndef EADDRNOTAVAIL
#define EADDRNOTAVAIL (ABADOORAND_HAUSNUMERO + 6)
#endif
#ifndef ECONNREFUSED
#define ECONNREFUSED (ABADOORAND_HAUSNUMERO + 7)
#endif
#ifndef EINPROGRESS
#define EINPROGRESS (ABADOORAND_HAUSNUMERO + 8)
#endif
#ifndef ENOTSOCK
#define ENOTSOCK (ABADOORAND_HAUSNUMERO + 9)
#endif
#ifndef EMSGSIZE
#define EMSGSIZE (ABADOORAND_HAUSNUMERO + 10)
#endif
#ifndef EAFNOSUPPORT
#define EAFNOSUPPORT (ABADOORAND_HAUSNUMERO + 11)
#endif
#ifndef ENETUNREACH
#define ENETUNREACH (ABADOORAND_HAUSNUMERO + 12)
#endif
#ifndef ECONNABORTED
#define ECONNABORTED (ABADOORAND_HAUSNUMERO + 13)
#endif
#ifndef ECONNRESET
#define ECONNRESET (ABADOORAND_HAUSNUMERO + 14)
#endif
#ifndef ENOTCONN
#define ENOTCONN (ABADOORAND_HAUSNUMERO + 15)
#endif
#ifndef ETIMEDOUT
#define ETIMEDOUT (ABADOORAND_HAUSNUMERO + 16)
#endif
#ifndef EHOSTUNREACH
#define EHOSTUNREACH (ABADOORAND_HAUSNUMERO + 17)
#endif
#ifndef ENETRESET
#define ENETRESET (ABADOORAND_HAUSNUMERO + 18)
#endif

/*  Native ABaDooRand error codes.                                              */
/*  Native ABaDooRand error codes.                                              */
//#define Exxx (ABADOORAND_HAUSNUMERO + 21)

// types
namespace abadoorand {

enum rng_type {pseudoRNG, trueRNG};

} /* namespace abadoorand */


#undef ABADOORAND_EXPORT

#ifdef __cplusplus
}
#endif

#endif

