--- 
layout: front_page 
title: DEBaM and DeTIM 
subtitle: Two Distributed Surface Mass Balance Models
repo: http://github.com/fmuzf/melt_mod
---

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

- __DeTIM__ offers various temperature index methods approaches. Discharge is
calculated from the water provided by melt plus liquid precipitation by three
linear reservoirs corresponding to the different storage properties of firn,
snow and glacier ice. Discharge simulations are optional, i.e. the mass balance
model can be run independently of the discharge model. Glacier retreat may also
be modeled using a simple volume-area scaling.

Minimum data requirements are a digital elevation model and hourly or daily air
temperature and precipitation data.
The models were initially developed by Regine Hock in the mid 1990s and have
been continuously expanded since. The subsurface module has been added in 2006
by Carleen Tijm-Reijmer, Utrecht University.

Download
--------
The models are hosted from our [github repository]({{%page.repo%}}).
You can download the source as zip or tar files via:
-   [Meltmod-release\_0.2.zip]({{%page.repo%}}/zipball/release_0.2)
-   [Meltmod-release\_0.2.tar.gz]({{%page.repo%}}/tarball/release_0.2)
-   [Meltmod-dev.zip]({{%page.repo%}}/zipball/dev)
-   [Meltmod-dev.tar.gz]({{%page.repo%}}/tarball/dev)

Alternately, you can also clone the project with [Git](http://git-scm.com) by
running:

    $ git clone https://github.com/fmuzf/melt_mod.git

from a terminal. If you plan to modify the source, or contribute to the
project, this is the preferred method. More information about git can be
found at [git-scm](http://git-scm.com/) or at 
[Github](help.github.com/articles/).

Installation
------------

Prerequisites:

* [Cmake](http://www.cmake.org/) is used to configure the build,
    any version >= 2.8 should work
* A working C compiler 

To build MeltMod, do

    $ cd melt_mod
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make install:
    $ cd ..

The binaries for MeltMod will now be located in melt_mod/bin
 
A more complete installation guide is available [here](install.html).

Documentation/Manual
---------------------
The most recent revision of the manual is available [here](http://gi.alaska.edu/~regine/meltmodel.html).


Literature And Previous Applications
------------------------------------
Details of the models can be be found in the following papers:

- DeTIM : {% bibliography tind_core %}
- DEBaM : {% bibliography ebal_core %}


Additional Information
----------------------

For further information contact [Regine Hock](http://gi.alaska.edu/~regine/),
(University of Alaska, Fairbanks) or Carleen Tijm-Reijmer 
(University Utrecht). Note that the model may
contain errors and the model manual may not be complete or outdated. User
support and further code improvements are available in direct collaboration
with us.

If you find a glaring error, please feel free to let up know via the
Github Issues page for the models.

Look at our [changelog]({{page.repo}}/tree/release_0.2/doc/changes.md) for recent updates.



References Citing the Models
-----------------------------

__Temperature Index Model: DeTIM__
{% bibliography tindex %}

__Energy Balance Model: DEBaM__
{% bibliography energy_bal %}
