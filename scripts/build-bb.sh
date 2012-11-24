#!/bin/bash

. scripts/default

if [ ! -e $DL_DIR/$BBTAR ]; then
	PushD $DL_DIR
	wget $BBURL
	PopD
fi

if [ -e $WORK_DIR/$BB/busybox ]; then
	echo "Already exist"
	exit 0
fi

PushD $WORK_DIR

tar jxf $DL_DIR/$BBTAR
cd $BB

make defconfig
sed \
	-e 's/.*CONFIG_STATIC.*/CONFIG_STATIC=y/' \
	-e 's/.*CONFIG_FEATURE_INITRD*/CONFIG_FEATURE_INITRD=n/' \
	-e 's/.*CONFIG_INETD*/CONFIG_INETD=n/' \
	-e 's/.*CONFIG_AR*/CONFIG_AR=y/' \
	.config > mini.config
echo "CONFIG_ASH=y" >> mini.config
make KCONFIG_ALLCONFIG=mini.config allnoconfig

Make
Make install

PopD
