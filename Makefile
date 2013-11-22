eXe = fast-grep
OBJS = $(eXe).o args.o print.o queue.o

#DEBUG = yes
ifdef DEBUG
DBGFLAGS = -g -O0
else
DBGFLAGS = -O3
endif
CFLAGS = $(DBGFLAGS) -pipe -W -Wall -D_GNU_SOURCE
LIBS = -lpthread

all: $(eXe)

$(eXe): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

.PHONY: clean check

check:
	clang --analyze $(CFLAGS) $(OBJS:.o=.c)

clean:
	rm -f core $(eXe) $(OBJS) *~
