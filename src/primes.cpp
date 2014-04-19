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

/******************************************************************************
The sieve of Eratosthenes to determine prime numbers
The algorithm
1 Create a contiguous list of numbers from two to some integer n.
2 Strike out from the list all multiples of two (4, 6, 8 etc.).
3 The list's next number that has not been struck out is a prime number.
4 Strike out from the list all multiples of the number you identified in the
  previous step.
5 Repeat steps 3 and 4 until you reach n.
http://www.cplusplus.com/forum/beginner/11654/
******************************************************************************/
//#include <string>
//#include <cstdlib>
//#include <stdio.h>
#include <string.h>
#include <cassert>
//#include <iostream>
#include <math.h>
//#include <unistd.h>
//#include <locale>
//#include <cstddef>

#include "primes.hpp"

using namespace std;

primes::primes(size_t aN) : mN(aN) {
	mApproxQtPrimes = 1.2 * mN / log(mN);
//	cout << "Build primes table[<" << mN << "] of approximative size = " << mApproxQtPrimes << "....." << endl;
    mPrimes = new size_t[mApproxQtPrimes];
    mIsPrime = new byte[mN/8+1]; memset(mIsPrime, 0, mN/8+1);
    mQt = sieve();
    mPrimeBiggest = mPrimes[mQt-1];
//    cout << "Found " << mQt << " prime numbers" << endl;
//    cout << "The largest prime number is " << mPrimeBiggest << endl;
    size_t a, b;
    for (size_t i = 0; i < mQt; i++) {
    	a = mPrimes[i] / 8;
    	b = mPrimes[i] % 8;
    	mIsPrime[a] |= 1 << b;
    }
    mQtEvals = 0;
}

primes::~primes() {
    delete[] mPrimes;
    delete[] mIsPrime;
}


bool primes::is(size_t aVal) {
	//if (aVal > mPrimeBiggest) return false;
	assert(aVal <= mPrimeBiggest);
	mQtEvals++;
    size_t a = aVal / 8;
    size_t b = aVal % 8;
	return (mIsPrime[a] & 1 << b);
}

size_t primes::index_after(size_t aVal) {
    return (index_before(aVal) + 1);
}

size_t primes::index_before(size_t aVal) {
    if (aVal <= 2)
        return 0;
    assert(aVal <= mPrimeBiggest);
    size_t rankEstimUpper = 1.2 * aVal / log(aVal);
    while (rankEstimUpper > mQt || mPrimes[rankEstimUpper] > aVal)
        rankEstimUpper--;
    return (rankEstimUpper);
}



size_t primes::sieve() {
    if (mN<2) return 0;
    const char blank = 0;
    const char marked = 1;
    char* theSieve;
    theSieve = new char[mN+1];
    for (size_t k=2; k<=mN; k++) theSieve[k] = blank;
    size_t idx = 0;

    for (size_t k=2; k<=mN; k++) {
        if (theSieve[k]==blank) {
            theSieve[k] = marked;
            mPrimes[idx] = k;
            idx++;
            for(size_t d=2*k; d<=mN; d+=k) theSieve[d] = marked;
        }
    }
    delete[] theSieve;
    return idx;
}

bool primes::is_obviously_not(size_t aUpperBoundPrime, size_t aVal) {
    if (aVal <= mPrimeBiggest && is(aVal))
        return false;
	// test divisibility with primes up to DMAX
	size_t rsq = sqrt(aVal);
	for (size_t k = 0; k < aUpperBoundPrime ; k++) {
		if (k >= mQt) {
			return false;
		}
		size_t p = mPrimes[k];
		if (p > aUpperBoundPrime) {
			return false;
		}
		if (aVal % p == 0) return true;
		if (p > rsq) return false; // we have explored up to the sqrt of aVal
	}
	return false;
}

