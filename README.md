## MeltMod: A Glacier Mass Balance Model##

MeltMod is a fully distributed mass-balance model for mountain glaciers, developed by Regine Hock and  Carleen Tijm-Reijmer.

Short-term mass balance variations (ablation and accumulation) are calculated  by a selection of various energy-balance and temperature-index methods.

# Building #
Prerequisites:
* Cmake: MeltMod uses [cmake](http://www.cmake.org/) to build itself, any version >=2.8 should work
* Python 2, as well as the argparse module (included by default on versions >= 2.7)

Building:
- Download the source: 
    *either via git ```git clone https://github.com/fmuzf/melt_mod.git```
    *or just download the zip file, then unzip it
- Navigate to the project root : ```cd melt_mod```
- Make an out-of-source build directory: ```mkdir build```
- Navigate to the build directory: ```cd build```
- Run cmake to generate the make file: ```cmake ..```
- Run make and test to build MeltMod: ```make && make test```

You should now have a working copy of all the executables needed to run the model.


Further information about MeltMod is available [here](http://www2.gi.alaska.edu/~regine/meltmodel.html).

