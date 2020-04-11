#include "libdp3t_v2.h"
#include "cuckoo_filter.h"

#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <sys/stat.h>

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
   
#if 1
#define N_MET                      (20*1000)    // People I met on a given day
#define INFECTED                         (7)    // invected people I met in the last days.
#define FAKE_INFECTED   (1500*1000-INFECTED)    // list of infected people on the national list
#else
#define N_MET          (100)    // People I met on a given day
#define INFECTED         (6)    // invected people met
#define FAKE_INFECTED   (10)    // list of invecsted peopel
#endif
    printf("\ncreating infected patients\n");
    dp3t_eph_seed_t * invected_seeds = (dp3t_eph_seed_t*) malloc(sizeof(dp3t_eph_seed_t) * INFECTED);
    assert(invected_seeds);
    for(int i = 0; i < INFECTED; i++) {
        assert(DPT3T_OK == generate_eph_seed (&(invected_seeds[i])));
    }

    printf("\nLocal gathering\n");
    typedef struct localrec_t {
        uint8_t * hash;
        void *some_auxdata;
    } localrec_t;
    
    localrec_t * records = (localrec_t *)malloc( sizeof(localrec_t) * N_MET);
    assert(records);

    for(int i = 0; i < N_MET; i++) {
        dp3t_eph_t ephid_received; // as received
        
        if (i < INFECTED) {
            assert(DPT3T_OK == generate_eph_ephid (&(invected_seeds[i]), &ephid_received));
        } else {
            assert(DPT3T_OK == generate_fake_ephid(&ephid_received));
        };
        records[i].hash = malloc(EPHID_HASHLEN);
        assert(DPT3T_OK == populate_cfentry(records[i].hash,&ephid_received, t));
    };
    printf("\nSort to get random order for check\n");
    qsort(records, N_MET, sizeof(localrec_t), &hcmp);

    
    // these are all sent to the server.
    //
    // Periodically (e.g., every 4 hours), the backend creates a new Cuckoo filter F and for each pair (t, seed ) uploaded by an infected patients it inserts
    //
    size_t cfsize =(FAKE_INFECTED + INFECTED + 100) * 32;
    
    cuckoo_ctx_t * ctx = cuckoo_filter_init(cfsize);
    assert(ctx);
    
    printf("\nBackend - server takes the list of recived seeds and puts them into a filter\n");
    {
        // first add some invected people
        //
        uint8_t buff[EPHID_HASHLEN];
        for(int i = 0; i < INFECTED; i++) {
            dp3t_eph_t ephid;
            generate_eph_ephid(&(invected_seeds[i]), &ephid);
            
            assert(DPT3T_OK == populate_cfentry(buff,&ephid, t));
            if (cuckoo_filter_put(ctx, buff, EPHID_HASHLEN) != CUCKOO_OK) {
                printf("Error on stuff 1\n");
                break;
            }
            
            // printf("REMO %03d: ",999); print_hex(buff, EPHID_HASHLEN);
        }
        
        // now add a lot of 'cruft' to the central filter - as if others reported too.
        for(int i = 0; i < FAKE_INFECTED; i++) {
            uint8_t fake[EPHID_HASHLEN];
            assert(1 == RAND_bytes(fake, sizeof(fake)));
            if (cuckoo_filter_put(ctx, fake, EPHID_HASHLEN) != CUCKOO_OK) {
                printf("Failted to insert fake one at %d\n", i);
                break;
            }
            
        }
    };
    // show_hash_slots(ctx);
    size_t filelen = 0;
    assert(CUCKOO_OK == cuckoo_filter_serialize(ctx, NULL, &filelen));
    uint8_t * buff;
    assert(buff=malloc(filelen));
    assert(CUCKOO_OK == cuckoo_filter_serialize(ctx, buff, &filelen));
    FILE * out;
    assert( out = fopen("to-phone.cfbin","w"));
    assert(fwrite(buff,1, filelen, out) == filelen);
    assert(fclose(out) == 0);
    printf("written %lu byte file\n", filelen);
    
    free(buff);
    cuckoo_free(ctx);
    
    // now load this file on the client and check
    struct stat statbuf;
    assert(0 ==  stat("to-phone.cfbin", &statbuf));
    filelen = statbuf.st_size;
    assert(buff=malloc(filelen));

    FILE * in;
    assert( in = fopen("to-phone.cfbin","r"));
    assert(filelen == fread(buff,1,filelen,in));
    assert(fclose(in) == 0);
    printf("Transfered %lu byte filter to the phone\n", filelen);

    printf("Checking for contaminated patients on the phone\n");
    assert(ctx = cuckoo_filter_init_from_file(buff, filelen));
    
    // show_hash_slots(ctx);
    
    // Now check if we can find the needles in this haystack
    int contaminated = 0;
    for(int i = 0; i < N_MET; i++) {
        cuckoo_return_t hit = cuckoo_filter_exists(ctx, records[i].hash, EPHID_HASHLEN);
        switch(hit) {
            case CUCKOO_OK:
                contaminated++;
                break;
            case CUCKOO_NOTFOUND:
                break;
            default:
                printf("ERROR !");
        }
    };
    printf("Got %d (false positives %d) contaminated out of %d in my list and %d in the filter\n",contaminated, contaminated-INFECTED, N_MET, INFECTED + FAKE_INFECTED);
    // assert(contaminated == INFECTED);
    cuckoo_free(ctx);
}

