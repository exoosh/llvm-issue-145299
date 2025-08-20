#  Repro for llvm/llvm-project#145299

This code tries to mimic what I have been observing when building a DSO that links libc++ statically but also gets instrumented with ASan and UBSan. ASan alone doesn't trigger the issue and the call stack suggests that UBSan is directly involved.

Boiled down, it suffices if we create a program that statically links libc++ but gets instrumented with UBSan, where the (apt.llm.org-provided) build of the xSan runtime DSO uses libstdc++ to invoke `dynamic_cast`.

The issue _seems_ that UBSan dynamically links libstdc++ and calls `dynamic_cast` from libstdc++ on an object that originated from libc++.

## Building

The phony `all` target is in all likelihood what you want, so simply:

```
make
```

... should do.

However, you can also use `make clean` and `make rebuild` (which combines `clean` and `all`).

You can override or prepopulate variables such as `CXX`, `LDFLAGS` etc. as needed.

### Debugging

You can override `CXX` with another Clang version and the build the `debug` target, which invokes `gdb --args` with the program and the DSO as argument.

```
env CXX=clang++-19 make clean debug
```
