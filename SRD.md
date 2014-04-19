# Software Requirements Document

## Functional Requirements

| # | Requirement | Status |
| --:| ---------------- |:---------:|
| 100 | ABaDooRand SHALL combine at least two independant random sources i.e. coming from two different libraries and with different algorithms. | OK |
| 110 | The combination of the different random sources SHALL not degrade the entropy of each one. | OK |
| 120 | The different random sources SHALL be seeded by a combination of a draw of a different source. | OK |
| 130 | For RNG, seeds SHALL be combined with an external entropy source. | OK |
| 140 | For PRNG, using the same seed SHALL provide the same draws. | OK |
| 150 | Re-seeds SHALL be performed independently and cyclicaly for all sources that requires it. | OK |
| 160 | Re-seed periods SHALL not have common divisors. | OK |
| 170 | For some random sources like [Fortuna](https://www.schneier.com/fortuna.html), the "re-seed" operation SHALL be replaced by a "add entropy" operation. | - |
| 180 | An abstraction layer SHALL enable to integrate new random sources with no code re-work. | OK |
| 190 | A batch of random draws SHALL be available for immediate draw, in order to circumvent the cpu overhead of ABaDooRand. | - |
| 200 | The batch SHALL be filled in a dedicated thread activated when it is half empty, and stopped when it is full. | - |

## Test Requirements

| # | Requirement | Status |
| --:| ---------------- |:---------:|
| 700 | For test, performance evaluation, and tests, ABaDooRand SHALL provide 32 bits and 64 bits getters for each random sources. | - |
| 710 | ABaDooRand SHALL provide a 32 bits and 64 bits rough test to check that at least on 10,000 draws, there is less than 10% of collisions. | OK |
| 720 | ABaDooRand SHALL provide a performance test comparison for 32 bits and 64 bits sources and the ABaDooRand combined one. | - |

## Interface Control Requirements

| # | Requirement | Status |
| --:| ---------------- |:---------:|
| 800 | ABaDooRand SHALL provide 32 bits and 64 bits getters. | OK |
| 810 | ABaDooRand SHALL have an initialiser, but it SHALL not be mandatory to call it. | OK |
| 820 | The ABaDooRand initialiser SHALL permit to choose the different random sources and their numbers. | OK |
| 830 | The ABaDooRand initialiser SHALL permit to choose between PRNG and RNG. | OK |
| 840 | The ABaDooRand initialiser SHALL permit to choose the initial seed, either one value, either an array of the size equal to the number of sources that need it. | OK |
| 850 | The ABaDooRand initialiser SHALL permit to choose the re-seed periods of all random sources. | - |
| 860 | The ABaDooRand initialiser SHALL permit to choose the batch size. | - |
| 870 | Makes ABaDooRand callable by [DieHarder](http://www.phy.duke.edu/~rgb/General/rand_rate.php)


## Implementation Requirements

| # | Requirement | Status |
| --:| ---------------- |:---------:|
| 900 | Language SHALL be C++ | OK |
| 910 | The combination of the different random sources is the bitwise XOR operation. | OK |
| 920 | The ABaDooRand library SHALL be able to be built under Linux >= 2.6 | OK |
| 930 | The ABaDooRand library SHALL be able to be used under Linux >= 2.6, Windows XP, 7, Wine-HQ, *BSD | - |

## Requirements on requirements

| # | Requirement | Status |
| --:| ---------------- |:---------:|
| 10 | Requirements numbers, when assigned, SHALL never be changed | OK |
|20 | Status SHALL be one of the following: `OK` Released, `I` Implemented but not enough tested, `-` Not yet implemented, `NA` Not Applicable or abandonned. | OK |
|30 | Requirement numbers 0 to 99 are reserved for *Requirements on requirements*, 100 to 699 for *Functional Requirements*, 700 to 799 to *Test Requirements*, 800 to 899 to *Interface Control Requirements*, 900 to 999 for *Implementation Requirements* | OK |
|40 | Requirements SHALL remain in a text file in the root of the project, so that we have a clear status of what is implemented for each version. | OK |

TODO: It may be interresting to add rationals for each requirement (when it is useful) in a separate file with links from the SRD.


