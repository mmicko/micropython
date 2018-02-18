# The LoFive port

Port for LoFive board.

Use tools from https://github.com/mwelling/freedom-e-sdk since they do include needed board definitions.

## Building 

    $ make
    
or if tools are not in $HOME/src/freedom-e-sdk
    
    $ make FREEDOM_E_SDK=/opt/freedom-e-sdk

## Uploading 

    $ make upload
