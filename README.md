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
