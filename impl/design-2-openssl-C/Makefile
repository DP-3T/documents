SOURCES := test.c libdp3t_v2.c cuckoofilter-utopiaprince/cuckoo_filter.c
OBJS := ./test.o ./cuckoo_filter.o ./libdp3t_v2.o
LIBS := -lcrypto

all: test

test: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)

clean:
	rm $(OBJS)
.c.o:
	$(CC) -Icuckoofilter-utopiaprince $(CFLAGS) $(INCLUDES) -c $<

