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

#include "clock.hpp"
#include "platform.hpp"
#include "likely.hpp"
//#include "config.hpp"
#include "err.hpp"
#include "mutex.hpp"

#include <stddef.h>

#if defined _MSC_VER
#if defined _WIN32_WCE
#include <cmnintrin.h>
#else
#include <intrin.h>
#endif
#endif

#if !defined ABADOORAND_HAVE_WINDOWS
#include <sys/time.h>
#endif

#if defined HAVE_CLOCK_GETTIME || defined HAVE_GETHRTIME
#include <time.h>
#endif

#ifdef ABADOORAND_HAVE_WINDOWS
typedef ULONGLONG (*f_compatible_get_tick_count64)();

static abadoorand::mutex_t compatible_get_tick_count64_mutex;

ULONGLONG compatible_get_tick_count64()
{
  compatible_get_tick_count64_mutex.lock();
  static DWORD s_wrap = 0;
  static DWORD s_last_tick = 0;
  const DWORD current_tick = ::GetTickCount();
  if (current_tick < s_last_tick)
    ++s_wrap;

  s_last_tick = current_tick;
  const ULONGLONG result = (static_cast<ULONGLONG>(s_wrap) << 32) + static_cast<ULONGLONG>(current_tick);
  compatible_get_tick_count64_mutex.unlock();
  return result;
}

f_compatible_get_tick_count64 init_compatible_get_tick_count64()
{
  f_compatible_get_tick_count64 func = NULL;
  HMODULE module = ::LoadLibraryA("Kernel32.dll");
  if (module != NULL)
    func = reinterpret_cast<f_compatible_get_tick_count64>(::GetProcAddress(module, "GetTickCount64"));

  if (func == NULL)
    func = compatible_get_tick_count64;

  return func;
}

static f_compatible_get_tick_count64 my_get_tick_count64 = init_compatible_get_tick_count64();
#endif

abadoorand::clock_t::clock_t () :
    last_tsc (rdtsc ()),
#ifdef ABADOORAND_HAVE_WINDOWS
    last_time (static_cast<uint64_t>((*my_get_tick_count64)()))
#else
    last_time (now_us () / 1000)
#endif
{
}

abadoorand::clock_t::~clock_t ()
{
}

uint64_t abadoorand::clock_t::now_us ()
{
#if defined ABADOORAND_HAVE_WINDOWS

    //  Get the high resolution counter's accuracy.
    LARGE_INTEGER ticksPerSecond;
    QueryPerformanceFrequency (&ticksPerSecond);

    //  What time is it?
    LARGE_INTEGER tick;
    QueryPerformanceCounter (&tick);

    //  Convert the tick number into the number of seconds
    //  since the system was started.
    double ticks_div = ticksPerSecond.QuadPart / 1000000.0;
    return (uint64_t) (tick.QuadPart / ticks_div);

#elif defined HAVE_CLOCK_GETTIME && defined CLOCK_MONOTONIC

    //  Use POSIX clock_gettime function to get precise monotonic time.
    struct timespec tv;
    int rc = clock_gettime (CLOCK_MONOTONIC, &tv);
		// Fix case where system has clock_gettime but CLOCK_MONOTONIC is not supported.
		// This should be a configuration check, but I looked into it and writing an
		// AC_FUNC_CLOCK_MONOTONIC seems beyond my powers.
		if( rc != 0) {
			//  Use POSIX gettimeofday function to get precise time.
			struct timeval tv;
			int rc = gettimeofday (&tv, NULL);
			errno_assert (rc == 0);
			return (tv.tv_sec * (uint64_t) 1000000 + tv.tv_usec);
		}
    return (tv.tv_sec * (uint64_t) 1000000 + tv.tv_nsec / 1000);

#elif defined HAVE_GETHRTIME

    return (gethrtime () / 1000);

#else

    //  Use POSIX gettimeofday function to get precise time.
    struct timeval tv;
    int rc = gettimeofday (&tv, NULL);
    errno_assert (rc == 0);
    return (tv.tv_sec * (uint64_t) 1000000 + tv.tv_usec);

#endif
}

uint64_t abadoorand::clock_t::now_ms ()
{
    uint64_t tsc = rdtsc ();

    //  If TSC is not supported, get precise time and chop off the microseconds.
    if (!tsc)
    {
#ifdef ABADOORAND_HAVE_WINDOWS
        // Under Windows, now_us is not so reliable since QueryPerformanceCounter
        // does not guarantee that it will use a hardware that offers a monotonic timer.
        // So, lets use GetTickCount when GetTickCount64 is not available with an workaround
        // to its 32 bit limitation.
        return static_cast<uint64_t>((*my_get_tick_count64)());
#else
        return now_us () / 1000;
#endif
    }

    //  If TSC haven't jumped back (in case of migration to a different
    //  CPU core) and if not too much time elapsed since last measurement,
    //  we can return cached time value.
    if (likely (tsc - last_tsc <= (clock_precision / 2) && tsc >= last_tsc))
        return last_time;

    last_tsc = tsc;
#ifdef ABADOORAND_HAVE_WINDOWS
    last_time = static_cast<uint64_t>((*my_get_tick_count64)());
#else
    last_time = now_us () / 1000;
#endif
    return last_time;
}

uint64_t abadoorand::clock_t::rdtsc ()
{
#if (defined _MSC_VER && (defined _M_IX86 || defined _M_X64))
    return __rdtsc ();
#elif (defined __GNUC__ && (defined __i386__ || defined __x86_64__))
    uint32_t low, high;
    __asm__ volatile ("rdtsc" : "=a" (low), "=d" (high));
    return (uint64_t) high << 32 | low;
#elif (defined __SUNPRO_CC && (__SUNPRO_CC >= 0x5100) && (defined __i386 || \
    defined __amd64 || defined __x86_64))
    union {
        uint64_t u64val;
        uint32_t u32val [2];
    } tsc;
    asm("rdtsc" : "=a" (tsc.u32val [0]), "=d" (tsc.u32val [1]));
    return tsc.u64val;
#elif defined(__s390__)
    uint64_t tsc;
    asm("\tstck\t%0\n" : "=Q" (tsc) : : "cc");
    return(tsc);
#else
    return 0;
#endif
}

//abadoorand::time_stamp_t::time_stamp_t () :
//    time (now_us ())
//    {
//    }

//abadoorand::time_stamp_t::time_stamp_t (const time_stamp_t& rhs) :
//    time (rhs.time)
//    {
//    }

//abadoorand::time_stamp_t::time_stamp_t (uint64_t time_) :
//    time (time_)
//    {
//    }

bool abadoorand::time_stamp_t::operator< (const time_stamp_t& rhs) const {
    if (time < rhs.time) {
        if (rhs.time - time < coherance)
            return true;
        else // 0 between
            return false;
    }
    else
    if (time > rhs.time) {
        if (time - rhs.time < coherance)
            return false;
        else // 0 between
            return true;
    }
    else
        return false; // equal
}

abadoorand::time_stamp_t& abadoorand::time_stamp_t::operator-= (const abadoorand::time_stamp_t& rhs) {
    if (time < rhs.time) // we assume we pass by 0
        time += (~(uint64_t)0) - rhs.time;
    else
    if (time >= rhs.time)
        time -= rhs.time;
    return *this;
}

//abadoorand::time_stamp_t abadoorand::time_stamp_t::operator- (const abadoorand::time_stamp_t& rhs) const {
//    //return abadoorand::time_stamp_t(time - rhs.time);
//    return abadoorand::time_stamp_t(*this) -= rhs;
//}

