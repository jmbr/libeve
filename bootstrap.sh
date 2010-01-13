#!/bin/sh

autoconf
autoheader
aclocal
libtoolize
automake --add-missing
autoconf
