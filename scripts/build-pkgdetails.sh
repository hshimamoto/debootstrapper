#!/bin/bash

. scripts/default

if [ -e $TOOLS_DIR/pkgdetails/pkgdetails ]; then
	echo "Already exist"
	exit 0
fi

PushD $TOOLS_DIR/pkgdetails

gcc -Wall -g -O2 -static -o pkgdetails pkgdetails.c

PopD
