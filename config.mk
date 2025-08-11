# eevo version number
VERSION = 0.1

# core modules and standard library to include
STD    = std/io.c std/os.c
STDEVO = std/io.evo std/doc.evo
CORE   = core/core.c core/string.c core/math.c $(STD)
EVO    = core/core.evo core/list.evo core/math.evo $(STDEVO)

# paths
PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man
LIBPREFIX = $(PREFIX)/lib/eevo/pkgs/std

# includes and libraries
INCS = -Iinclude
LIBS = -lm -ldl

# flags
DEFINES = -DVERSION=\"$(VERSION)\" -D_POSIX_C_SOURCE=200809L
CFLAGS  = -std=c99 -O3 -pedantic -Wall -fPIC $(INCS) $(DEFINES)
LDFLAGS = -O3 -Wl,-rpath=$(DESTDIR)$(PREFIX)/lib/eevo $(LIBS)

# turn off debug mode by default
DEBUG ?= 0

# compiler and linker
CC ?= cc
