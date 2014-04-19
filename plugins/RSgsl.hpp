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

#ifndef RSGSL_HPP_
#define RSGSL_HPP_

#include <gsl/gsl_randist.h> // http://www.csse.uwa.edu.au/programming/gsl-1.0/gsl-ref_16.html

#include "../src/RandSource.hpp"

namespace abadoorand {

class RS_gsl: public abadoorand::RandSource {
public:
    RS_gsl(ABaDooRand* aAbadorand, const rng_type aRNGtype = pseudoRNG, const uint64_t aSeed = 0, const size_t aReseedPeriodMax = 1000000, char* aArgs = NULL);
    virtual ~RS_gsl();
    uint32_t raw_draw_uniform_int32();
    uint64_t raw_draw_uniform_int64();
    void raw_add_entropy(uint64_t aEntropy = 0); // some random sources may not accept external entropy
    void raw_seed(uint64_t aSeed = 0); // used for PRNG only
private:
    gsl_rng_type * mGslRangeType;
    gsl_rng* mGslRand;
    unsigned long int mGslRandMax;
};

} /* namespace abadoorand */
#endif /* RSGSL_HPP_ */
