# Zynq Petalinux Example

> **WARNING:** it is WIP and I'm learning Petalinux

* Tools version: 2022.1
* Board: ZCU102 (Zynq US+)
* Host OS: Ubuntu 20.04
* License: CC-BY-4.0

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
petalinux-package --boot --u-boot --fpga images/linux/system.bit --force
```

## Prepare the Hardware, boot and login

* Make two partitions on your SD card
  * The first partition must be a FAT32 file system with at least 500MB space
  * The second partition needs to be of EXT4 file system.
* Copy the following 3 files (`images/linux`) in the FAT32 partition:
  * `BOOT.bin`, `boot.scr` and `image.ub`
* Extract the contents of rootfs.tar.gz into the EXT4 partition:
```
sudo tar -xzvf rootfs.tar.gz -C /media/<mount-point>
sync
```
* Insert the SD card in your board.
* For the ZCU102 the switches at SW6[4:1] must be OFF OFF OFF ON.
* Connect a micro-USB cable in J83 (USB-UART bridge).
* Configure a serial terminal (ex: PuTTY)
  * Device: `/dev/ttyUSB0` (ZCU102)
  * Bauds: 115200
* Power ON.
* Login with:
  * User: petalinux
  * Pass: <should be set on first boot>

## devmem example

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
