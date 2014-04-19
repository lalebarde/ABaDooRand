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

#include <cstring>
#include "Seeds.hpp"
#include "ABaDooRand.hpp"

namespace abadoorand {

Seeds::Seeds(uint64_t* aSeeds, size_t aQtSeeds, size_t aReseedPeriodMax)
    : mQtSeeds(aQtSeeds), mReseedPeriodMax(aReseedPeriodMax)
{
    if (aSeeds && aQtSeeds) {
        mSeeds = new uint64_t[aQtSeeds];
        memcpy(mSeeds, aSeeds, aQtSeeds * sizeof(uint64_t));
    }
    else {
        mSeeds = new uint64_t[1];
        mSeeds[0] = 0;
    }
}

Seeds::Seeds(const uint64_t aSeed, const size_t aReseedPeriodMax)
    : mQtSeeds(1), mReseedPeriodMax(aReseedPeriodMax)
{
    mSeeds = new uint64_t[1];
    mSeeds[0] = aSeed;
}

Seeds::~Seeds() {
    delete [] mSeeds;
}

Seeds::Seeds(const Seeds& rhs)
    : mQtSeeds(rhs.mQtSeeds), mReseedPeriodMax(rhs.mReseedPeriodMax)
{
    mSeeds = new uint64_t[mQtSeeds];
    memcpy(mSeeds, rhs.mSeeds, mQtSeeds * sizeof(uint64_t));
}


uint64_t& Seeds::operator[](size_t idx) {
    return mSeeds[idx];
}

const uint64_t& Seeds::operator[](size_t idx) const {
    return mSeeds[idx];
}

const Seeds& Seeds::operator = (const Seeds& rhs) {
    mQtSeeds = rhs.mQtSeeds;
    mReseedPeriodMax = rhs.mReseedPeriodMax;
    mSeeds = new uint64_t[mQtSeeds];
    memcpy(mSeeds, rhs.mSeeds, mQtSeeds * sizeof(uint64_t));
    return *this;
}


int Seeds::produce_seeds(ABaDooRand* const aAbadorand, const size_t aQtSeeds) {
    if (aQtSeeds <= mQtSeeds)
        return 0;
    uint64_t *newSeeds = new uint64_t[aQtSeeds];
    ABaDooRand r(aAbadorand->get_rng_type(), aAbadorand->get_seeds(), NULL); // does not need to produce seeds itself
    int rc = r.draw_uniform_int64(newSeeds, aQtSeeds);
    if (rc == 0) {
        for (size_t i = 0; i < aQtSeeds; i++)
            newSeeds[i] ^= mSeeds[i % mQtSeeds];
        delete [] mSeeds;
        mSeeds = newSeeds;
        mQtSeeds = aQtSeeds;
    }
    else {
        delete [] newSeeds;
    }
    return rc;
}

int Seeds::test_produce_seeds(const size_t aQtSeeds) {
    ABaDooRand r;
    return produce_seeds (&r, aQtSeeds);
}


} /* namespace abadoorand */
