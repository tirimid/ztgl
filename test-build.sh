#!/bin/bash

INCLUDE=""
DEFINES=""
WARNINGS="-Wall -Wextra -Wshadow"
LIBRARIES="$(pkg-config --cflags sdl2)"
CFLAGS="-std=c99 -pedantic"

CC=gcc
CFLAGS_FULL="$INCLUDE $DEFINES $WARNINGS $CFLAGS $DEFINES $LIBRARIES"

echo "[$0] test-build: copying" >&2
cp tgl.h tgl.c

echo "[$0] test-build: compilation" >&2
$CC -o tgl.o -c tgl.c $CFLAGS_FULL
if [ $? -ne 0 ]
then
	echo "[$0] test-build: failed to compile!" >&2
	exit 1
fi

echo "[$0] test-build: finished successfully" >&2
