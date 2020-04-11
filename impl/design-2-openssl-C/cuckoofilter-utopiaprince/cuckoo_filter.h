/*
 * Copyright (C) 2015, Leo Ma <begeekmyfriend@gmail.com>
 */

#ifndef _CUCKOO_FILTER_H_
#define _CUCKOO_FILTER_H_

//#define CUCKOO_DBG
#define CUCKOO_HASH_LEN (256/8)

typedef struct cuckoo_ctx_rec cuckoo_ctx_t;

typedef enum {
    CUCKOO_OK = 0,
    CUCKOO_NOTFOUND,
    CUCKOO_COLLISION,
    CUCKOO_ERR,
} cuckoo_return_t;

#define CUCKOO_MAGIC 0xD3D33D3D // 0xD300 / 1.00 minor/major version.
#define CUCKOO_MAJOR 1
#define CUCKOO_MINOR 1

typedef struct {
    uint32_t magic;
    uint8_t major_version;
    uint8_t minor_version;
    uint8_t depth;
    uint8_t limit_verify;
    uint32_t bucket_num;
    uint32_t slot_num;
} cuckoo_file_hdr;


cuckoo_ctx_t * cuckoo_filter_init(size_t size);

cuckoo_ctx_t * cuckoo_filter_init_from_file(uint8_t * inbuff, size_t leninbuf);
cuckoo_return_t cuckoo_filter_serialize(cuckoo_ctx_t * ctx, uint8_t * outbuffOrNull, size_t * lenoutbuf);

cuckoo_return_t cuckoo_filter_exists(cuckoo_ctx_t *, uint8_t *key, size_t keyle);
uint8_t *cuckoo_filter_get(cuckoo_ctx_t * ctx, uint8_t *key, size_t keylen);
cuckoo_return_t cuckoo_filter_put(cuckoo_ctx_t *, uint8_t *key, size_t keylen);


cuckoo_return_t cuckoo_free(cuckoo_ctx_t *);

void show_hash_slots(cuckoo_ctx_t * ctx);

#endif /* _CUCKOO_FILTER_H_ */
