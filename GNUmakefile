CXX=clang++
SOURCES:=prog.cpp
PROG:=prog
# Figure out sanitizer runtime library path (dodge #145326)
TGTTRIPLE:=$(shell $(CXX) -print-target-triple)
SANRT_LIBDIR:=$(patsubst %/$(TGTTRIPLE),%/linux,$(shell $(CXX) -print-runtime-dir))
LDFLAGS+=-Wl,-rpath,"\$$ORIGIN:$(SANRT_LIBDIR)" -Wl,--no-allow-shlib-undefined -Wl,--no-whole-archive
# Sanitizer builds
SANITIZERS:=undefined,vptr
LDFLAGS+=-fuse-ld=lld -shared-libsan -fsanitize=$(SANITIZERS) -L.
# Little cheat to get these for both C and C++
CPPFLAGS+=-fPIC -O0 -g3 -ggdb -fsanitize=$(SANITIZERS) -fvisibility=hidden
CXXFLAGS+=-stdlib=libc++ -nostdlib++

# "stolen" from GNU make snippet I contributed to AFL++
override LLVM_TOO_NEW_DEFAULT := 21
override LLVM_TOO_OLD_DEFAULT := 17
override _CLANGFMT_VERSIONS_TO_TEST := $(patsubst %,-%,$(shell seq $(LLVM_TOO_NEW_DEFAULT) -1 $(LLVM_TOO_OLD_DEFAULT)))
detect_newest=$(shell for v in "" $(_CLANGFMT_VERSIONS_TO_TEST); do test -n "$$(command -v -- $1$$v)" && { echo "$1$$v"; break; }; done)
CLANG_FORMAT:=$(call detect_newest,clang-format)

all: $(PROG)
	$(realpath $(PROG))

debug: $(PROG)
	env ASAN_OPTIONS=detect_leaks=0 gdb --args $(realpath $(PROG))

$(PROG): $(SOURCES)
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(PROG): LDFLAGS+=-fPIC
$(PROG): LDLIBS+=-l:libc++.a -l:libc++abi.a

pretty:
	$(CLANG_FORMAT) -i $(SOURCES)

clean:
	rm -f -- $(PROG) $(wildcard *.o)

rebuild: clean all

.PHONY: pretty clean rebuild debug
.NOTPARALLEL: clean rebuild
.INTERMEDIATE: prog.o
