#!/bin/bash

OUTFILE=tgl.h

echo "[$0] combine: add header (tgl.h)" >&2
cat src/tgl.h > $OUTFILE

echo "[$0] combine: open implementation" >&2
echo "#ifdef TGL_IMPLEMENTATION" >> $OUTFILE
echo "#ifndef TGL_IMPLINCLUDED" >> $OUTFILE
echo "#define TGL_IMPLINCLUDED" >> $OUTFILE

echo "[$0] combine: add prelude (prelude.c)" >&2
cat src/prelude.c >> $OUTFILE

echo "[$0] combine: add input (tgl_input.c)" >&2
cat src/tgl_input.c >> $OUTFILE

echo "[$0] combine: add options (tgl_options.c)" >&2
cat src/tgl_options.c >> $OUTFILE

echo "[$0] combine: add pack (tgl_pack.c)" >&2
cat src/tgl_pack.c >> $OUTFILE

echo "[$0] combine: add ui (tgl_ui.c)" >&2
cat src/tgl_ui.c >> $OUTFILE

echo "[$0] combine: add util (tgl_util.c)" >&2
cat src/tgl_util.c >> $OUTFILE

echo "[$0] combine: close implementation" >&2
echo "#endif" >> $OUTFILE
echo "#endif" >> $OUTFILE

echo "[$0] combine: finished successfully" >&2
