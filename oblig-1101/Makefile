## Author: Steffen Viken Valvaag <steffenv@cs.uit.no> 
LIST_SRC=linkedlist.c
SET_SRC= set.c   # Insert the file name of your set implementation here
SPAMFILTER_SRC=spamfilter.c common.c $(LIST_SRC) $(SET_SRC)
NUMBERS_SRC=numbers.c common.c $(LIST_SRC) $(SET_SRC)
ASSERT_SRC=assert_set.c common.c $(LIST_SRC) $(SET_SRC)
INCLUDE=include

NUMBERS_SRC:=$(patsubst %.c,src/%.c, $(NUMBERS_SRC))
SPAMFILTER_SRC:=$(patsubst %.c,src/%.c, $(SPAMFILTER_SRC))
ASSERT_SRC:=$(patsubst %.c,src/%.c, $(ASSERT_SRC))

CFLAGS=-Wall -Wextra -g
LDFLAGS=-lm -DLOG_LEVEL=0 -DERROR_FATAL

all: spamfilter numbers

spamfilter: $(SPAMFILTER_SRC) Makefile
	gcc -o $@ -Wall -Wpedantic $(CFLAGS) $(SPAMFILTER_SRC) -I$(INCLUDE)

numbers: $(NUMBERS_SRC) Makefile
	gcc -o $@ -Wall -Wpedantic $(CFLAGS) $(NUMBERS_SRC) -I$(INCLUDE)

assert: $(ASSERT_SRC) Makefile
	gcc -o $@ $(ASSERT_SRC) $(CFLAGS) -I$(INCLUDE)

clean:
	rm -f *~ *.o *.exe spamfilter numbers assert
