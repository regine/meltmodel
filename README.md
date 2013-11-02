Overview
--------
- Distributed Energy Balance Model (DEBaM)
- Distributed Enhanced Temperature Index Model (DETIM)

### v2.0.0


The models compute glacier surface mass balance (ablation and accumulation) and
discharge, with hourly to daily resolution. 
The mass balance model is fully distributed, i.e. calculations are performed for each grid cell of a
digital elevation model. Discharge is calculated from the water provided by melt plus liquid precipitation by three
linear reservoirs corresponding to the different storage properties of firn,
snow and glacier ice. Discharge simulations are optional, i.e. the mass balance
model can be run independently of the discharge model. Glacier retreat may also
be modeled using a simple volume-area scaling.
In addition, DEBaM can compute subsurface temperatures, water content and percolation
by a 1-D multi-layer snow model that is forced by the surface energy balance.

Minimum data requirements are a digital elevation model and hourly or daily air
temperature and precipitation data (DETIM) and temperature, precipitation, wind speed, humidity
and shortwave incoming radiation (DEBaM).

The models were initially developed by Regine Hock in the mid 1990s and have
been continuously expanded since. The subsurface module has been added in 2006
by Carleen Tijm-Reijmer, Utrecht University.

- __DEBaM__ computes surface melt by an energy balance approach.

- __DETIM__ offers various temperature index methods approaches. 


Download
--------
Clone the project with [Git](http://git-scm.com) by
running:

    $ git clone https://github.com/regine/meltmodel.git

from a terminal. If you plan to modify the source, or contribute to the
project, this is the preferred method. More information about git can be
found at [git-scm](http://git-scm.com/) or at 
[Github](http://help.github.com/articles/).

Installation
------------

Prerequisites:

* A working C compiler 

To build DeBAM and DETIM, do

    $ cd meltmodel
    $ make all

The model executables will now be located in ```meltmodel/bin```
 
A more complete installation guide is available in install.md.

Additional Information
----------------------

For further information contact [Regine Hock](http://gi.alaska.edu/~regine/),
(University of Alaska, Fairbanks) or Carleen Tijm-Reijmer 
(University Utrecht). Note that the model may
contain errors and the model manual may not be complete or outdated. User
support and further code improvements are available in direct collaboration
with us.

If you find a glaring error or have suggestions how the manual can be improved, please feel free to let up know via the
Github Issues page for the models or via email.
