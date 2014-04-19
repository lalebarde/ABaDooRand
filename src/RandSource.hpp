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

#ifndef RANDSOURCE_HPP_
#define RANDSOURCE_HPP_

#include <set>
#include <map>
#include <string>
#include <stdint.h>
#include <mutex>          // std::call_once, std::once_flag

#ifdef CX11
#include <random>
#define rd__(x) m_rd(x)
#else
#define rd__(x) rand(x)
#endif

#include "../include/abadoorand.h"
#include "../include/utils.h"
#include "ABaDooRand.hpp"
#include "primes.hpp"
#include "clock.hpp"

#define BIGGEST_PRIME 1000000

namespace abadoorand {

// Virtual Class for random sources

class ABaDooRand;

class RandSource {
public: // API
    RandSource(ABaDooRand* aAbadorand, const rng_type aRNGtype = pseudoRNG, const uint64_t aSeed = 0, const size_t aReseedPeriodMax = 1000000, char* aArgs = NULL);
    virtual ~RandSource();
    virtual uint32_t draw_uniform_int32(bool withoutReseed = false); // SHALL not be overloaded
    virtual uint64_t draw_uniform_int64(bool withoutReseed = false); // SHALL not be overloaded
    virtual int draw_uniform_int32(uint32_t* aDest, size_t aSize, bool withoutReseed = false); // SHALL not be overloaded
    virtual int draw_uniform_int64(uint64_t* aDest, size_t aSize, bool withoutReseed = false); // SHALL not be overloaded
    virtual void add_entropy(uint64_t aEntropy = 0); // SHALL not be overloaded
    virtual void seed(uint64_t aSeed = 0); // SHALL not be overloaded
    void set_reseed_period(ABaDooRand* const aAbadorand);
    static void destroy_primes();
    bool isCryptoSecure = false; // to be set by derived classes if necessary
    bool isPseudoRNGcapable = false; // to be set by derived classes if necessary
protected:
    virtual uint32_t raw_draw_uniform_int32() = 0;
    virtual uint64_t raw_draw_uniform_int64() = 0;
    virtual int raw_draw_uniform_int32(uint32_t* aDest, size_t aSize); // the default, if not overloaded, is a loop with use of "uint32_t raw_draw_uniform_int32()"
    virtual int raw_draw_uniform_int64(uint64_t* aDest, size_t aSize); // the default, if not overloaded, is a loop with use of "uint64_t raw_draw_uniform_int64()"
    virtual void raw_add_entropy(uint64_t aEntropy = 0) = 0; // some random sources may not accept external entropy
    virtual void raw_seed(uint64_t aSeed = 0) = 0; // used for PRNG only
    ABaDooRand* mAbadorand; // used to access our meta random source
    rng_type mRNGtype; // pseudoRNG or trueRNG
    size_t mReseedPeriod;
    static thread_local std::set<size_t> mReseedPeriods; // http://stackoverflow.com/questions/13106049/c11-gcc-4-8-thread-local-performance-penalty
    size_t mQtDraws; // compared to mReseedPeriod
    static std::once_flag isPrimesCTORed, isPrimesDTORed; // thread safe alchimy
    static primes* mPrimes; // for every threads, used to set mReseedPeriod - http://stackoverflow.com/questions/1270927/are-function-static-variables-thread-safe-in-gcc
protected: // options
//    char* mArgs;
    map_args_t mOptions;
private:
    #ifdef CX11
    // for random_device, no static nor thread_local, but what's the best solution ?
    std::random_device m_rd; // 32 bits true random from hw    http://www.cplusplus.com/reference/random/random_device/random_device/    http://www.cplusplus.com/reference/random/random_device/operator%28%29/
    #endif
//    std::uniform_int_distribution<int>* mpDistribution;
//    std::mt19937 generator (seed);
    uint64_t rand64();
    static clock_t mClock;
    static size_t mQtRandSource;
};

} /* namespace abadoorand */
#endif /* RANDSOURCE_HPP_ */
