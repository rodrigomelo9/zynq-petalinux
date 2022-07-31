# Zynq Petalinux Example

> **WARNING:** it is WIP and I'm doing my first steps with Petalinux

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
  * Pass: <by default, you need to specify a new one in each boot>

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

## Custom app (mytest) example session

> **WARNING:** it is WIP

```
$ sudo mytest /dev/mem 0xA0004000
ADDR=0xa0004000 PAGE_SIZE=0x00001000 PAGE_ADDR=0xa0004000 PAGE_OFFSET=0x00000000

* 32-bits
0xa3a966af (diff = 0)
0xa3a966db (diff = 0)
0xa3a96707 (diff = 0)
0xa3a96733 (diff = 0)
* 64-bits
0xb3a983bda3a983bd (diff = 0)
0xb3a983e9a3a983e9 (diff = 44)
0xb3a98415a3a98415 (diff = 44)
0xb3a98441a3a98441 (diff = 44)
* 128-bits
0xd3a996e9c3a996e9b3a996e9a3a996e9 (diff = 0)
0xd3a99715c3a99715b3a99715a3a99715 (diff = 44)
0xd3a99741c3a99741b3a99741a3a99741 (diff = 44)
0xd3a9976dc3a9976db3a9976da3a9976d (diff = 44)
* The End
```
