#!/bin/bash

OUTFILE=ztgl.h

echo "[$0] combine: add header (ztgl.h)" >&2
cat src/ztgl.h > $OUTFILE

echo "[$0] combine: open implementation" >&2
echo "#ifdef Z_IMPLEMENTATION" >> $OUTFILE
echo "#ifndef Z_IMPLINCLUDED" >> $OUTFILE
echo "#define Z_IMPLINCLUDED" >> $OUTFILE

echo "[$0] combine: add prelude (prelude.c)" >&2
cat src/prelude.c >> $OUTFILE

echo "[$0] combine: add input (z_input.c)" >&2
cat src/z_input.c >> $OUTFILE

echo "[$0] combine: add options (z_options.c)" >&2
cat src/z_options.c >> $OUTFILE

echo "[$0] combine: add pack (z_pack.c)" >&2
cat src/z_pack.c >> $OUTFILE

echo "[$0] combine: add ui (z_ui.c)" >&2
cat src/z_ui.c >> $OUTFILE

echo "[$0] combine: add util (z_util.c)" >&2
cat src/z_util.c >> $OUTFILE

echo "[$0] combine: close implementation" >&2
echo "#endif" >> $OUTFILE
echo "#endif" >> $OUTFILE

echo "[$0] combine: finished successfully" >&2
