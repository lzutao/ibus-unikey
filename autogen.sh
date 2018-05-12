#!/bin/sh
# Run this to generate all the initial makefiles, etc.
# Ref: https://github.com/GNOME/gtetrinet/blob/master/autogen.sh

# Exit immediately if a command exits with a non-zero status
set -e
# Print commands and their arguments as they are executed
#set -x

test -n "$srcdir" || srcdir=$(dirname "$0")
test -n "$srcdir" || srcdir=.

olddir=$(pwd)
cd "$srcdir"

( test -f configure.ac ) || {
    echo "*** ERROR: Directory '$srcdir' does not look like the top-level project directory ***"
    exit 1
}

if [ "$#" = 0 -a "x$NOCONFIGURE" = "x" ]; then
    echo "*** WARNING: I am going to run 'configure' with no arguments." >&2
    echo "*** If you wish to pass any to it, please specify them on the" >&2
    echo "*** '$0' command line." >&2
    echo "" >&2
fi

AUTORECONF=$(which autoreconf)
if test -z "$AUTORECONF"; then
    echo "*** No autoreconf found, please intall it ***"
    exit 1
fi

autoreconf --install --verbose

cd "$olddir"
if test -z "$NOCONFIGURE"; then
    export CFLAGS="-g -O0"
    export CXXFLAGS="$CFLAGS"
    #"$srcdir/configure" --enable-maintainer-mode "$@"
    "$srcdir/configure" "$@"

    if test "$1" = "--help"; then
        exit 0
    else
        echo "Now type 'make' to compile."
    fi
else
    echo "Skipping configure process."
fi
