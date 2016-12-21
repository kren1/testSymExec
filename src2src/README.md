Tool for source to source transformations that does the necessary instrumentation

Makefile thanks to [eliben](https://github.com/eliben/llvm-clang-samples)

Run example:

```bash
build/tool --extra-args="-I/path/to/includes" input.c
```

##Instalation

The tool needs to find clang builtin includes like ```stddef.h```, one way to do it is to move it to the same directory as clang executable ie. ```/usr/lib/llvm-3.9/bin```.
