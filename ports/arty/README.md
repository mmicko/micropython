# The Arty FPGA Dev Kit port

Port for Arty FPGA Dev Kit board.

Use official SiFive tools from https://github.com/sifive/freedom-e-sdk/.

## Building 

    $ make
    
or if tools are not in $HOME/src/freedom-e-sdk
    
    $ make FREEDOM_E_SDK=/opt/freedom-e-sdk


## Supported boards

By default it builds form freedom-e300-arty, but it also supports two additional evaluation cores.

For 32-bit E31 IP core:

    $ make BOARD=coreplexip-e31-arty

For 64-bit E51 IP core:

    $ make BOARD=coreplexip-e51-arty

## Uploading 

    $ make upload

For coreplex IP boards just add upload at the end of line:

    $ make BOARD=coreplexip-e31-arty upload

    $ make BOARD=coreplexip-e51-arty upload
