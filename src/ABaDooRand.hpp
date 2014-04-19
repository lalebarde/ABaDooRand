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

#ifndef ABADOORAND_HPP_
#define ABADOORAND_HPP_

#include <gsl/gsl_randist.h> // http://www.csse.uwa.edu.au/programming/gsl-1.0/gsl-ref_16.html
#include <stdint.h>
#include <list>
#include <map>

#include "../include/abadoorand.h"
#include "../include/utils.h"
#include "RandSource.hpp"
#include "clock.hpp"
#include "Seeds.hpp"

namespace abadoorand {

class RandSource;

class ABaDooRand {
public:
    //ABaDooRand(rng_type aRNGtype = pseudoRNG, uint64_t aSeed = 0, gsl_rng_type * aGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_taus));
    ABaDooRand(const rng_type aRNGtype = pseudoRNG, Seeds aSeeds = Seeds(), const char* aArgs = NULL);
    ~ABaDooRand();
    uint32_t draw_uniform_int32(bool withoutReseed = false);
    uint64_t draw_uniform_int64(bool withoutReseed = false);
    int draw_uniform_int32(uint32_t* aDest, size_t aSize, bool withoutReseed = false);
    int draw_uniform_int64(uint64_t* aDest, size_t aSize, bool withoutReseed = false);
    void seed(Seeds aSeeds = Seeds()); // used for PRNG only
    rng_type get_rng_type() {return mRNGtype;}
    Seeds& get_seeds() {return mSeeds;}
public: // performance tests only
//    uint32_t draw_uniform_int32_gsl();
//    uint32_t draw_uniform_int32_ec();
//    uint64_t draw_uniform_int64_gsl();
//    uint64_t draw_uniform_int64_ec();
private: // CTOR
    const rng_type mRNGtype;
    std::list<RandSource*> mRandSources;
private: // RandSource list with their arguments
    multimap_args_t mRandSourcesArgs;
private:
    gsl_rng* mRand1;
    clock_t mClock;
private: // seeds
    bool hasOwnSeeds;
    Seeds mSeeds;
};

} /* namespace abadoorand */
#endif /* ABADOORAND_HPP_ */
