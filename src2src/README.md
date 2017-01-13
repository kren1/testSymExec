Tool for source to source transformations that does the necessary instrumentation

Makefile thanks to [eliben](https://github.com/eliben/llvm-clang-samples)

Run example:

```bash
build/tool --extra-args="-I/path/to/includes" input.c
```

##Building

Running ```make``` should produce the tool in ```build/``` directory.

The tool needs to find clang builtin includes like ```stddef.h```, one way to do it is to move it to the same directory as clang executable ie. ```/usr/lib/llvm-3.9/bin```. Or run ```make link_builtins``` to create a symbolic link.

##Testing

```make test``` should run the lit tests, that test some of the transformations.

Note that currently ```LIT``` var in the Makefile needs to point to a llvm-3.4 version of lit.py for this to work.
