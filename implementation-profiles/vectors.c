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

#include <strings.h>
#include <stdlib.h>
#include <time.h>


void printhex(uint8_t * hashed_seed, size_t len) {
   for(int i = 0; i < len; i++) 
      printf("%02x",hashed_seed[i]);
   printf(" (size %lu)\n", len);
};

int main(int argc, char ** argv) {
  SHA256_CTX sha256;

  uint8_t seed[ 32 ];
  bzero(seed,sizeof(seed));

  printf("Seed:\t\t"); printhex(seed,sizeof(seed));

  uint8_t hashed_seed[32];
  SHA256_Init(&sha256);
  SHA256_Update(&sha256, seed, 32);
  SHA256_Final(hashed_seed,  &sha256);
  printf("H(Seed):\t\t"); printhex(hashed_seed, 32);

  printf("TRUNCATE128(H(Seed)):\t"); printhex(hashed_seed, 128 / 8 );

  struct tm ts = { 
	.tm_sec = 0, .tm_min = 0, .tm_hour = 0, 
	.tm_mon = 3, .tm_year = 120, 
	.tm_wday = 5, 
	.tm_isdst = 0, .tm_gmtoff = 0
  };

  time_t t = timegm(&ts);
  printf("Time:\t\t%lu\n", t);
  uint8_t tbuff[4];
  *(uint32_t *)tbuff= htonl(t);

  printf("t:\t\t"); printhex(tbuff,4);

  uint8_t ephid_concat_t[16 + 4];
  bcopy(hashed_seed, ephid_concat_t +  0, 16);
  bcopy(tbuff       ,ephid_concat_t + 16, 4);

  printf("ephid||t:\t\t"); printhex(ephid_concat_t, 16+4);

  uint8_t hash[32];
  SHA256_Init(&sha256);
  SHA256_Update(&sha256, ephid_concat_t, 16 + 4);
  SHA256_Final(hash,  &sha256);
  printf("H(E || t)):\t"); printhex(hash, 32);

}
