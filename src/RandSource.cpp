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

#include <new>
#include <string.h>
#include <chrono>

#include "RandSource.hpp"

namespace abadoorand {

thread_local std::set<size_t> RandSource::mReseedPeriods;
std::once_flag RandSource::isPrimesCTORed, RandSource::isPrimesDTORed;
primes* RandSource::mPrimes; // = NULL;
size_t RandSource::mQtRandSource = 0;


RandSource::RandSource(ABaDooRand* aAbadorand, const rng_type aRNGtype, const uint64_t aSeed, const size_t aReseedPeriodMax, char* aArgs)
    : mAbadorand(aAbadorand), mRNGtype(aRNGtype), mReseedPeriod(aReseedPeriodMax) //, mArgs(aArgs)
{
    mQtDraws = 0;
    std::call_once(isPrimesCTORed, [&]{ // &mPrimes, &mpDistribution
        mPrimes = new primes(BIGGEST_PRIME);
//        mQtRandSource = 0;
//        mpDistribution = new std::uniform_int_distribution<int>(0, 100);
    });

    get_args(&mOptions, aArgs, ",", "=", "");
    mQtRandSource++;
}

RandSource::~RandSource() {
    mQtRandSource--;
}

void RandSource::destroy_primes() {
        std::call_once(isPrimesDTORed, [&]{ // &mPrimes, &mpDistribution
            delete mPrimes;
//            delete mpDistribution;
        });
}

void RandSource::set_reseed_period(ABaDooRand* const aAbadorand) {
    size_t reseedPeriodMax = mReseedPeriod;
    if (reseedPeriodMax <= mPrimes->biggest()) {
        size_t imax = mPrimes->index_before(reseedPeriodMax);
        size_t imin = mPrimes->index_before(reseedPeriodMax / 2);
        while (true) {
//            mReseedPeriod = mPrimes->get(rd__() % (imax - imin) + imin);
            mReseedPeriod = mPrimes->get(aAbadorand->draw_uniform_int32(true) % (imax - imin) + imin);
            if (mReseedPeriods.count(mReseedPeriod) == 0) {
                mReseedPeriods.insert(mReseedPeriod);
                break;
            }
        }
    }
    else {
//        mReseedPeriod = rd__() % (reseedPeriodMax / 2) + reseedPeriodMax / 2;
        mReseedPeriod = aAbadorand->draw_uniform_int32(true) % (reseedPeriodMax / 2) + reseedPeriodMax / 2;
        if (mReseedPeriod % 2 == 0)
            mReseedPeriod++;
        while (true) {
            mReseedPeriod -= 2;
            if (mPrimes->is_obviously_not(mPrimes->biggest(), mReseedPeriod)) // probably not a prime
                continue; // re-draw
            bool isGood = true;
            for (auto period : mReseedPeriods)
                if (!(period % mReseedPeriod && mReseedPeriod % period)) {
                    isGood = false;
                    break;
                }
            if (isGood) {
                mReseedPeriods.insert(mReseedPeriod);
                break;
            }
        }
    }
}

uint32_t RandSource::draw_uniform_int32(bool withoutReseed) {
    uint32_t res;
    draw_uniform_int32(&res, 1, withoutReseed);
    return res;
}

uint64_t RandSource::draw_uniform_int64(bool withoutReseed) {
    uint64_t res;
    draw_uniform_int64(&res, 1, withoutReseed);
    return res;
}

int RandSource::draw_uniform_int32(uint32_t* aDest, size_t aSize, bool withoutReseed) {
    if (!aSize)
        return 0;
    if (withoutReseed)
        return raw_draw_uniform_int32(aDest, aSize);
    if (mQtDraws + aSize < mReseedPeriod) {
        mQtDraws += aSize;
        return raw_draw_uniform_int32(aDest, aSize);
    }
    else {
        uint32_t* dest = aDest;
        int rc = raw_draw_uniform_int32(aDest, mReseedPeriod - mQtDraws);
        dest = aDest + mReseedPeriod - mQtDraws;
        size_t sizeReminder = aSize - (mReseedPeriod - mQtDraws);
        add_entropy(mAbadorand->draw_uniform_int64(true));
        for (size_t i = 0; i < sizeReminder / mReseedPeriod; i++) {
            rc |= raw_draw_uniform_int32(dest , mReseedPeriod);
            if (rc)
                break;
            dest += mReseedPeriod;
            add_entropy(mAbadorand->draw_uniform_int64(true));
        }
        sizeReminder = sizeReminder % mReseedPeriod;
        if (!rc) {
            mQtDraws = sizeReminder;
            rc |= raw_draw_uniform_int32(dest , sizeReminder);
        }
    }
    return 0;
}

int RandSource::draw_uniform_int64(uint64_t* aDest, size_t aSize, bool withoutReseed) {
    if (!aSize)
        return 0;
    if (withoutReseed)
        return raw_draw_uniform_int64(aDest, aSize);
    if (mQtDraws + 2 * aSize < mReseedPeriod) {
        mQtDraws += 2 * aSize;
        return raw_draw_uniform_int64(aDest, aSize);
    }
    else {
        uint64_t* dest = aDest;
        int rc = raw_draw_uniform_int64(aDest, (mReseedPeriod - mQtDraws) / 2);
        dest = aDest + (mReseedPeriod - mQtDraws) / 2;
        size_t sizeReminder = aSize - (mReseedPeriod - mQtDraws) / 2;
        add_entropy(mAbadorand->draw_uniform_int64(true));
        for (size_t i = 0; i < sizeReminder / (mReseedPeriod / 2); i++) {
            rc |= raw_draw_uniform_int64(dest , mReseedPeriod / 2);
            if (rc)
                break;
            dest += mReseedPeriod / 2;
            add_entropy(mAbadorand->draw_uniform_int64(true));
        }
        sizeReminder = sizeReminder % (mReseedPeriod / 2);
        if (!rc) {
            mQtDraws = sizeReminder * 2;
            rc |= raw_draw_uniform_int64(dest , sizeReminder);
        }
    }
    return 0;
}

void RandSource::add_entropy(uint64_t aEntropy) {
    if (mRNGtype == trueRNG) {  // combines hardware source if present, plus two time sources: us and tics
        aEntropy ^= mClock.now_us() ^ rand64() ^ std::chrono::system_clock::now().time_since_epoch().count();
    }
    raw_add_entropy(aEntropy);
}

void RandSource::seed(uint64_t aSeed) {
    raw_seed (aSeed);
    mQtDraws = 0;
}

uint64_t RandSource::rand64() {
    return ((uint64_t) rd__() << 32 ) ^ rd__();
}

int RandSource::raw_draw_uniform_int32(uint32_t* aDest, size_t aSize) {
    for (size_t i = 0; i < aSize; i++) {
        aDest[i] = raw_draw_uniform_int32();
    }
    return 0;
}

int RandSource::raw_draw_uniform_int64(uint64_t* aDest, size_t aSize) {
    for (size_t i = 0; i < aSize; i++) {
        aDest[i] = raw_draw_uniform_int64();
    }
    return 0;
}

} /* namespace abadoorand */
