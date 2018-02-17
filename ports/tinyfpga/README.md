# The TinyFPGA-B2 port

Port for Lattice chip based Upduino, iceBreaker and similar boards.

## Building 

By default the port will build rtl code as well using yosys

    $ make

To deploy use

    $ make prog

To deploy just firmware use

    $ make prog_fw
