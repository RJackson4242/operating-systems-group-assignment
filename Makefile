CC = gcc
CFLAGS = -Wall -Wpointer-arith -Wstrict-prototypes -std=gnu89 -fPIC -MMD -MP -lpthread

all: test-mergesort test-mergesort-custom

#This builds an executable 
test-mergesort: test-mergesort.o mergesort.o
	$(CC) $(CFLAGS) -o $@ $?

test-mergesort-custom: test-mergesort-custom.o mergesort.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

.PHONY: clean
clean:
	/bin/rm -f *.o *.d test-mergesort test-mergesort-custom
