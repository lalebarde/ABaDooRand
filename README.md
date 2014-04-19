#<p align="center">
  <img src="https://github.com/lalebarde/ABaDooRand/blob/master/Logo/logo-ABaDooRand.png?raw=true" alt="ABaDooRand logo" width="234px"/>
#</p>

## Welcome

ABaDooRand is a library that produces anti-backdoor pseudo-random numbers, thus, cryptographically secure PRNG 
(CSPRNG) and RNG (CSRNG).
Since PRISM and Snowden 2013-2014 revelations, we by principle don't want 
to trust any library proposed for cryptography or random numbers, nor hardware random sources. The present library 
is an attempt to provide counter-measures against [possible backdoors](http://en.wikipedia.org/wiki/Dual_EC_DRBG). 
The principle is to use at least two independent wellknown open-source libraries and to xor numbers 
produced by each of them. Of course, this is obtained at the cost of more cpu, so a batch of ready numbers is provided 
for immediate fetch.

In addition, both engines are used to periodically re-seed the other with independent 
periods, one being a prime.

ABaDooRand provides 32 bits and 64 bits getters.

## Requirements

Follow the link [here](SRD.md)

## Status

At present time, we use full [gsl](http://www.csse.uwa.edu.au/programming/gsl-1.0/gsl-ref_16.html) random generators capabilities with the uniform distribution. 
Libraries are dynamically linked and shall be available on your system.

## To Do

- Add other distributions than uniform
- Add a batch to have a bag of ready random draws for tremendous performance
- Add the cryptographic random generator[libSodium](https://github.com/jedisct1/libsodium) CurveCP stream generator
- Add the cryptographic random generator [Fortuna](http://en.wikipedia.org/wiki/Fortuna_%28PRNG%29 )
- Add gsl-like API to be able to test ABaDooRand with [DieHarder](http://www.phy.duke.edu/~rgb/General/rand_rate.php)
- Provide performance comparisons
- Add reseed periods parametrization, and ressed (de)activation since some algorithms like Fortuna embeds this feature.
- Add Autotools and CMake features
- Add documentation
- Re-implement the libsodium generator to make it deterministic when "pseudo RNG" is choosen.
- Add a plugin abstraction mechanism for random sources. 


## Building and installation

TODO: Cf the INSTALL file included with the distribution. CMakeLists.txt, configure.ac and Makefile.am are in draft state.

## Use

Cf tests/ABaDooRand_test.cpp

## Resources

TODO: Documentation is provided with the distribution. Refer to
doc/abadoorand.html, or "man abadoorand" after you have installed ABaDooRand on your system.

Mailing list: abadoorand@freelists.org

Git repository: http://github.com/lalebarde/abadoorand

## Licensing

Cf [here](LICENSE.txt).

