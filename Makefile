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
LDFLAGS = -Wl,-rpath libmds-ng -Wl,-rpath build
LIBFLAGS = -Lbuild -Llibmds-ng -lmds -lascii
CFLAGS = -std=gnu99 -fpic $(INCLUDEDIR) $(WARNINGS) $(DEBUG) $(OPTIM) -o $@

CC = gcc
AR = ar rcs $@

# Object file list
INIT = init/init.o
DRIVERS = drivers/common.o drivers/vt100.o drivers/xterm.o
RENDER = render/render.o
MISC = global.o
ALLOBJ = $(INIT) $(DRIVERS) $(RENDER) $(MISC)
TESTS = tests/test1 tests/test2
ALLBIN = $(ALLOBJ) $(TESTS) $(BUILDDIR)/libascii.a

all : __MKDIR__ $(BUILDDIR)/libascii.a libmds-ng/libmds.so test

$(BUILDDIR)/libascii.a : $(DRIVERS) $(INIT) $(MISC) $(RENDER)
	$V printf "Creating static library \033[1m$@\033[0m...\n"
	$V $(AR) $^

%.o : %.c
	$V printf "Compiling \033[1m$@\033[0m from $^...\n"
	$V $(CC) $(CFLAGS) -c $^

###

test : $(TESTS)

tests/% : tests/%.c $(BUILDDIR)/libascii.a libmds-ng/libmds.so
	$V printf "Compiling and linking \033[1m$@\033[0m...\n"
	$V $(CC) $(CFLAGS) $(LDFLAGS) $< $(LIBFLAGS)

libmds-ng/libmds.so : libmds-ng
	$V (cd $<; $(MAKE))

clean : __FORCE__
	$V rm -f $(ALLBIN)
	$V make -Clibmds-ng cleanproper
	$V echo Clean

__MKDIR__ : __FORCE__
	$V mkdir -p $(BUILDDIR)

__FORCE__ :
