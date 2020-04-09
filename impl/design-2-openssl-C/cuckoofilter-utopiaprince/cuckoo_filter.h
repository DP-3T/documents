/*
 * Copyright (C) 2015, Leo Ma <begeekmyfriend@gmail.com>
 */

#ifndef _CUCKOO_FILTER_H_
#define _CUCKOO_FILTER_H_

//#define CUCKOO_DBG
#define CUCKOO_HASH_LEN (256/8)



int cuckoo_filter_init(size_t size);
uint8_t *cuckoo_filter_get(uint8_t *key);
int cuckoo_filter_put(uint8_t *key);

#endif /* _CUCKOO_FILTER_H_ */
