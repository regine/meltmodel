## MeltMod: A Glacier Mass Balance Model##

MeltMod is a fully distributed mass-balance model for mountain glaciers, 
developed by Regine Hock and  Carleen Tijm-Reijmer.

Short-term mass balance variations (ablation and accumulation) are calculated
by a selection of various energy-balance and temperature-index methods.

# Building #
Prerequisites:
* Cmake: MeltMod uses [Cmake](http://www.cmake.org/) to build itself,
    any version >=2.8 should work
* A working C compiler 

To download the latest release use git:

    git clone git@github.com:fmuzf/melt_mod.git

To build MeltMod, do

    $ cd melt_mod
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make && make install:
    $ cd ..

The binaries for MeltMod will now be located in melt_mod/bin

Further information about MeltMod is available 
[here](http://www2.gi.alaska.edu/~regine/meltmodel.html).

# License #
GPL v3 or greater

