#!/bin/bash

if command -v libtoolize >/dev/null 2>&1 ; then
    libtoolize
else
    echo "No libtoolize found, assuming we don't need one"
fi

echo "----------------------------------------"
echo "Touch"
touch NEWS README AUTHORS ChangeLog
echo "----------------------------------------"
echo "aclocal"
aclocal
echo "----------------------------------------"
echo "autoheader"
autoheader
echo "----------------------------------------"
echo "automake"
# --gnu???
automake --foreign --add-missing
echo "----------------------------------------"
echo "autoconf"
autoconf
