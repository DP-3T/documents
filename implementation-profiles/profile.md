# DP3-T Implementation profile 
Against version 2020/4/8 of the whitepaper

## Design 2

### General

Byte sequences are 8 bit octed strings.

### Generating Empheral IDs

The H is an SHA256 as per per RFC 6234

TRUNKCATE128() takes the first 32 bytes (of the 64 byte SHA256)

Test vector:
 
    Seed:     0000000000000000000000000000000000000000000000000000000000000000 
    			(i.e. 0x00, 0x00 .. 0x00 32 bytes)
    H (seed): 66687aadf862bd776c8fc18b8e9f8e20089714856ee233b3902a591d0d5f2925
    TRUNKCATE128(H(seed)): 
              66687aadf862bd776c8fc18b8e9f8e20

### Local storare / handling of ‘t’

‘t’ is a network order (big endian) unsigned 32 bit number. I.e. the number 1 is encoded transmitted as 0x00, 0x00, 0x00, 0x01 on the wire.

‘t’ contains the unix UTC/Z timestamp as defined by RFC 3339.

So the H(EphID||t) stored is a SHA256 taken over 16 + 4 = 20 sequentiel bytes in that order (EphID, then time).

Test vector:

	Time: 2020-4-10 00:00:00 UTC
	T = 1586476800 
	        5E8FB700 (4 bytes)
	EphID || t = 
	        66687aadf862bd776c8fc18b8e9f8e201586476800) (16+4 butes)
	H(EphID || t)
	        109708e29597623f56fd365ba92f1c717ca23994aabd7939822909c465cb10a5 (32 bytes)

### Cuckoo filter and serialisation

The depth of the Cuckoo filter shall be 4.

The Cuckoo filter shall be serialised as:

-	Depth:			unsigned 32 bit integer (A)
-   	Number of slots: 	unsigned 32 bit integer (S)
- 	Number of buckets:	unsigned 32 bit integer (B)
-	Buckets B x ( A x slotsID)
-		with the slotID an unsigned 32 bit integer. 
-	Slots(numbered 0 .. slotsID) 	S x ( key )
-		with the key a 31 bit  unsigned int;
-		the topbit denotes a populated (0) or empty (1) slot.

### Cuckoo filter publication

The filter should be published prefixed by an RFC3161 timestamp. 




## Design 1

The PRF used is HMAC-SHA256 as per RFC 6234 and RFC 2104 - and and where Skt_ is used as the `key’ and the string  “broadcast key” (without trailing \0, i.e. exactly those 13 US-ASCII characters is the plaintext.

The PRG used is AES128 in counter mode; with the IV set to a 128 bit unsigned number in network order (i.e the first IV is a byte array if [  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 ]) we start at 0, not 1

and the plaintext 128 bits of 0’s.



