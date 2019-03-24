# The Freedom-E-SDK port

Port for Freedom-E-SDK supported boards.

Use official SiFive tools from https://github.com/sifive/freedom-e-sdk/.

## Building 

    $ make
    
or if tools are not in $HOME/src/freedom-e-sdk
    
    $ make FREEDOM_E_SDK=/opt/freedom-e-sdk

If RISCV tools are not in path just set RISCV_PATH in make parameters.
Same for openocd binary (use RISCV_OPENOCD_PATH in that case)

    $ make RISCV_PATH=/opt/riscv64  RISCV_OPENOCD_PATH=/opt/riscv-openocd

## Supported boards

By default it builds for sifive-hifive1, but it also supports two additional cores.

    sifive-hifive1
    sifive-hifive1-revb
    freedom-e310-arty
    coreip-e20-arty
    coreip-e20-rtl
    coreip-e21-arty
    coreip-e21-rtl
    coreip-e24-arty
    coreip-e24-rtl
    coreip-e31-arty
    coreip-e31-rtl
    coreip-e34-arty
    coreip-e34-rtl
    coreip-e76-arty
    coreip-e76-rtl
    coreip-s51-arty
    coreip-s51-rtl
    coreip-s54-arty
    coreip-s54-rtl
    coreip-s76-arty
    coreip-s76-rtl

## Uploading 

    $ make upload

For core IP boards just add upload at the end of line:

    $ make BOARD=coreip-e31-arty upload

    $ make BOARD=coreip-s51-arty upload
