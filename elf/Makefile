# Changed when ABI backwards compatibility is broken.
# Typically uses the major version.
SONAME = 0

CXXFLAGS+=-g -O2 -Werror
override CXXFLAGS+=-std=c++0x -Wall -fPIC

all: libelf++.a libelf++.so libelf++.so.$(SONAME) libelf++.pc

SRCS := elf.cc mmap_loader.cc to_string.cc
HDRS := elf++.hh data.hh common.hh to_hex.hh
CLEAN :=

libelf++.a: $(SRCS:.cc=.o)
	ar rcs $@ $^
CLEAN += libelf++.a $(SRCS:.cc=.o)

$(SRCS:.cc=.o): $(HDRS)

to_string.cc: enum-print.py to_string.py data.hh Makefile
	python3 to_string.py

CLEAN += to_string.cc

libelf++.so.$(SONAME): $(SRCS:.cc=.o)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -shared -Wl,-soname,$@ -o $@ $^
CLEAN += libelf++.so.*

libelf++.so:
	ln -s $@.$(SONAME) $@
CLEAN += libelf++.so

# Create pkg-config for local library and headers. This will be
# transformed in to the correct global pkg-config by install.
libelf++.pc: always
	@(VER=$$(git describe --match 'v*' | sed -e s/^v//); \
	  echo "libdir=$$PWD"; \
	  echo "includedir=$$PWD"; \
	  echo ""; \
	  echo "Name: libelf++"; \
	  echo "Description: C++11 ELF library"; \
	  echo "Version: $$VER"; \
	  echo "Libs: -L\$${libdir} -lelf++"; \
	  echo "Cflags: -I\$${includedir}") > $@
CLEAN += libelf++.pc

.PHONY: always

PREFIX?=/usr/local

install: libelf++.a libelf++.so libelf++.so.$(SONAME) libelf++.pc
	install -d $(PREFIX)/lib/pkgconfig
	install -t $(PREFIX)/lib libelf++.a
	install -t $(PREFIX)/lib libelf++.so.$(SONAME)
	install -t $(PREFIX)/lib libelf++.so
	install -d $(PREFIX)/include/libelfin/elf
	install -t $(PREFIX)/include/libelfin/elf common.hh data.hh elf++.hh
	sed 's,^libdir=.*,libdir=$(PREFIX)/lib,;s,^includedir=.*,includedir=$(PREFIX)/include,' libelf++.pc \
		> $(PREFIX)/lib/pkgconfig/libelf++.pc

clean:
	rm -f $(CLEAN)

.DELETE_ON_ERROR:
