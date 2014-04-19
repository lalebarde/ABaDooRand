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

#include <stdio.h>
#include <set>
#include <new>
#include <cstring>

#include "../src/clock.hpp"

#define QT_NUMBERS 10000

#include "../include/utils.h"
#include "../src/Seeds.hpp"
#include "../src/ABaDooRand.hpp"
#include "../src/RandSource.hpp" // RandSource::destroy_primes();

int main (int argc, char *argv[]) {
    int verbose = (argc > 1 && argv[1][0] == '1');
    if (verbose)
        printf("abadoorand test\n");

    // test utils
    int rcu = test_utils(verbose);

    // test Seeds
    int rcs = abadoorand::seeds_test(verbose);


    // initialise some seeds
    uint64_t seed1234[] = {1234};


    // rough functional test and performance evaluation
    abadoorand::ABaDooRand r(abadoorand::pseudoRNG, abadoorand::Seeds(seed1234, 1, 100), "rand");
    std::set<uint64_t> s;
    abadoorand::time_stamp_t t;

    // 32 bits draw test
    for (int i  = 0; i < QT_NUMBERS; i++) {
            s.insert(r.draw_uniform_int32());
    }
    uint64_t duree = t.now_us() - t.get();
    int rc32 = s.size() > QT_NUMBERS * 0.9 ? 0 : -1;
    s.clear();
//    t.set_to_now();
//    for (int i  = 0; i < QT_NUMBERS; i++) {
//            s.insert(r.draw_uniform_int32_ec());
//    }
//    uint64_t duree_ec = t.now_us() - t.get();
//    t.set_to_now();
//    for (int i  = 0; i < QT_NUMBERS; i++) {
//            s.insert(r.draw_uniform_int32_gsl());
//    }
//    uint64_t duree_gsl = t.now_us() - t.get();
//    if (verbose) {
//        printf ("\"unsigned long int\" are %d bits long\n", 8 * (int) sizeof (unsigned long int));
//        printf ("32 bit draws: %s\n", rc32 ? "KO" : "OK");
//        printf ("duration for 1M draws: %d us for gsl, x%1.2f for elliptic curve, x%1.2f for ABaDoorRand\n",
//                (int) (duree_gsl * 1000000 / QT_NUMBERS), (double) duree_ec / duree_gsl, (double) duree / duree_gsl);
//    }

    // 64 bits draw test
    t.set_to_now();
    for (int i  = 0; i < QT_NUMBERS; i++) {
            s.insert(r.draw_uniform_int64());
    }
    duree = t.now_us() - t.get();
    int rc64 = s.size() > QT_NUMBERS * 0.9 ? 0 : -1;
    s.clear();
//    t.set_to_now();
//    for (int i  = 0; i < QT_NUMBERS; i++) {
//            s.insert(r.draw_uniform_int64_ec());
//    }
//    duree_ec = t.now_us() - t.get();
//    t.set_to_now();
//    for (int i  = 0; i < QT_NUMBERS; i++) {
//            s.insert(r.draw_uniform_int64_gsl());
//    }
//    duree_gsl = t.now_us() - t.get();
//    if (verbose) {
//        printf ("64 bit draws: %s\n", rc64 ? "KO" : "OK");
//        printf ("duration for 1M draws: %d us for gsl, x%1.2f for elliptic curve, x%1.2f for ABaDoorRand\n",
//                (int) (duree_gsl * 1000000 / QT_NUMBERS), (double) duree_ec / duree_gsl, (double) duree / duree_gsl);
//    }

    // PRNG test with default seed
    abadoorand::ABaDooRand* r1 = new abadoorand::ABaDooRand; // default seed = 0
    abadoorand::ABaDooRand* r2 = new abadoorand::ABaDooRand;
    int rcPRNG0 = 0;
    for (int i = 0; i < QT_NUMBERS; i++)
        if (r1->draw_uniform_int32() != r2->draw_uniform_int32() || r1->draw_uniform_int64() != r2->draw_uniform_int64()) {
            rcPRNG0 = -1;
            break;
        }
    if (verbose)
        printf ("PRNG test with default seed: %s\n", rcPRNG0 ? "KO" : "OK");
    delete r1;
    delete r2;

    // PRNG test with given seed
    r1 = new abadoorand::ABaDooRand(abadoorand::pseudoRNG, abadoorand::Seeds(seed1234, 1)); // seed = 1234
    r2 = new abadoorand::ABaDooRand(abadoorand::pseudoRNG, abadoorand::Seeds(1234)); // seed = 1234
    int rcPRNG1 = 0;
    for (int i = 0; i < QT_NUMBERS; i++)
        if (r1->draw_uniform_int32() != r2->draw_uniform_int32() || r1->draw_uniform_int64() != r2->draw_uniform_int64()) {
            rcPRNG1 = -1;
            break;
        }
    if (verbose)
        printf ("PRNG test with given seed: %s\n", rcPRNG1 ? "KO" : "OK");
    delete r1;
    delete r2;

    // PRNG with re-seed
    r1 = new abadoorand::ABaDooRand(abadoorand::pseudoRNG, 1234); // seed = 1234
    uint32_t draw32_1[QT_NUMBERS];
    r1->draw_uniform_int32(draw32_1, QT_NUMBERS);
    uint32_t draw32_2[QT_NUMBERS];
    r1->seed(1234);
    r1->draw_uniform_int32(draw32_2, QT_NUMBERS);
    int rcPRNG2 = memcmp(draw32_1, draw32_2, sizeof(draw32_1));

    uint64_t draw64_1[QT_NUMBERS];
    r1->seed(1234);
    r1->draw_uniform_int64(draw64_1, QT_NUMBERS);
    uint64_t draw64_2[QT_NUMBERS];
    r1->seed(1234);
    r1->draw_uniform_int64(draw64_2, QT_NUMBERS);
    rcPRNG2 |= memcmp(draw64_1, draw64_2, sizeof(draw64_1));

    if (verbose)
        printf ("PRNG test with re-seeds: %s\n", rcPRNG2 ? "KO" : "OK");
    delete r1;

    // PRNG with different seeds
    r1 = new abadoorand::ABaDooRand(abadoorand::pseudoRNG, abadoorand::Seeds(1234)); // seed = 1234
    r2 = new abadoorand::ABaDooRand(abadoorand::pseudoRNG, abadoorand::Seeds(4321)); // seed = 4321
    size_t qtCollisions = 0;
    for (int i = 0; i < QT_NUMBERS; i++)
        if (r1->draw_uniform_int32() == r2->draw_uniform_int32() || r1->draw_uniform_int64() == r2->draw_uniform_int64())
            qtCollisions++;
    int rcPRNG3 = (qtCollisions > 0.01 * QT_NUMBERS) ? -1 : 0;
    if (verbose)
        printf ("PRNG test with different seeds: %s\n", rcPRNG3 ? "KO" : "OK");
    delete r1;
    delete r2;

    // PRNG with a seeds set and 3 sources
    uint64_t seeds_set[] = {123456789101112, 2222233333, 33221100};
    abadoorand::Seeds seedsSet(seeds_set, 3, 100);
    r1 = new abadoorand::ABaDooRand(abadoorand::pseudoRNG, seedsSet,
            "gsl [gsl_rng_type = gsl_rng_taus]; rand; gsl [gsl_rng_type = gsl_rng_mt19937]");
    r1->draw_uniform_int32(draw32_1, QT_NUMBERS);
    r1->seed(seedsSet);
    r1->draw_uniform_int32(draw32_2, QT_NUMBERS);
    int rcPRNG4 = memcmp(draw32_1, draw32_2, sizeof(draw32_1));
    dump(draw32_1, QT_NUMBERS, "/tmp/draw32_1");
    dump(draw32_2, QT_NUMBERS, "/tmp/draw32_2");

    r1->seed(seedsSet);
    r1->draw_uniform_int64(draw64_1, QT_NUMBERS);
    r1->seed(seedsSet);
    r1->draw_uniform_int64(draw64_2, QT_NUMBERS);
    rcPRNG4 |= memcmp(draw64_1, draw64_2, sizeof(draw64_1));

    if (verbose)
        printf ("PRNG test with seeds set and 3 sources: %s\n", rcPRNG4 ? "KO" : "OK");
    delete r1;

    int rcPRNG = rcPRNG0 || rcPRNG1 || rcPRNG2 || rcPRNG3 || rcPRNG4;

    // true RNG test
    r1 = new abadoorand::ABaDooRand(abadoorand::trueRNG);
    r2 = new abadoorand::ABaDooRand(abadoorand::trueRNG);
    qtCollisions = 0;
    for (int i = 0; i < QT_NUMBERS; i++)
        if (r1->draw_uniform_int32() == r2->draw_uniform_int32() || r1->draw_uniform_int64() == r2->draw_uniform_int64())
            qtCollisions++;
    int rcTRNG = (qtCollisions > 0.01 * QT_NUMBERS) ? -1 : 0;
    if (verbose)
        printf ("True RNG test: %s\n", rcTRNG ? "KO" : "OK");
    delete r1;
    delete r2;

    abadoorand::RandSource::destroy_primes();

    return rcu || rcs || rc32 || rc64 || rcPRNG || rcTRNG;
}

