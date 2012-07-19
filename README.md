## MeltMod: A Glacier Mass Balance Model##

MeltMod is a fully distributed mass-balance model for mountain glaciers, developed by Regine Hock and  Carleen Tijm-Reijmer.

Short-term mass balance variations (ablation and accumulation) are calculated  by a selection of various energy-balance and temperature-index methods.

# Building #
Prerequisites:
* Cmake: MeltMod uses [cmake](http://www.cmake.org/) to build itself, any version >=2.8 should work

Building:
1. Download the source: 
    * either via git ```git clone https://github.com/fmuzf/melt_mod.git```
    * or just download the zip file, then unzip it
2. Navigate to the project root : ```cd melt_mod```
3. Make an out-of-source build directory: ```mkdir build```
4. Navigate to the build directory: ```cd build```
5. Run cmake to generate the make file: ```cmake ..```
5 Run make and test to build MeltMod: ```make && make test```

You should now have a working copy of all the executables needed to run the model.


Further information about MeltMod is available [here](http://www2.gi.alaska.edu/~regine/meltmodel.html).

