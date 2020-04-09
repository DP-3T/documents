#include "libdp3t_v2.h"
#include "cuckoo_filter.h"

#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>

#include <openssl/sha.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/hmac.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/crypto.h>

#include <assert.h>
#include <strings.h>
#include <stdlib.h>
#include "cuckoo_filter.h"

#define TRIVIAL_CUCKOO_IMPLEMENTATION_256BIT_HASH (1)

dp3t_err_t generate_fake_ephid( dp3t_eph_t * ephid) {
    return (1 == RAND_bytes(ephid->id, sizeof(ephid->id))) ? DPT3T_OK : DPT3T_ERR;
}

int hcmp(const void *b1, const void *b2) {
    for(int i = 0; i < EPHID_HASHLEN; i+=4) {
        uint32_t a1 = (*(uint32_t**)b1)[i];
        uint32_t a2 = (*(uint32_t**)b2)[i];
        if (a1 > a2) return +1;
        if (a1 < a2) return -1;
    };
    return 0;
};

int main(int argc, char ** argv) {
    dp3t_eph_seed_t seed;
    assert(DPT3T_OK == generate_eph_seed (&seed));
    print_seed(&seed);
    
    dp3t_eph_t ephid;
    assert(DPT3T_OK == generate_eph_ephid (&seed, &ephid));
    print_ephid(&ephid);
    
    uint32_t t = 0;
    
#define N_MET          (50*1000)
#define INFECTED           (100)
#define FAKE_INFECTED (500*1000)

    printf("\ncreating infected patients\n");
    dp3t_eph_seed_t * invected_seeds = (dp3t_eph_seed_t*) malloc(sizeof(dp3t_eph_seed_t) * INFECTED);
    assert(invected_seeds);
    for(int i = 0; i < INFECTED; i++) {
        assert(DPT3T_OK == generate_eph_seed (&(invected_seeds[i])));
    }
    
    typedef struct localrec_t {
        uint8_t * hash;
        void *some_auxdata;
    } localrec_t;
    localrec_t * records = (localrec_t *)malloc( sizeof(localrec_t) * N_MET);
    assert(records);
    
    
    printf("\nLocal gathering\n");
    
    for(int i = 0; i < N_MET; i++) {
        dp3t_eph_t ephid_received; // as received
        
        if (i < INFECTED) {
            assert(DPT3T_OK == generate_eph_ephid (&(invected_seeds[i]), &ephid_received));
        } else {
            assert(DPT3T_OK == generate_fake_ephid(&ephid_received));
        };
        records[i].hash = malloc(EPHID_HASHLEN);
        assert(DPT3T_OK == populate_cfentry(records[i].hash,&ephid_received, t));
        if (i<10) { printf("%s %03d: ",i == N_MET/2 ? "---->" :"local", i); print_hex(records[i].hash, EPHID_HASHLEN);};
    };
    
    // Periodically (e.g., every 4 hours), the backend creates a new Cuckoo filter F and for each pair (t, seed ) uploaded by an infected patients it inserts
        
    size_t cfsize =(FAKE_INFECTED + INFECTED + 100) * 32;
    printf("Cuckoo filter block of %d Mb - tables is %d Mb\n",
           (int)(0.5 + cfsize / 1024 / 1024),
           (int)(0.5 + ((cfsize / CUCKOO_HASH_LEN) / 4 + (cfsize / CUCKOO_HASH_LEN) / 4 /4 ) * 32) / 1024 / 1024);
           
    assert(0==cuckoo_filter_init(cfsize));
    
    printf("\nBackend gathering\n");
    {
        // first add some invected people
        //
        uint8_t buff[EPHID_HASHLEN];
        for(int i = 0; i < INFECTED; i++) {
            dp3t_eph_t ephid;
            generate_eph_ephid(&(invected_seeds[i]), &ephid);
            
            assert(DPT3T_OK == populate_cfentry(buff,&ephid, t));
            if (cuckoo_filter_put(buff)) {
                printf("Error on stuff 1\n");
                break;
            }
            
            // printf("REMO %03d: ",999); print_hex(buff, EPHID_HASHLEN);
        }
        
        // now add a lot of 'cruft' to the central filter - as if others reported too.
        for(int i = 0; i < FAKE_INFECTED; i++) {
            uint8_t fake[EPHID_HASHLEN];
            assert(1 == RAND_bytes(fake, sizeof(fake)));
            if (i < 10) {printf("remo %03d: ",i); print_hex(fake, sizeof(fake));};
            if (cuckoo_filter_put(fake)) {
                printf("Error on stuff 2 at %d\n", i);
                break;
            }
            
        }
    };
    
    printf("\nSort to get random order for check\n");
    qsort(records, N_MET, sizeof(localrec_t), &hcmp);
    
    printf("\nChecking for contaminated patients\n");
    // Now check if we can find the needle in this haystack
    int contaminated = 0;
    for(int i = 0; i < N_MET; i++) {
        uint8_t * val = cuckoo_filter_get(records[i].hash);
        if (val) contaminated++;
        if (i < 10) printf("%s %03d: ",val ? "found" : "nope ", i);
        if (i < 10) print_hex(records[i].hash, EPHID_HASHLEN);
    };
    printf("Got %d contaminated out of %d in my list and %d in the filter\n",contaminated, N_MET, INFECTED + FAKE_INFECTED);
    assert(contaminated == INFECTED);
    
}

