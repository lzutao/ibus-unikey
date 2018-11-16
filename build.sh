#!/bin/sh
if [ "$#" != 1 ]; then
  echo "usage: $0 <html output dir>" 1>&2
  exit 1
fi

set -exu

HERE_DIR="$( cd "$(dirname "$0")"; pwd -P )"

UNIKEY_VERSION=$(git tag --sort=refname | tail -n1)
UNIKEY_DIR=$(realpath .)
UNIKEY_BUILDDIR=builddir

# -- generate JSON compilation database ----------------------------------------

meson "${UNIKEY_BUILDDIR}" "${UNIKEY_DIR}"

# -- generate pages -------------------------------------------------------------

OUTPUT_DIRECTORY="$1"
codebrowser_generator \
    -color \
    -b "$UNIKEY_BUILDDIR" \
    -a \
    -o "${OUTPUT_DIRECTORY}" \
    -p "unikey:${UNIKEY_DIR}:${UNIKEY_VERSION}"
codebrowser_indexgenerator "${OUTPUT_DIRECTORY}"
