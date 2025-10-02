#!/bin/bash

INCLUDE=""
DEFINES="-DZ_IMPLEMENTATION"
WARNINGS="-Wall -Wextra -Wshadow"
LIBRARIES="$(pkg-config --cflags sdl2)"
FLAGS="-std=c++20 -pedantic -fno-rtti -fno-exceptions"

CPP=g++
FLAGSFULL="$INCLUDE $DEFINES $WARNINGS $FLAGS $LIBRARIES"

echo "[$0] test-build: copying" >&2
cp ztgl.hh ztgl.cc

echo "[$0] test-build: compilation" >&2
$CPP -o ztgl.o -c ztgl.cc $FLAGSFULL
if [ $? -ne 0 ]
then
	echo "[$0] test-build: failed to compile!" >&2
	exit 1
fi

echo "[$0] test-build: finished successfully" >&2
