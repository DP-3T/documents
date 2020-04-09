SOURCES := test.c libdp3t_v2.c cuckoofilter-utopiaprince/cuckoo_filter.c
OBJS := $(SOURCES:.c=.o)
LIBS := -lcrypto

all: test

test: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)
	./test

.c.o:
	$(CC) -Icuckoofilter-utopiaprince $(CFLAGS) $(INCLUDES) -c $<

