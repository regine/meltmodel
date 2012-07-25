## MeltMod: A Glacier Mass Balance Model##

MeltMod is a fully distributed mass-balance model for mountain glaciers, developed by Regine Hock and  Carleen Tijm-Reijmer.

Short-term mass balance variations (ablation and accumulation) are calculated  by a selection of various energy-balance and temperature-index methods.

# Building #
Prerequisites:
* Cmake: MeltMod uses [cmake](http://www.cmake.org/) to build itself, any version >=2.8 should work
* Python 2, as well as the argparse module (included by default on versions >= 2.7)

To download the latest release use git:

    git clone git@github.com:fmuzf/melt_mod.git

Installation requirements:

  *  [Cmake](http://www.cmake.org/) version >= 2.8 must be installed and running.

  *  A C -compiler

  * [Python](http://www.python.org/) version 2.x

To build MeltMod locally (within the source tree), do

    $ cd melt_mod
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make && make test:

You should now have a working copy of all the executables needed to run the model.


Further information about MeltMod is available [here](http://www2.gi.alaska.edu/~regine/meltmodel.html).

