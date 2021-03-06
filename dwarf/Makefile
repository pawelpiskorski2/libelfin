# Changed when ABI backwards compatibility is broken.
# Typically uses the major version.
SONAME = 0

CXXFLAGS+=-g -O2 -Werror
override CXXFLAGS+=-std=c++14 -Wall -fPIC

all: libdwarf++.a libdwarf++.so.$(SONAME) libdwarf++.so libdwarf++.pc

SRCS := dwarf.cc cursor.cc die.cc value.cc abbrev.cc \
	expr.cc rangelist.cc line.cc attrs.cc \
	die_str_map.cc elf.cc to_string.cc
HDRS := dwarf++.hh data.hh internal.hh small_vector.hh ../elf/to_hex.hh
CLEAN :=

libdwarf++.a: $(SRCS:.cc=.o)
	ar rcs $@ $^
CLEAN += libdwarf++.a $(SRCS:.cc=.o)

$(SRCS:.cc=.o): $(HDRS)

to_string.cc: ../elf/enum-print.py dwarf++.hh data.hh Makefile
	python3 to_string.py

CLEAN += to_string.cc

libdwarf++.so.$(SONAME): $(SRCS:.cc=.o)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -shared -Wl,-soname,$@ -o $@ $^
CLEAN += libdwarf++.so.*

libdwarf++.so:
	ln -s $@.$(SONAME) $@
CLEAN += libdwarf++.so

# Create pkg-config for local library and headers. This will be
# transformed in to the correct global pkg-config by install.
libdwarf++.pc: always
	@(VER=$$(git describe --match 'v*' | sed -e s/^v//); \
	  echo "libdir=$$PWD"; \
	  echo "includedir=$$PWD"; \
	  echo ""; \
	  echo "Name: libdwarf++"; \
	  echo "Description: C++11 DWARF library"; \
	  echo "Version: $$VER"; \
	  echo "Requires: libelf++ = $$VER"; \
	  echo "Libs: -L\$${libdir} -ldwarf++"; \
	  echo "Cflags: -I\$${includedir}") > $@
CLEAN += libdwarf++.pc

.PHONY: always

PREFIX?=/usr/local

install: libdwarf++.a libdwarf++.so.$(SONAME) libdwarf++.so libdwarf++.pc
	install -d $(PREFIX)/lib/pkgconfig
	install -t $(PREFIX)/lib libdwarf++.a
	install -t $(PREFIX)/lib libdwarf++.so.$(SONAME)
	install -t $(PREFIX)/lib libdwarf++.so
	install -d $(PREFIX)/include/libelfin/dwarf
	install -t $(PREFIX)/include/libelfin/dwarf data.hh dwarf++.hh small_vector.hh
	sed 's,^libdir=.*,libdir=$(PREFIX)/lib,;s,^includedir=.*,includedir=$(PREFIX)/include,' libdwarf++.pc \
		> $(PREFIX)/lib/pkgconfig/libdwarf++.pc

clean:
	rm -f $(CLEAN)

.DELETE_ON_ERROR:
