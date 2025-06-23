#  Trying to reproduce llvm/llvm-project#145299

This code tries to mimic what I have been observing when building a DSO that links libc++ statically but also gets instrumented with ASan and UBSan. ASan alone doesn't trigger the issue and the call stack suggests that UBSan is directly involved.

* There is a trivial program calling `dlopen(..., RTLD_NOW)`
* There is a DSO that statically links libc++ and is instrumented for ASan and UBSan (which both are dynamically linked)

The issue _seems_ that UBSan dynamically links libstdc++ and calls `dynamic_cast` from libstdc++ on an object that originated from libc++.

Unfortunately this example doesn't currently successfully reproduce the issue seen in our real-world code.

## Building

The phony `all` target is in all likelihood what you want, so simply:

```
make
```

... should do.

However, you can also use `make clean` and `make rebuild` (which combines `clean` and `all`).

You can override or prepopulate variables such as `CC`, `CXX`, `CFLAGS`, `LDFLAGS` etc. as needed.

### Debugging

You can override `CC` and `CXX` with another Clang version and the build the `debug` target, which invokes `gdb --args` with the program and the DSO as argument.

```
env CC=clang-19 CXX=clang++-19 make clean debug
```
