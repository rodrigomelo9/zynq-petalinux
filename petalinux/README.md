# How was the content of this directory generated

```
source /<PETALINUX_INSTALLATION>/settings.sh
petalinux-create -t project -n petalinux --template zynqMP
cd petalinux/
petalinux-config --get-hw-description=../vivado/
petalinux-create -t apps --name mytest --template c --enable
petalinux-config -c rootfs
```

* Changes in the rootfs: ``
  * *Filesystem Packages* -> *misc* -> *python3*
    * `python3`, `python3-io` and `python3-mmap` were selected
* `project-spec/meta-user/recipes-apps/myapp/files/mytest.c` was customized
* `project-spec/meta-user/recipes-bsp/device-tree/files/system-user.dtsi` was customized
  * UIO enabled for BRAM controllers 0 and 1
  * Memory reserved for BRAM controller 2
* `project-spec/meta-user/recipes-bsp/u-boot/files/platform-top.h`
  * Memory reserved for BRAM controller 2
