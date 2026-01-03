CC              = gcc
CFLAGS          = -ggdb -gdwarf-5 -std=c17 -pedantic -pedantic-errors -Wall -Wextra -Werror
CPPFLAGS        = -D_DEFAULT_SOURCE -D_POSIX_SOURCE=200809L -D_XOPEN_SOURCE=800
LDFLAGS         = -Wl,-z,relro -Wl,-z,now
LIBS            = -lc

SOURCES.server  = common.h server.c
OBJECTS.server  = $(filter-out %.h,$(SOURCES.server:.c=.o))
PROGRAMS.server = server$(EXEEXT)

SOURCES.client  = common.h client.c
OBJECTS.client  = $(filter-out %.h,$(SOURCES.client:.c=.o))
PROGRAMS.client = client$(EXEEXT)

OBJECTS         = $(OBJECTS.server) $(OBJECTS.client)
PROGRAMS        = $(PROGRAMS.server) $(PROGRAMS.client)

all:  $(PROGRAMS)

clean:
	rm -f $(OBJECTS) $(PROGRAMS)

%.o: %.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -fPIC -o $@ $<

$(OBJECTS.server): $(SOURCES.server)
$(OBJECTS.client): $(SOURCES.client)

$(PROGRAMS.server): $(OBJECTS.server)
	$(CC) $(LDFLAGS) -pie -o $@ $< $(LIBS)

$(PROGRAMS.client): $(OBJECTS.client)
	$(CC) $(LDFLAGS) -pie -o $@ $< $(LIBS)

.PHONY: all clean
