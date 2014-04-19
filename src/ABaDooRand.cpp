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

#include <sodium.h>
#include <string.h>
//#include <chrono>

#include "../include/abadoorand.h"
#include "ABaDooRand.hpp"
#include "err.hpp"
#include "../plugins/RSrand.hpp"
#include "../plugins/RSgsl.hpp"
#include "primes.hpp"

namespace abadoorand {

ABaDooRand::ABaDooRand(const rng_type aRNGtype,  Seeds aSeeds, const char* aArgs)
         : mRNGtype(aRNGtype), mSeeds(aSeeds)
{
    if (!aArgs)
        aArgs = "gsl";
    get_args(&mRandSourcesArgs, aArgs, ";", "[", "]");

    mSeeds.produce_seeds(this, mRandSourcesArgs.size());

    RandSource* pRandSource = NULL;
    int i = 0;
    for (auto randSourceArgs : mRandSourcesArgs) {
        uint64_t seed = mSeeds[i++];
        if (randSourceArgs.first == "rand")
            pRandSource = new RS_rand(this, aRNGtype, seed, mSeeds.mReseedPeriodMax, (char*) randSourceArgs.second.c_str());
        else
        if (randSourceArgs.first == "gsl")
            pRandSource = new RS_gsl(this, aRNGtype, seed, mSeeds.mReseedPeriodMax, (char*) randSourceArgs.second.c_str());
        if (pRandSource) {
            mRandSources.push_back(pRandSource);
            if (aRNGtype == trueRNG)
                pRandSource->add_entropy(seed);
        }
        pRandSource = NULL;
    }

    for (auto randSource : mRandSources) { // set the reseed periods from ABaDooRand draws
        randSource->set_reseed_period(this);
    }

    if (mRNGtype == pseudoRNG)
        seed( mSeeds); // reseed because previous operations did consume draws

    if (mRandSources.size() == 0)
        abadoorand_assert("Error : No valid random source specified");
}

ABaDooRand::~ABaDooRand() {
    for (auto randSource : mRandSources)
        delete randSource;
}

//uint32_t ABaDooRand::draw_uniform_int32_gsl() {
//    return gsl_rng_uniform_int(mRand1, gsl_rng_max(mRand1));
//}
//
//uint32_t ABaDooRand::draw_uniform_int32_ec() {
//    return randombytes_uniform(0xFFFFFFFF);
//}

//uint32_t ABaDooRand::draw_uniform_int32(bool withoutReseed) {
//    // first draw with the gsl random generator
//    if (!withoutReseed && t1 > T1) {
//        unsigned long int seed = (sizeof (unsigned long int) == 4) ? draw_uniform_int32(true) : draw_uniform_int64(true);
//        if (mRNGtype == trueRNG)
//            seed ^= mClock.now_us();
//        gsl_rng_set (mRand1, seed);
//        t1 = 0;
//    }
//    uint32_t r1 = gsl_rng_uniform_int(mRand1, gsl_rng_max(mRand1));
//    t1++;
//
//    // second draw with the libsodium's elliptic curve random generator
//    if (t2 > T2) {
//        randombytes_stir();
//        t2 = 0;
//    }
//    uint32_t r2 = randombytes_uniform(0xFFFFFFFF);
//    t2++;
//
//    // return the combined random number
//    return r1 ^ r2;
//}

uint32_t ABaDooRand::draw_uniform_int32(bool withoutReseed) {
    uint32_t res = 0;
    for (auto randSource : mRandSources)
        res ^= randSource->draw_uniform_int32(withoutReseed);
    return res;
}

int ABaDooRand::draw_uniform_int32(uint32_t* aDest, size_t aSize, bool withoutReseed) {
    int res = 0;
    memset(aDest, 0, aSize * sizeof (uint32_t));
    uint32_t* temp = new uint32_t[aSize];
    for (auto randSource : mRandSources) {
        res ^= randSource->draw_uniform_int32(temp, aSize, withoutReseed);
        if (res)
            break;
        for (size_t i = 0; i < aSize; i++)
            aDest[i] ^= temp[i];
    }
    delete[] temp;
    return res;
}


//uint64_t ABaDooRand::draw_uniform_int64_gsl() {
//    uint64_t r1 = gsl_rng_uniform_int(mRand1, gsl_rng_max(mRand1));
//    if (sizeof (unsigned long int) == 4)
//        r1 =  (r1 << 32) | gsl_rng_uniform_int(mRand1, gsl_rng_max(mRand1));
//    return r1;
//}
//
//uint64_t ABaDooRand::draw_uniform_int64_ec() {
//    return ((uint64_t) randombytes_uniform(0xFFFFFFFF) << 32) | randombytes_uniform(0xFFFFFFFF);
//}

//uint64_t ABaDooRand::draw_uniform_int64(bool withoutReseed) {
//    // first draw with the gsl random generator
//    if (!withoutReseed && t1 > T1) {
//        unsigned long int seed = (sizeof (unsigned long int) == 4) ? draw_uniform_int32(true) : draw_uniform_int64(true);
//        if (mRNGtype == trueRNG)
//            seed ^= mClock.now_us();
//        gsl_rng_set (mRand1, seed);
//        t1 = 0;
//    }
//    uint64_t r1 = gsl_rng_uniform_int(mRand1, gsl_rng_max(mRand1));
//    t1++;
//    if (sizeof (unsigned long int) == 4) {
//        r1 =  (r1 << 32) | gsl_rng_uniform_int(mRand1, gsl_rng_max(mRand1));
//        t1++;
//    }
//
//    // second draw with the libsodium's elliptic curve random generator
//    if (t2 > T2) {
//        randombytes_stir();
//        t2 = 0;
//    }
//    uint64_t r2 = ((uint64_t) randombytes_uniform(0xFFFFFFFF) << 32) | randombytes_uniform(0xFFFFFFFF);
//    t2 += 2;
//
//    // return the combined random number
//    return r1 ^ r2;
//}

uint64_t ABaDooRand::draw_uniform_int64(bool withoutReseed) {
    uint64_t res = 0;
    for (auto randSource : mRandSources)
        res ^= randSource->draw_uniform_int64(withoutReseed);
    return res;
}

int ABaDooRand::draw_uniform_int64(uint64_t* aDest, size_t aSize, bool withoutReseed) {
    int res = 0;
    memset(aDest, 0, aSize * sizeof (uint64_t));
    uint64_t* temp = new uint64_t[aSize];
    for (auto randSource : mRandSources) {
        res ^= randSource->draw_uniform_int64(temp, aSize, withoutReseed);
        if (res)
            break;
        for (size_t i = 0; i < aSize; i++)
            aDest[i] ^= temp[i];
    }
    delete[] temp;
    return res;
}

void ABaDooRand::seed(Seeds aSeeds) {
    int i = 0;
    for (auto randSource : mRandSources) {
        uint64_t seed = mSeeds[i++];
        randSource->seed(seed);
    }
}

} /* namespace abadoorand */
