#!/bin/bash

echo "[$0] combine: add header (tgl.h)" >&2
cat src/tgl.h > tgl.h

echo "[$0] combine: open implementation guard" >&2
echo "#ifdef TGL_IMPLEMENTATION" >> tgl.h
echo "#ifndef TGL_IMPLINCLUDED" >> tgl.h
echo "#define TGL_IMPLINCLUDED" >> tgl.h
echo "#include <stdarg.h>" >> tgl.h

echo "[$0] combine: add input (tgl_input.c)" >&2
cat src/tgl_input.c >> tgl.h

echo "[$0] combine: add util (tgl_util.c)" >&2
cat src/tgl_util.c >> tgl.h

echo "[$0] combine: close implementation guard" >&2
echo "#endif" >> tgl.h
echo "#endif" >> tgl.h

echo "[$0] combine: finished successfully" >&2
