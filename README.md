# Zynq Petalinux Example

> **WARNING:** it is WIP and I'm doing my first steps with Petalinux

* Tools version: 2022.1
* Board: ZCU102 (Zynq US+)
* Host OS: Ubuntu 20.04
* License: CC-BY-4.0

## Description

In the PL, the PS master M_AXI_HPM0_FPD controls 3 BRAM controllers (all 128-bits):
* axi_bram_ctrl_0 at 0xA000_4000
* axi_bram_ctrl_1 at 0xA000_0000
* axi_bram_ctrl_2 at 0xA000_2000
The axi_bram_ctrl_0 is connected to a free runnings counter producing the pattern
`0xD<cnt[7:0]>C<cnt[7:0]>B<cnt[7:0]>A<cnt[7:0]>`, to clearly identify the four
32-bits groups. The other two BRAM controllers, are connected to a True Dual Port
Memory (128 x 8192).

In the PS, there is a Linux generated with Petalinux.
The BRAM controllers `0` and `1` supports the UIO driver (`uio4` and `uio5`).
The BRAM controller `2` is configured as reserved memory.

# Instructions

## Generate the Hardware Design

* Open the Vivado project `vivado/project.xpr`
* Generate Bitstream
* File -> Export -> Export Hardware (include bitstream)
  * It will generate the needed `.xsa` file
* Close Vivado

## Build Linux

```
source /<PETALINUX_INSTALLATION>/settings.sh
cd petalinux
petalinux-config --get-hw-description=../vivado/
petalinux-build
```

## Prepare the Hardware, boot and login

* Connect a micro-USB cable in J83 (USB-UART bridge, ZCU102)
* Configure a serial terminal (ex: PuTTY)
  * Device: `/dev/ttyUSB0` (ZCU102)
  * Bauds: 115200

To boot from JTAG:
* For the ZCU102 the switches at SW6[4:1] must be ON ON ON ON
* Power ON
* `petalinux-boot --jtag --kernel`

To boot from an SD card:
* `petalinux-package --boot --u-boot --fpga images/linux/system.bit --force`
* Make two partitions on your SD card
  * The first partition must be a FAT32 file system with at least 500MB space
  * The second partition needs to be of EXT4 file system.
* Copy the following 3 files (`images/linux`) in the FAT32 partition:
  * `BOOT.bin`, `boot.scr` and `image.ub`
* Extract the contents of rootfs.tar.gz into the EXT4 partition:
  * `sudo tar -xzvf rootfs.tar.gz -C /media/<mount-point>; sync`
* Insert the SD card in your board.
* For the ZCU102 the switches at SW6[4:1] must be OFF OFF OFF ON
* Power ON

* Login:
  * User: petalinux
  * Pass: <by default you need to specify a new one in each boot>

## devmem example session

```
petalinux:~$ sudo devmem 0xA0000000 64 0x1122334455667788
petalinux:~$ sudo devmem 0xA0000000 64
0x1122334455667788
petalinux:~$ sudo devmem 0xA0002000 64 0x99AABBCCDDEEFF00
petalinux:~$ sudo devmem 0xA0002000 64
0x99AABBCCDDEEFF00
petalinux:~$ sudo devmem 0xA0004000 64
0xB6C57068A6C57068
petalinux:~$ sudo devmem 0xA0004008 64
0xDCE4A6AACCE4A6AA
petalinux:~$ sudo devmem 0xA0004000 32
0xA1A0B39C
petalinux:~$ sudo devmem 0xA0004000 16
0x0AEB
petalinux:~$ sudo devmem 0xA0004000 8
0xFA
```

## Custom app example session

Using `/dev/mem`:
```
$ sudo mytest /dev/mem 0xA0004000
* 32-bits
0xae7483b0 (diff = 0)
0xae7483dc (diff = 44)
0xae748408 (diff = 44)
0xae748434 (diff = 44)
* 64-bits
0xbe74976bae74976b (diff = 0)
0xbe749797ae749797 (diff = 44)
0xbe7497c3ae7497c3 (diff = 44)
0xbe7497efae7497ef (diff = 44)
* 128-bits
0xde74aa0cce74aa0cbe74aa0cae74aa0c (diff = 0)
0xde74aa38ce74aa38be74aa38ae74aa38 (diff = 44)
0xde74aa64ce74aa64be74aa64ae74aa64 (diff = 44)
0xde74aa90ce74aa90be74aa90ae74aa90 (diff = 44)
* The End
```

Using `/dev/uio4`:
```
$ sudo mytest /dev/uio4 0xA0004000
* 32-bits
0xa4c3058d (diff = 0)
0xa4c305b9 (diff = 44)
0xa4c305e5 (diff = 44)
0xa4c30611 (diff = 44)
* 64-bits
0xb4c3197ca4c3197c (diff = 0)
0xb4c319a8a4c319a8 (diff = 44)
0xb4c319d4a4c319d4 (diff = 44)
0xb4c31a00a4c31a00 (diff = 44)
* 128-bits
0xd4c32d40c4c32d40b4c32d40a4c32d40 (diff = 0)
0xd4c32d6cc4c32d6cb4c32d6ca4c32d6c (diff = 44)
0xd4c32d98c4c32d98b4c32d98a4c32d98 (diff = 44)
0xd4c32dc4c4c32dc4b4c32dc4a4c32dc4 (diff = 44)
* The End
```
