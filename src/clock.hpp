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

#ifndef __ABADOORAND_CLOCK_HPP_INCLUDED__
#define __ABADOORAND_CLOCK_HPP_INCLUDED__

#include "stdint.hpp"

namespace abadoorand
{
//  Low-precision clock precision in CPU ticks. 1ms. Value of 1000000
//  should be OK for CPU frequencies above 1GHz. If should work
//  reasonably well for CPU frequencies above 500MHz. For lower CPU
//  frequencies you may consider lowering this value to get best
//  possible latencies.
    const int clock_precision = 1000000;


    class clock_t
    {
    public:

        clock_t ();
        ~clock_t ();

        //  CPU's timestamp counter. Returns 0 if it's not available.
        static uint64_t rdtsc ();

        //  High precision timestamp.
        static uint64_t now_us ();

        //  Low precision timestamp. In tight loops generating it can be
        //  10 to 100 times faster than the high precision timestamp.
        uint64_t now_ms ();

    private:

        //  TSC timestamp of when last time measurement was made.
        uint64_t last_tsc;

        //  Physical time corresponding to the TSC above (in milliseconds).
        uint64_t last_time;

        clock_t (const clock_t&);
        const clock_t &operator = (const clock_t&);
    };

    class time_stamp_t
    {
    public:
        time_stamp_t() : time (now_us ()) {};
        time_stamp_t(const time_stamp_t& rhs) : time (rhs.time) {};
        time_stamp_t(uint64_t time_) : time (time_) {};
        inline uint64_t get () {return time;}
        static inline uint64_t now_us () {return clock_t::now_us();}
        inline void set_to_now () {time = clock_t::now_us();}
        bool operator< (const time_stamp_t& rhs) const;
        time_stamp_t& operator-= (const time_stamp_t& rhs);
        time_stamp_t operator- (const time_stamp_t& rhs) const {return time_stamp_t(*this) -= rhs;};
        static const uint64_t coherance = 1000000llu * 3600 * 24 * 365 * 1000; // 1000 year in us - 2^64/1000000/3600/24/365 = 584000 years in us

    private:
        uint64_t time;
    };

}

#endif
