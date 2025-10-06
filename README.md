# ztgl

## Introduction

ZTGL, or (Z)-prefixed (T)irimid's (G)ame (L)ibrary is a library containing code
for modules that I've either found myself constantly reimplementing in other
game / graphical projets, or simply wanted to segment into a library.

## Dependencies

* SDL2
* SDL2_ttf
* A C++20 compiler (for testing and usage)

## Management

* To test whether the library compiles, run `./test-build.sh`

## Usage

To use the library in your own projects, simply copy `ztgl.hh` into the
directory tree, then include the header in your code. As it is a single-header
library, you will need to pass `-DZTGL_IMPLEMENTATION` or `#define
ZTGL_IMPLEMENTATION` in order to include implementation code where needed.

## Contributing

Bug fixes will be accepted. Features will be accepted if I like them. Feel free
to fork this repository and make your own version of ZTGL.
