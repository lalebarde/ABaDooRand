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

#ifndef SEEDS_HPP_
#define SEEDS_HPP_

//#include <stdint.h>
#include "../include/abadoorand.h"
//#include "ABaDooRand.hpp"

#define DEFAULT_RESEED_PERIOD_MAX 1000000

namespace abadoorand {

class ABaDooRand;

class Seeds {
friend class ABaDooRand;
public:
    Seeds(uint64_t* aSeeds, size_t aQtSeeds, size_t aReseedPeriodMax = DEFAULT_RESEED_PERIOD_MAX);
    Seeds(const uint64_t aSeed = 0, const size_t aReseedPeriodMax = DEFAULT_RESEED_PERIOD_MAX);
    ~Seeds();
    Seeds(const Seeds& rhs); // copy CTOR
    uint64_t& operator[](size_t idx);
    const uint64_t& operator[](size_t idx) const;
    const Seeds& operator = (const Seeds& rhs);
    size_t size() const {return mQtSeeds;}
    size_t reseed_period_max() const {return mReseedPeriodMax;}
public: // test
    int test_produce_seeds(const size_t aQtSeeds);
private:
    int produce_seeds(ABaDooRand* const aAbadorand, const size_t aQtSeeds);
    uint64_t* mSeeds;
    size_t mQtSeeds;
    size_t mReseedPeriodMax;
};

int seeds_test(int verbose);

} /* namespace abadoorand */
#endif /* SEEDS_HPP_ */
