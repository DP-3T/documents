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
} cuckoo_return_t;

#define VERSION 0xD3000100 // 0xD300 / 01.00 minor/major version.
#
typedef struct {
    uint32_t version;
    uint32_t depth;
    uint32_t slot_num;
    uint32_t bucket_num;
} cuckoo_file_hdr;


cuckoo_ctx_t * cuckoo_filter_init(size_t size);

cuckoo_ctx_t * cuckoo_filter_init_from_file(uint8_t * inbuff, size_t leninbuf);
cuckoo_return_t cuckoo_filter_serialize(cuckoo_ctx_t * ctx, uint8_t * outbuffOrNull, size_t * lenoutbuf);

cuckoo_return_t cuckoo_filter_exists(cuckoo_ctx_t *, uint8_t *key);

uint8_t *cuckoo_filter_get(cuckoo_ctx_t * ctx, uint8_t *key);
cuckoo_return_t cuckoo_filter_put(cuckoo_ctx_t *, uint8_t *key);


cuckoo_return_t cuckoo_free(cuckoo_ctx_t *);

void show_hash_slots(cuckoo_ctx_t * ctx);

#endif /* _CUCKOO_FILTER_H_ */
