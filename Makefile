HEADERDIR = include

V = @

WARNINGS += -Wall
WARNINGS += -Wextra
#WARNINGS += -Waggregate-return
WARNINGS += -Wbad-function-cast
WARNINGS += -Wcast-align
WARNINGS += -Wcast-qual
WARNINGS += -Wdeclaration-after-statement
WARNINGS += -Wfloat-equal
WARNINGS += -Wformat=2
WARNINGS += -Wmissing-declarations
WARNINGS += -Wmissing-include-dirs
WARNINGS += -Wmissing-prototypes
WARNINGS += -Wnested-externs
WARNINGS += -Wpointer-arith
WARNINGS += -Wredundant-decls
WARNINGS += -Wsequence-point
WARNINGS += -Wshadow
WARNINGS += -Wstrict-prototypes
WARNINGS += -Wswitch
WARNINGS += -Wundef
WARNINGS += -Wunreachable-code
WARNINGS += -Wunused-parameter
WARNINGS += -Wwrite-strings
WARNINGS += -Werror

DEBUG = -g
OPTIM = -O3 -march=native -mtune=native
INCLUDEDIR = -I$(HEADERDIR)
LIBFLAGS = -L. -lascii
CFLAGS = -std=gnu99 -fpic $(INCLUDEDIR) $(WARNINGS) $(DEBUG) $(OPTIM) -o $@

CC = gcc
AR = ar rcs $@

# Source file list
SOURCES += src/init.c
SOURCES += src/render.c
SOURCES += src/drivers/common.c
SOURCES += src/drivers/xterm.c
SOURCES += src/drivers/vt100.c
SOURCES += src/global.c
SOURCES += src/io_toplevel.c
SOURCES += src/dstring.c

OBJECTS = $(foreach curfile,$(SOURCES),$(basename $(curfile)).o)

TESTS = tests/build/test1 tests/build/test2

.SUFFIXES :

all : libascii.a test

libascii.a : $(OBJECTS)
	$V printf "Creating static library \033[1m$@\033[0m...\n"
	$V $(AR) $^

%.o : %.c
	$V printf "Compiling \033[1m$(notdir $@)\033[0m from $(notdir $<)...\n"
	$V $(CC) $(CFLAGS) -c $^

###

test : $(TESTS)

tests/build/% : tests/%.c libascii.a
	$V printf "Compiling and linking \033[1m$@\033[0m...\n"
	$V $(CC) $(CFLAGS) $(LDFLAGS) $< $(LIBFLAGS)

clean : __FORCE__
	$V rm -f $(OBJECTS)
	$V rm -f $(TESTS)
	$V echo Clean

__FORCE__ :
