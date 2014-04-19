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

#ifndef PRIMES_HPP_
#define PRIMES_HPP_

typedef unsigned char byte;

typedef size_t big_int;

class primes {
public:
	size_t* mPrimes;
    primes(size_t aN);
    ~primes();
    bool is(size_t aVal);
    size_t biggest() {return mPrimeBiggest;}
    size_t size() {return mQt;}
    size_t get(size_t aI) {return mPrimes[aI];}
    size_t index_after(size_t aVal); // returns the index of the prime just after the value
    size_t index_before(size_t aVal); // returns the index of the prime just before the value
    //void set(size_t aI, size_t aVal) {mPrimes[aI] = aVal;}
    void reset_eval_counter() {mQtEvals = 0;}
    bool is_obviously_not(size_t aUpperBoundPrime, size_t aVal);
    size_t qt_evals() {return mQtEvals;}
private:
    size_t mN; // range
    byte* mIsPrime;
    size_t mApproxQtPrimes;
    size_t mQtEvals;
    size_t mPrimeBiggest;
    size_t mQt; // qt of primes in the range
    size_t sieve();
};


#endif /* PRIMES_H_ */
