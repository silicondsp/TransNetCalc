

To use TCL:


1- cd to the location of transmissionlinenets. This should be the directory: transmissionlinenets
2- type pwd to get the path.
3- Assume  the path is  THE_PATH.
4- type the following in the directory:
        export LD_LIBRARY_PATH=THE_PATH/library
 
5- cd to the TCL directory.
6- Type 
         ./trc

The TCL shell command will show up.

You are ready to do transmission line analysis.

IF you change locations make sure you place the file:
        init.tcl
   Somewhere TCL can find it. It will give you a list of locations.


To test laod the tcl script: length_vswr.tcl
To do this run ./trc
When you get the '%' promt (TCL Shell) type: source length_vswr.tcl
The results are stored in the file vswr.dat.
type; exit to leave.

 
