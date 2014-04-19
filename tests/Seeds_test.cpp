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

#include "../src/Seeds.hpp"
#include "../src/err.hpp"

namespace abadoorand {

int seeds_test(int verbose) {
    if (verbose)
        printf ("Test class Seeds\n");

    Seeds s0;
    if (s0.size() != 1)
        abadoorand_assert("Error : Seeds s0 has not the expected size\n");
    if (s0[0] != 0)
        abadoorand_assert("Error : Seeds s0 has not been initialized correctly\n");
    if (s0.reseed_period_max() != DEFAULT_RESEED_PERIOD_MAX)
        abadoorand_assert("Error : Seeds s0 has not the expected ReseedPeriodMax\n");

    Seeds s1(123, 7);
    if (s1.size() != 1)
        abadoorand_assert("Error : Seeds s1 has not the expected size\n");
    if (s1[0] != 123)
        abadoorand_assert("Error : Seeds s1 has not been initialized correctly\n");
    if (s1.reseed_period_max() != 7)
        abadoorand_assert("Error : Seeds s1 has not the expected ReseedPeriodMax\n");

    uint64_t seeds[] = {1, 23, 345, 0, 4567, 888777666555444333};
    Seeds s2(seeds, 6, 99);
    if (s2.size() != 6)
        abadoorand_assert("Error : Seeds s2 has not the expected size\n");
    for (size_t i = 0; i < 6; i++)
        if (s2[i] != seeds[i])
            abadoorand_assert("Error : Seeds s2 has not been initialized correctly\n");
    if (s2.reseed_period_max() != 99)
        abadoorand_assert("Error : Seeds s2 has not the expected ReseedPeriodMax\n");

    s2.test_produce_seeds(10);
    if (s2.size() != 10)
        abadoorand_assert("Error : Seeds s2 has not the expected size\n");
    if (s2.reseed_period_max() != 99)
        abadoorand_assert("Error : Seeds s2 has not the expected ReseedPeriodMax\n");

    return 0;
}

} /* namespace abadoorand */
