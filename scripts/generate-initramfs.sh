#!/bin/bash

. scripts/default

pushd $WORK_DIR > /dev/null 2>&1

rm -rf initramfs
mkdir -p initramfs
cd initramfs

mkdir rootfs
cd rootfs

mkdir -p dev sys proc
mkdir -p bin sbin lib
mkdir -p etc var

# busybox
cp -a $WORK_DIR/$BB/_install/* .

# debootstrap
cp -a $WORK_DIR/debootstrap/usr .

# pkgdetails
cp -a $TOOLS_DIR/pkgdetails/pkgdetails usr/share/debootstrap/pkgdetails

# init
cp -a $FILES_DIR/init init
chmod +x init

# generate initramfs.img
find . | cpio -o -H newc -R 0:0 | gzip -9 > ../initramfs.img

cd ..

popd > /dev/null 2>&1
