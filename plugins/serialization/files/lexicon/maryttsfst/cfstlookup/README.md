CFstLookup
==========

This C library allows access to FST dictionary files produced
by [MaryTTS](https://github.com/marytts/marytts).

Its development is still early stage, its API is not stable,
and no manipulation functionality is present at the moment.
Loading/Unloading dictionary is expected to be reliable and
to work cross-platform and cross-architecture.
Lookup function is also expected to work.

How to install
==============

The library uses CMake as its build system. In order to compile, test
and install (in a system directory) the library, you may use the usual:

```
mkdir build
cd build
cmake ..
make
make test
make install
```

If you want to install different directory, you can specify it in
`CMAKE_INSTALL_PREFIX`:

```
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=install
make
make test
make install
```
