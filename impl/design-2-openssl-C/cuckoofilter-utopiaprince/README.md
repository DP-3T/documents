Cuckoo Filter
=============

This used to be:

	A key-value filter using cuckoo hashing, substituting for bloom filter.

and the master version still lives at:

	https://github.com/begeekmyfriend/CuckooFilter.git

But it was heavily modified for different bit lengths & is now a simple key store; no values.

Usage
-----

```c
cd cuckoo_filter
make
./cockoo_db input_file output_file
```

Define `CUCKOO_DBG` in cuckoo_filter.h to open debug logging.
