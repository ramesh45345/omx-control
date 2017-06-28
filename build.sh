#!/bin/bash

# Exit on error.
set -e

# Get folder of this script
SCRIPTSOURCE="${BASH_SOURCE[0]}"
FLWSOURCE="$(readlink -f "$SCRIPTSOURCE")"
SCRIPTDIR="$(dirname "$FLWSOURCE")"
SCRNAME="$(basename $SCRIPTSOURCE)"
echo "Executing ${SCRNAME}."

# Check for cmake
DEP="cmake"
for D in $DEP
do
    type $D > /dev/null 2>&1 || { echo "$D is not available." 1>&2; exit 1; }
done

# Create build folder if it is somehow missing
if [ ! -d $SCRIPTDIR/build ]; then
  echo "Build folder is missing. Creating $SCRIPTDIR/build"
  mkdir build
fi
cd $SCRIPTDIR/build
# Remove the old binary if it exists.
BINARYPATH="$SCRIPTDIR/build/omx-control"
if [ -f $BINARYPATH ]; then
  echo "Removing old binary at $BINARYPATH"
  rm $BINARYPATH
fi
# Run cmake
echo "Running cmake."
cmake ..
# Make the project
echo "Running make."
make

# Print location of binary.
if [ -f $BINARYPATH ]; then
  chmod a+x "$BINARYPATH"
  echo "Binary successfully build at $BINARYPATH"
  BINSCRIPTPATH="$SCRIPTDIR/build/omx-control.sh"
  echo -e '#!/bin/sh\nexec ./omx-control' > "$BINSCRIPTPATH"
  chmod a+x "$BINSCRIPTPATH"
  echo "Loader script successfully created at $BINSCRIPTPATH"
else
  echo "Build not successful, binary not found at $BINARYPATH"
fi
