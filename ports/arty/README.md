# The E310 Arty FPGA Dev Kit port

Port for E310 Arty FPGA Dev Kit board.

Use official SiFive tools from https://github.com/sifive/freedom-e-sdk/.

## Building 

    $ make
    
or if tools are not in $HOME/src/freedom-e-sdk
    
    $ make FREEDOM_E_SDK=/opt/freedom-e-sdk

## Uploading 

    $ make upload
