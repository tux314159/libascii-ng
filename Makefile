HEADERDIR = include
BUILDDIR = build

V = @

DEBUG = -g
OPTIM = -O3 -march=native -mtune=native
INCLUDEDIR = -I$(HEADERDIR)
LIBFLAGS = -L$(BUILDDIR) -lascii
CFLAGS = -fpic $(INCLUDEDIR) $(DEBUG) $(OPTIM) -MMD -MP -o $@
CFLAGS += $(shell cat compile_flags.txt | tr '\n' ' ')

CC = gcc
AR = ar -rcs $@

# Source file list
SRCS += src/dstring.c
SRCS += src/llist.c
SRCS += src/global.c
SRCS += src/init.c
SRCS += src/drivers/common.c
SRCS += src/drivers/xterm.c
SRCS += src/drivers/vt100.c
SRCS += src/render.c
SRCS += src/winsys.c

TESTSRCS += $(wildcard src/tests/*.c)

 ###

OBJS := $(SRCS:%=$(BUILDDIR)/%.o)
OBJS := $(OBJS:.c.o=.o)
DEPS := $(OBJS:.o=.d)

TESTBINS = $(TESTSRCS:%.c=$(BUILDDIR)/%)
###

.PHONY : all clean tests __FORCE__

.SUFFIXES :

###

all : $(BUILDDIR)/libascii.a tests
	$V echo "Build complete."

-include $(DEPS)

$(BUILDDIR)/libascii.a : $(OBJS)
	$V printf "Creating static library \033[1m$(notdir $@)\033[0m...\n"
	$V $(AR) $^

$(BUILDDIR)/%.o : %.c
	$V mkdir -p $(dir $@)
	$V printf "Compiling \033[1m$(notdir $@)\033[0m from $(notdir $<)...\n"
	$V $(CC) $(CFLAGS) -c $<

### 

tests : $(TESTBINS)

$(BUILDDIR)/src/tests/% : src/tests/%.c $(BUILDDIR)/libascii.a
	$V mkdir -p $(dir $@)
	$V printf "Compiling \033[1m$(notdir $@)\033[0m from $(notdir $<)...\n"
	$V $(CC) $(CFLAGS) $< $(LIBFLAGS)

###

clean : __FORCE__
	$V rm -rf build/
	$V echo Clean

__FORCE__ :
