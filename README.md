Overview
--------
Welcome to the homepage of two open source glacier mass-balance and discharge 
models:
- Distributed Energy Balance Model (DEBaM)
- Distributed Enhanced Temperature Index Model (DeTIM)

The models compute glacier mass balance (ablation and accumulation) and
discharge, with hourly to daily resolution. The mass balance model is
fully distributed, i.e. calculations are performed for each grid cell of a
digital elevation model.

- __DEBaM__ computes surface melt by an energy balance approach. In addition,
subsurface temperatures, water content and percolation can be computed by a
one-dimensional multi-layer snow model that is forced by the surface energy
balance.

- __DeTiM__ offers various temperature index methods approaches. Discharge is
calculated from the water provided by melt plus liquid precipitation by three
linear reservoirs corresponding to the different storage properties of firn,
snow and glacier ice. Discharge simulations are optional, i.e. the mass balance
model can be run independently of the discharge model. Glacier retreat may also
be modeled using a simple volume-area scaling.

Minimum data requirements are a digital elevation model and hourly or daily air temperature and precipitation data. The models were initially developed by Regine Hock in the mid 1990s and have been continuously expanded since. The subsurface module has been added in 2006 by Carleen Tijm-Reijmer, Utrecht University.


Installation
------------
Prerequisites:
* Cmake: MeltMod uses [Cmake](http://www.cmake.org/) to build itself,
    any version >=2.8 should work
* A working C compiler 

To download the latest release use git:

    git clone https://github.com/fmuzf/melt_mod.git

To build MeltMod, do

    $ cd melt_mod
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make && make install:
    $ cd ..

The binaries for MeltMod will now be located in melt_mod/bin

A more complete installation guide is available [here](http://fmuzf.github.com/melt_mod/install.html).

Documentation/Manual
---------------------
The most recent manual version is available [here](http://gi.alaska.edu/~regine)

Recent Changes
--------------------------------
See the [changelog](https://github.com/fmuzf/melt_mod/tree/release_0.2/doc).



Additional Information
----------------------

For further information contact [Regine Hock](http://gi.alaska.edu/~regine), (University of Alaska, Fairbanks) or Carleen Tijm Reijmer (University Utrecht). Note that the model may contain errors and the model manual may be incomplete or outdated. User support and further code improvements are available in  direct collaboration with us.


