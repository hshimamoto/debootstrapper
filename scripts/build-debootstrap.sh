#!/bin/bash

. scripts/default

if [ ! -e $DL_DIR/$DEB ]; then
	pushd $DL_DIR > /dev/null 2>&1
	wget $DEBURL
	popd > /dev/null 2>&1
fi

MkDir $WORK_DIR/debootstrap

if [ -e $WORK_DIR/debootstrap/usr/sbin/debootstrap ]; then
	echo "Already exist"
	exit 0
fi

PushD $WORK_DIR/debootstrap

ar x $DL_DIR/$DEB
tar zxf data.tar.gz

PopD
