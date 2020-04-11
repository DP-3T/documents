# Design 2 - test

Simple implémentation of:

1)	Design 2 from  https://github.com/DP-3T/documents/blob/master/DP3T%20White%20Paper.pdf  

2) 	With key params taken from https://github.com/dirkx/DP-3T-Documents/blob/implementation-profile-start/implementation-profiles/profile.md


### Typical sizing:

* Contact moments in relevant period (e.g. 14 days): 5-20k 
* Size of the infected in past 14 day list: 0.5 - 5 Million
* Number of contacts that match those infected: 0 - 10

### Typical results

Resulting file size: 8 - 40 Mbyte

Resulting scan times: < 0.1 second on an iPhone 5.

This assumes that the false positives are kept lower than 1:1000 for actual individual match.

The Cuckoo Filter; if it where not hashed itself would at those numbers disclose around 1/3 of the bits of the 256Bit hash.


