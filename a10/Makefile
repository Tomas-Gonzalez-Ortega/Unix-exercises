CC := gcc
CFLAGS := -g -Wall -Wextra -std=gnu11

PROG := tttserver tttclient

all:

tttserver: logger.o
$(PROG): tttmodel.o

a: all
all: $(PROG)
.PHONEY: a all

c: clean
clean:
	$(RM) *.o $(PROG)
.PHONEY: c clean

logger.o: logger.c
tttclient.o: tttclient.c tttmodel.h
tttmodel.o: tttmodel.c tttmodel.h
tttserver.o: tttserver.c tttmodel.h logger.h
