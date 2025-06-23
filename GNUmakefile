CC=clang
CXX=clang++
SOURCES:=prog.c lib.cpp
PROG:=prog
DSO:=libfoo.so
# Figure out sanitizer runtime library path (dodge #145326)
TGTTRIPLE:=$(shell $(CC) -print-target-triple)
SANRT_LIBDIR:=$(patsubst %/$(TGTTRIPLE),%/linux,$(shell $(CC) -print-runtime-dir))
LDFLAGS+=-Wl,-rpath,"$$ORIGIN:$(SANRT_LIBDIR)"
# Sanitizer builds
LDFLAGS+=-fuse-ld=lld -shared-libsan -fsanitize=address,undefined
# Little cheat to get these for both C and C++
CPPFLAGS+=-fPIC -O1 -g3 -ggdb -fsanitize=address,undefined
CXXFLAGS+=-stdlib=libc++ -nostdlib++

# "stolen" from GNU make snippet I contributed to AFL++
override LLVM_TOO_NEW_DEFAULT := 21
override LLVM_TOO_OLD_DEFAULT := 17
override _CLANGFMT_VERSIONS_TO_TEST := $(patsubst %,-%,$(shell seq $(LLVM_TOO_NEW_DEFAULT) -1 $(LLVM_TOO_OLD_DEFAULT)))
detect_newest=$(shell for v in "" $(_CLANGFMT_VERSIONS_TO_TEST); do test -n "$$(command -v -- $1$$v)" && { echo "$1$$v"; break; }; done)
CLANG_FORMAT:=$(call detect_newest,clang-format)

all: $(PROG) $(DSO)
	$(realpath $(PROG)) $(realpath $(DSO))

debug: $(PROG) $(DSO)
	env ASAN_OPTIONS=detect_leaks=0 gdb --args $(realpath $(PROG)) $(realpath $(DSO))

$(PROG): prog.o

$(DSO): lib.cpp
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(DSO): LDFLAGS+=-l:libc++.a  -l:libc++abi.a -fPIC -shared
#-fvisibility=hidden

pretty:
	$(CLANG_FORMAT) -i $(SOURCES)

clean:
	rm -f -- $(PROG) $(DSO) $(wildcard *.o)

rebuild: clean all

.PHONY: pretty clean rebuild debug
.NOTPARALLEL: clean rebuild
.INTERMEDIATE: lib.o prog.o
