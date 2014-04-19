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

#include "../src/err.hpp"
#include "RSgsl.hpp"

namespace abadoorand {

RS_gsl::RS_gsl(ABaDooRand* aAbadorand, const rng_type aRNGtype, const uint64_t aSeed, const size_t aReseedPeriodMax, char* aArgs)
    : RandSource(aAbadorand, aRNGtype, aSeed, aReseedPeriodMax, aArgs)
{
    if (mOptions.count("gsl_rng_type") == 0) {
        mOptions["gsl_rng_type"] = "gsl_rng_taus"; // default options values
        mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_taus);
    }
    else {
        std::string& gslRangeTypeStr = mOptions["gsl_rng_type"];
        if (gslRangeTypeStr == "gsl_rng_borosh13") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_borosh13);
        else if (gslRangeTypeStr == "gsl_rng_coveyou") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_coveyou);
        else if (gslRangeTypeStr == "gsl_rng_cmrg") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_cmrg);
        else if (gslRangeTypeStr == "gsl_rng_fishman18") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_fishman18);
        else if (gslRangeTypeStr == "gsl_rng_fishman20") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_fishman20);
        else if (gslRangeTypeStr == "gsl_rng_fishman2x") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_fishman2x);
        else if (gslRangeTypeStr == "gsl_rng_gfsr4") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_gfsr4);
        else if (gslRangeTypeStr == "gsl_rng_knuthran") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_knuthran);
        else if (gslRangeTypeStr == "gsl_rng_knuthran2") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_knuthran2);
        else if (gslRangeTypeStr == "gsl_rng_knuthran2002") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_knuthran2002);
        else if (gslRangeTypeStr == "gsl_rng_lecuyer21") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_lecuyer21);
        else if (gslRangeTypeStr == "gsl_rng_minstd") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_minstd);
        else if (gslRangeTypeStr == "gsl_rng_mrg") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_mrg);
        else if (gslRangeTypeStr == "gsl_rng_mt19937") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_mt19937);
        else if (gslRangeTypeStr == "gsl_rng_mt19937_1999") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_mt19937_1999);
        else if (gslRangeTypeStr == "gsl_rng_mt19937_1998") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_mt19937_1998);
        else if (gslRangeTypeStr == "gsl_rng_r250") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_r250);
        else if (gslRangeTypeStr == "gsl_rng_ran0") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_ran0);
        else if (gslRangeTypeStr == "gsl_rng_ran1") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_ran1);
        else if (gslRangeTypeStr == "gsl_rng_ran2") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_ran2);
        else if (gslRangeTypeStr == "gsl_rng_ran3") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_ran3);
        else if (gslRangeTypeStr == "gsl_rng_rand") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_rand);
        else if (gslRangeTypeStr == "gsl_rng_rand48") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_rand48);
        else if (gslRangeTypeStr == "gsl_rng_random128_bsd") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_random128_bsd);
        else if (gslRangeTypeStr == "gsl_rng_random128_glibc2") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_random128_glibc2);
        else if (gslRangeTypeStr == "gsl_rng_random128_libc5") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_random128_libc5);
        else if (gslRangeTypeStr == "gsl_rng_random256_bsd") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_random256_bsd);
        else if (gslRangeTypeStr == "gsl_rng_random256_glibc2") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_random256_glibc2);
        else if (gslRangeTypeStr == "gsl_rng_random256_libc5") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_random256_libc5);
        else if (gslRangeTypeStr == "gsl_rng_random32_bsd") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_random32_bsd);
        else if (gslRangeTypeStr == "gsl_rng_random32_glibc2") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_random32_glibc2);
        else if (gslRangeTypeStr == "gsl_rng_random32_libc5") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_random32_libc5);
        else if (gslRangeTypeStr == "gsl_rng_random64_bsd") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_random64_bsd);
        else if (gslRangeTypeStr == "gsl_rng_random64_glibc2") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_random64_glibc2);
        else if (gslRangeTypeStr == "gsl_rng_random64_libc5") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_random64_libc5);
        else if (gslRangeTypeStr == "gsl_rng_random8_bsd") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_random8_bsd);
        else if (gslRangeTypeStr == "gsl_rng_random8_glibc2") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_random8_glibc2);
        else if (gslRangeTypeStr == "gsl_rng_random8_libc5") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_random8_libc5);
        else if (gslRangeTypeStr == "gsl_rng_random_bsd") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_random_bsd);
        else if (gslRangeTypeStr == "gsl_rng_random_glibc2") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_random_glibc2);
        else if (gslRangeTypeStr == "gsl_rng_random_libc5") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_random_libc5);
        else if (gslRangeTypeStr == "gsl_rng_randu") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_randu);
        else if (gslRangeTypeStr == "gsl_rng_ranf") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_ranf);
        else if (gslRangeTypeStr == "gsl_rng_ranlux") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_ranlux);
        else if (gslRangeTypeStr == "gsl_rng_ranlux389") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_ranlux389);
        else if (gslRangeTypeStr == "gsl_rng_ranlxd1") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_ranlxd1);
        else if (gslRangeTypeStr == "gsl_rng_ranlxd2") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_ranlxd2);
        else if (gslRangeTypeStr == "gsl_rng_ranlxs0") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_ranlxs0);
        else if (gslRangeTypeStr == "gsl_rng_ranlxs1") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_ranlxs1);
        else if (gslRangeTypeStr == "gsl_rng_ranlxs2") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_ranlxs2);
        else if (gslRangeTypeStr == "gsl_rng_ranmar") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_ranmar);
        else if (gslRangeTypeStr == "gsl_rng_slatec") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_slatec);
        else if (gslRangeTypeStr == "gsl_rng_taus") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_taus);
        else if (gslRangeTypeStr == "gsl_rng_taus2") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_taus2);
        else if (gslRangeTypeStr == "gsl_rng_taus113") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_taus113);
        else if (gslRangeTypeStr == "gsl_rng_transputer") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_transputer);
        else if (gslRangeTypeStr == "gsl_rng_tt800") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_tt800);
        else if (gslRangeTypeStr == "gsl_rng_uni") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_uni);
        else if (gslRangeTypeStr == "gsl_rng_uni32") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_uni32);
        else if (gslRangeTypeStr == "gsl_rng_vax") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_vax);
        else if (gslRangeTypeStr == "gsl_rng_waterman14") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_waterman14);
        else if (gslRangeTypeStr == "gsl_rng_zuf") mGslRangeType = const_cast<gsl_rng_type *>(gsl_rng_zuf);
        else abadoorand_assert("Error: bad gsl_rng type");
    }
    mGslRand = gsl_rng_alloc (mGslRangeType);
    gsl_rng_set (mGslRand, aSeed); // seed - http://www.csse.uwa.edu.au/programming/gsl-1.0/gsl-ref_16.html
    mGslRandMax = gsl_rng_max(mGslRand);
    isPseudoRNGcapable = true;
}

RS_gsl::~RS_gsl() {
    gsl_rng_free (mGslRand);
}

uint32_t RS_gsl::raw_draw_uniform_int32() {
    return gsl_rng_uniform_int(mGslRand, mGslRandMax);
}

uint64_t RS_gsl::raw_draw_uniform_int64() {
    uint64_t r1 = gsl_rng_uniform_int(mGslRand, mGslRandMax);
    if (sizeof (unsigned long int) == 4)
        r1 =  (r1 << 32) | gsl_rng_uniform_int(mGslRand, mGslRandMax);
    return r1;
}

void RS_gsl::raw_add_entropy(uint64_t aEntropy) {
    gsl_rng_set (mGslRand, aEntropy);
}

void RS_gsl::raw_seed(uint64_t aSeed) {
    gsl_rng_set (mGslRand, aSeed);
}

} /* namespace abadoorand */
