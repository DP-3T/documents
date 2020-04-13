# DP3-T Implementation profile 
Against version 2020/4/8 of the whitepaper

## Design 2

### General

Byte sequences are 8 bit octed strings.

In both designs - the CDN's webserver its Date: header set (RFC 2616, section 14.18) can be used, during the daily fetch, as the authoritative time specifier.

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

### Cuckoo filter and serialisation version 1.00

(Aligned with https://github.com/dirkx/DP-3T-Documents/tree/editable-version/impl/design-2-openssl-C)

The key shall be the hash as resulting from 2020/4/12 version of the Whitepaper: H(TRUNCATE128(H(seed))||i). No further hasing is required.

Note - hash propably not needed - less than 20% exposed.

The key shall be 32 bytes which are used as follows:

	byte  0..3		up to <bits-hash bits for the LSB of the Cuckoo hash
	byte  4..7		up to <bits-hash bits for the MSB of the Cuckoo hash
	byte  8..		up to `<bits-verify>' bytes.
		
Where <bits-hash> and <bits-verify> are set as low as is feasible given the acceptable false positive rates.

The Cuckoo filter shall be serialised as:

    Magic string 	4	bytes D3, D3, 3D, 3D
    Version			1	byte, major/minor, currently set to 0x10 for version 1.0
    <Depth>			1	byte, fixed to 4
    <bits-hash>		1	byte, without the occupy bit.
    <bits-verify>	1	byte
    <N buckets>		4	bytes, unsigned 32 bit integer, network order
    <N slots>		4	bytes, unsigned 32 bit integer, network order
  
Followed by

    <N buckets> with each
    	<Depth> slots with each
    		sequence of ( bit-hash + 1 )/ 8 rounded up bytes:	
	    		is occupied		1 			bit
    			partial hash	bits-hash	bits
    			padding			*			any padding bits set to 0 if bits-hash +1 not a multiple of 8
       		verify hash			bits-verify	bits
    			padding			*			any padding bits set to 0 if bits-verify +1 not a multiple of 8
 
With the partial hash being limited to the number of bits needed for N buckets.  

So this, 1.0, version of the serialisation does not pack the bits; both hashes are padded to the a full byte.
 
### Cuckoo filter publication

The filter should be published prefixed by a RFC3161 timestamp. 



## Design 1

The PRF used is HMAC-SHA256 as per RFC 6234 and RFC 2104 - and and where Skt_ is used as the `key’ and the string  “Decentralized Privacy-Preserving Proximity Tracing” (without trailing \0, i.e. exactly those 50 US-ASCII characters is the plaintext.


Test vectors:

	SK: 
		0000000000000000000000000000000000000000000000000000000000000000
	SK derivation:
		66687aadf862bd776c8fc18b8e9f8e20089714856ee233b3902a591d0d5f2925


The PRG used is AES128 in counter mode; with the IV set to a 128 bit unsigned number in network order (i.e the first IV is a byte array if [  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 ]) we start at 0, not 1 and the plaintext 128 bits of 0’s.

	IV: 00000000000000000000000000000000        
	eph: 0 - Ephemeral(day:0, token:c7044845a6a0da7a61687e1bb08afca4)
 
	IV: 00000000000000000000000000000001
	eph: 1 - Ephemeral(day:0, token:a747e729bf2e3de3ec6ecbdb0f889f5b)

	IV: 00000000000000000000000000000002
	eph: 2 - Ephemeral(day:0, token:034015608c5a55672315cb614f5a94a3)

