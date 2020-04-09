/* Copyright © 2020 Dirk-Willem van Gulik. All rights reserved.
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include "libdp3t_v2.h"

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

dp3t_err_t generate_eph_seed( dp3t_eph_seed_t * seed) {
    return (1 == RAND_bytes(seed->id, sizeof(seed->id))) ? DPT3T_OK : DPT3T_ERR;
}

dp3t_err_t generate_eph_ephid(dp3t_eph_seed_t * seed, dp3t_eph_t * key) {
    uint8_t buff[SHA256_DIGEST_LENGTH];
    
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, seed->id,EPHID_SEED_LEN);
    SHA256_Final(buff,  &sha256);
    // https://github.com/DP-3T/documents/issues/93
    memcpy(key->id, buff, EPHID_IDLEN);
    
    return DPT3T_OK;
}

dp3t_err_t populate_cfentry(uint8_t * buff, dp3t_eph_t * ephid, uint32_t t) {
    assert(buff);
    
    uint8_t hashed_identity[EPHID_IDLEN + sizeof(t)];
    
    memcpy(hashed_identity,ephid->id,EPHID_IDLEN);
    
    uint32_t t_wire = htonl(t);
    memcpy(hashed_identity+EPHID_IDLEN,&t_wire,sizeof(t_wire));
    
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, hashed_identity,sizeof(hashed_identity));

    assert(SHA256_DIGEST_LENGTH == EPHID_HASHLEN);
    SHA256_Final(buff,  &sha256);

    return DPT3T_OK;
};


void print_hex(uint8_t * buff, size_t len) {
    for(int i = 0; i < len; i++)
        printf("%02x", buff[i]);
    printf("\n");
};

void print_seed( dp3t_eph_seed_t * x) {
    printf("SKT: "); print_hex(x->id, EPHID_SEED_LEN);
}

void print_ephid( dp3t_eph_t * x) {
    printf("EPH: "); print_hex(x->id, EPHID_IDLEN);
}
