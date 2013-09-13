--- 
layout: front_page 
title: DEBaM and DETIM 
subtitle: Two Distributed Glacier Surface Mass-Balance and Discharge Models
repo: http://github.com/regine/meltmodel
---

Overview
--------
Welcome to the homepage of two open source glacier surface mass-balance and discharge 
models:

- __D__istributed __E__nergy __Ba__lance __M__odel (DEBaM)
- __D__istributed __E__nhanced __T__emperature __I__ndex __M__odel (DETIM)


The models compute __glacier surface mass balance__ (ablation and accumulation) and
discharge, with hourly to daily resolution. 
The mass balance model is fully distributed, i.e. calculations are performed for each grid cell of a
digital elevation model. Discharge is calculated by three
linear reservoirs corresponding to the different storage properties of firn,
snow and glacier ice. __Discharge__ simulations are optional, i.e. the mass balance
model can be run independently of the discharge model. Glacier __retreat__ may also
be modeled using a simple volume-area scaling.
In addition, DEBaM can compute subsurface temperatures, water content and percolation
via a 1-D __multi-layer snow model__ that is forced by the surface energy balance.

__Minimum data requirements__ are a digital elevation model and hourly or daily air
temperature and precipitation data (DETIM) and temperature, precipitation, wind speed, relative humidity
and shortwave incoming radiation (DEBaM).

The models were initially developed by Regine Hock in the mid 1990s, and have
since been continuously expanded. In 2006, Carleen Tijm-Reijmer contributed
the subsurface snow model used by DEBaM. The models were made open source in August 2012
with help of Lyman Gillispie.

- __DEBaM__ computes surface melt by an energy balance approach
- __DETIM__ computes surface melt by various temperature index approaches. 


Download
--------
The models are hosted from our [github repository]({{%page.repo%}}).
You can download the source as zip or tar files via:

-  [meltmodel-release\_0.2.zip]({{%page.repo%}}/zipball/release_0.2)
-  [meltmodel-release\_0.2.tar.gz]({{%page.repo%}}/tarball/release_0.2)

Alternately, you can also clone the project with [Git](http://git-scm.com) by
running:

    $ git clone https://github.com/regine/meltmodel.git

from a terminal (i.e. open a terminal and copy the line above (git ....) into the command line). If you plan to modify the model source code, or contribute to the
project, this is the preferred method. More information about git can be
found at [git-scm](http://git-scm.com/) or at 
[Github](http://help.github.com/articles/).

The download will always include both DETIM and DEBaM. Most files are needed for both models.

Installation
------------

Prerequisites:

* GNU Make
* A working C compiler 

To build DeBAM and DETIM, do

    $ cd meltmodel
    $ make models

The model executables (detim and debam) will now be located in ```meltmodel/bin```

A more complete installation guide is available [here](install-web.html).

Documentation/Manual
---------------------
The most recent revision of the manual is available [here](http://gi.alaska.edu/~regine/meltmodel.html).


Literature And Previous Applications
------------------------------------
The models are described in detail in the following papers:

- DETIM : 
  1. Hock, R. (1999),
      A distributed temperature index ice and snow melt
      model including potential direct solar radiation, 
      *Journal of Glaciology*, *45*(149), 101-111.

- DEBaM : 
  1. Hock, R., and B. Holmgren (2005),
     A distributed energy balance model for complex
     topography and its application to Storglaciären, Sweden,
     *Journal of Glaciology*, *51*(172), 25-36.
  2. Reijmer, C. H., and R. Hock (2008),
     A distributed energy balance model including
     a multi-layer sub-surface snow model,
     *Journal of Glaciology*, *54*(184), 61-72.
  
List of references which are based on application of the source code is available [here](references.html)


Additional Information
----------------------

For further information contact [Regine Hock](http://gi.alaska.edu/~regine/),
(University of Alaska, Fairbanks) or Carleen Tijm-Reijmer 
(University Utrecht). Note that the model may
contain errors and the model manual may not be complete or outdated. User
support and further code improvements are available in direct collaboration
with us.

If you find errors or have suggestions how the manual (or model) can be improved, 
please feel free to let up know via the Github Issues page for the models or via email.

There have been tools developed to help with certain tasks related to the model;
a partial list of these may be found at the [External Tools page](tools.html)


Model updates and changes
-------------------------
The model constantly evolves. All significant model changes and updates since 2012 are logged [here]({{page.repo}}/tree/release_0.2/changes.md).
