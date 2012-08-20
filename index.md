--- 
layout: front_page 
title: DEBaM and DETIM 
subtitle: Two Distributed Glacier Surface Mass-Balance and Discharge Models
repo: http://github.com/fmuzf/melt_mod
---

Overview
--------
Welcome to the homepage of two open source glacier surface mass-balance and discharge 
models:
- __D__istributed __E__nergy __Ba__lance __M__odel (DEBaM)
- __D__istributed __E__nhanced __T__emperature __I__ndex __M__odel (DETIM)


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
The models are hosted from our [github repository]({{%page.repo%}}).
You can download the source as zip or tar files via:
-   [meltmod-release\_0.2.zip]({{%page.repo%}}/zipball/release_0.2)
-   [meltmod-release\_0.2.tar.gz]({{%page.repo%}}/tarball/release_0.2)
-   [meltmod-dev.zip]({{%page.repo%}}/zipball/dev)
-   [meltmod-dev.tar.gz]({{%page.repo%}}/tarball/dev)

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

To build DeBAM and DETIM, do

    $ cd melt_mod
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make install
    $ cd ..

The model executables will now be located in ```melt_mod/bin```
 
A more complete installation guide is available [here](install.html).

Documentation/Manual
---------------------
The most recent revision of the manual is available [here](http://gi.alaska.edu/~regine/meltmodel.html).


Literature And Previous Applications
------------------------------------
The models are described in detail in the following papers:

- DETIM : {% bibliography tind_core %}
- DEBaM : {% bibliography ebal_core %}

List of references which are based on application of the source code is available [here](references.html)



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

Look at our [changelog]({{page.repo}}/tree/release_0.2/doc/changes.md) for recent updates.
