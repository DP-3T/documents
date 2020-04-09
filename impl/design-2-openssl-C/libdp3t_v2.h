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

#ifndef dp3t_v2_h
#define dp3t_v2_h

#include <stdio.h>
#include <stdint.h>

#define EPHID_SEED_LEN (32 /* bytes */)
#define EPHID_IDLEN (16 /* bytes */)
#define EPHID_HASHLEN (256/8)

typedef struct dp3t_eph_seed_t {
    uint8_t id[EPHID_SEED_LEN];
} dp3t_eph_seed_t;

typedef struct dp3t_eph_t {
    uint8_t id[EPHID_IDLEN];
} dp3t_eph_t;

typedef enum {
    DPT3T_OK,
    DPT3T_ERR,
} dp3t_err_t;

dp3t_err_t generate_eph_seed( dp3t_eph_seed_t * seed);
dp3t_err_t generate_eph_ephid( dp3t_eph_seed_t * seed, dp3t_eph_t * key);

dp3t_err_t populate_cfentry(uint8_t * buff, dp3t_eph_t * ephid, uint32_t t);

void print_hex(uint8_t * buff, size_t len);
void print_seed( dp3t_eph_seed_t * x);
void print_ephid( dp3t_eph_t * x);

#endif /* dp3t_core_h */

