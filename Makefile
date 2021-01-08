BUILDDIR = build
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
INCLUDEDIR = -I$(HEADERDIR) -Ilibmds-ng/include
LDFLAGS = -Wl,-rpath libmds-ng -Wl,-rpath .
LIBFLAGS = -Lbuild -Llibmds-ng -lmds -lascii
CFLAGS = -std=gnu99 -fpic $(INCLUDEDIR) $(WARNINGS) $(DEBUG) $(OPTIM) -o $@

CC = gcc
AR = ar rcs $@

# Object file list
INIT = init/init.o
DRIVERS = drivers/common.o drivers/vt100.o drivers/xterm.o
RENDER = render/render.o
MISC = global.o

all : __MKDIR__ tests $(BUILDDIR)/libascii.a

tests : tests/spinner

tests/spinner : tests/spinner.c $(BUILDDIR)/libascii.a libmds-ng/libmds.so

$(BUILDDIR)/libascii.a : $(DRIVERS) $(INIT) $(MISC) $(RENDER)
	$V printf "Creating static library \033[1m$@\033[0m...\n"
	$V $(AR) $^

%.o : %.c
	$V printf "Compiling \033[1m$@\033[0m from $^...\n"
	$V $(CC) $(CFLAGS) -c $^

% : %.c
	$V printf "Compiling \033[1m$@\033[0m from $^...\n"
	$V $(CC) $(CFLAGS) $(LDFLAGS) $^ $(LIBFLAGS)

__MKDIR__ : __FORCE__
	$V mkdir -p $(BUILDDIR)

__FORCE__ :
