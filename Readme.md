# How to Use

Work on x86_64 system.

## Build files

### Busybox

    $ ./scripts/build-bb.sh

### Kernel

    $ ./scripts/build-kernel.sh

### debootstrap

    $ ./scripts/build-debootstrap.sh

### pkgdetails

    $ ./scripts/build-pkgdetails.sh

## Generate initramfs

    $ ./scripts/generate-initramfs.sh

## Generate base filesystem

    $ ./host/proxy.rb

    $ nc -l 1234 | xz -9 > squeeze.1st.<arch>.tar.xz

    $ qemu \
        -kernel work/linux-3.7.3/arch/x86/boot/bzImage \
        -initrd work/initramfs/initramfs.img \
        -append "console=ttyS0 arch=amd64 panic=1" \
        -device virtio-net,netdev=vnic \
        -netdev user,id=vnic \
        -m 2048 -smp 4 \
        -nographic \
        -no-reboot

    # cd /rootfs
    # tar cf - * | nc 10.0.2.2 1234
