# eevo - easy expression value organizer
# See LICENSE file for copyright and license details.

include config.mk

ifeq ($(DEBUG), 1)
CFLAGS += -g -Og
LDFLAGS += -g -Og
endif

EXE = eevo
LIB = $(STD:.c=.so)
DOC = doc/eevo.1.md doc/eevo.5.md
MAN = $(DOC:doc/%.md=doc/man/%)

# markman options for converting markdown to man pages
MANOPTS = -nCD -t EEVO -V "$(EXE) $(VERSION)" -d "`date '+%B %Y'`"
# VERSION without patch eg 1.2.3 -> 1.2
VER=$(shell cut -d '.' -f 1,2 <<< $(VERSION))

all: options $(EXE) $(LIB)

options:
	@echo $(EXE) build options:
	@echo "CFLAGS  = $(CFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"

core.evo.h: $(EVO)
	@echo xxd $@
	@echo "char eevo_core[] = { " > $@
	@cat $^ | xxd -i - >> $@
	@echo ", 0x00};" >> $@

$(EXE)$(VER).c: eevo.c $(CORE) core.evo.h
	@echo creating $@
	@cat $^ > $@
	@sed -i 's/^#include "eevo.h"/#include "$(EXE)$(VER).h"/' $@
	@sed -i '/^#include "..\/eevo.h"/d' $@

$(EXE)$(VER).h: eevo.h
	@echo creating $@
	@cp $< $@

.o:
	@echo $(LD) $@
	@$(LD) -o $@ $< $(LDFLAGS)

.c.o:
	@echo $(CC) $<
	@$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ): $(CORE) eevo.h config.mk

$(LIB): $(STD)
	@echo $(CC) -o $@
	@$(CC) -shared $(CFLAGS) -o $@ $^

$(EXE): $(EXE)$(VER).h $(EXE)$(VER).o main.o
	@echo $(CC) -o $@
	@$(CC) -o $@ main.o $(EXE)$(VER).o $(LDFLAGS)

clean:
	@echo cleaning
	@rm -f $(EXE) $(EXE)$(VER).? $(EXE).o main.o $(LIB) test/test test/test.o

man: $(MAN)

doc/man/%.1: doc/%.1.md $(EXE)
	@echo updating man page $@
	@markman $(MANOPTS) -s "`./$(EXE) -h 2>&1 | cut -d' ' -f2-`" $< > $@

doc/man/%.5: doc/%.5.md $(EXE)
	@echo updating man page $@
	@markman $(MANOPTS) -5 $< > $@

dist: $(EXE)$(VER).h $(EXE)$(VER).c
	@echo creating dist tarball
	@tar -cf $(EXE)-$(VERSION).tar $^
	@gzip $(EXE)-$(VERSION).tar

install: all
	@echo installing $(DESTDIR)$(PREFIX)/bin/$(EXE)$(VER)
	@mkdir -p $(DESTDIR)$(PREFIX)/bin
	@cp -f $(EXE) $(DESTDIR)$(PREFIX)/bin/$(EXE)$(VER)
	@chmod 755 $(DESTDIR)$(PREFIX)/bin/$(EXE)$(VER)
	@echo installing $(DESTDIR)$(PREFIX)/bin/$(EXE)
	@ln -sf $(EXE)$(VER) $(DESTDIR)$(PREFIX)/bin/$(EXE)
	@echo installing $(DESTDIR)$(PREFIX)/bin/evo
	@sed -e "s@\./@@g" < evo > $(DESTDIR)$(PREFIX)/bin/evo
	@chmod 755 $(DESTDIR)$(PREFIX)/bin/evo
	@echo installing $(DESTDIR)$(MANPREFIX)/man1/$(EXE).1
	@echo installing $(DESTDIR)$(MANPREFIX)/man5/$(EXE).5
	@mkdir -p $(DESTDIR)$(MANPREFIX)/man5
	@mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	@cp -f doc/man/$(EXE).1 $(DESTDIR)$(MANPREFIX)/man1/
	@cp -f doc/man/$(EXE).5 $(DESTDIR)$(MANPREFIX)/man5/
	@chmod 644 $(DESTDIR)$(MANPREFIX)/man1/$(EXE).1
	@chmod 644 $(DESTDIR)$(MANPREFIX)/man5/$(EXE).5
	@echo installing core to $(DESTDIR)$(PREFIX)/lib/eevo/pkgs/core
	@mkdir -p $(DESTDIR)$(PREFIX)/lib/eevo/pkgs/core
	@cp -f $(EVO) $(LIB) $(DESTDIR)$(PREFIX)/lib/eevo/pkgs/core

uninstall:
	@echo removing $(EXE) from $(DESTDIR)$(PREFIX)/bin
	@rm -f $(DESTDIR)$(PREFIX)/bin/$(EXE)
	@echo removing manual page from $(DESTDIR)$(MANPREFIX)/man1
	@rm -f $(DESTDIR)$(MANPREFIX)/man1/$(EXE).1
	@echo removing manual page from $(DESTDIR)$(MANPREFIX)/man5
	@rm -f $(DESTDIR)$(MANPREFIX)/man1/$(EXE).5
	@echo removing shared libraries from $(DESTDIR)$(PREFIX)/lib/eevo
	@rm -rf $(DESTDIR)$(PREFIX)/lib/eevo/
	@echo removing eevo libraries from $(DESTDIR)$(PREFIX)/share/eevo
	@rm -rf $(DESTDIR)$(PREFIX)/share/eevo/

test/test.o: test/tests.h

test: $(EXE)$(VER).h $(EXE)$(VER).o test/tests.h test/test.o
	@echo $(CC) -o test/test
	@$(CC) -o test/test $^ $(LDFLAGS)
	@echo running tests
	@./test/test

.PHONY: all options clean man dist install uninstall test
