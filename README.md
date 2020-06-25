# CrapsSim
a simulator for trying out craps strategies

# code structure

CrapsSim.cpp - main program entry point
stdafx.cpp - pre-compiled header entry point

# getting started

This project was written using Conan (for package management), Cmake (for building), MinGW-W64 (for compiling on windows, gcc on linux should work fine).

## install first

Install mingw-w64 (or just gcc for linux), conan, and cmake.  Make sure to add all of them to your path.

To ensure all versions, run...

```
gcc -v (should be 8.1 or higher)
conan -v (should be 1.26 or higher)
cmake --version (should be 3.17 or higher)
```

## build

```
mkdir build && cd build
conan install .. --build missing
cmake .. -G "MinGW Makefiles"
cmake --build .
```