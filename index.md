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

- __DeTiM__ offers various temperature index methods approaches. Discharge is
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

-   [Meltmod-release\_0.2.zip]({{%page.repo%}}/zipball/release_0.2)
-   [Meltmod-release\_0.2.tar.gz]({{%page.repo%}}/tarball/release_0.2)
-   [Meltmod-dev.zip]({{%page.repo%}}/zipball/dev)
-   [Meltmod-dev.tar.gz]({{%page.repo%}}/tarball/dev)

You can also clone the project with [Git](http://git-scm.com) by
running:

    $ git clone git://{{ page.repo }}

Installation
------------

 
A  more complete installation guide is avilable [here](install.html).

Documentation/Manual
---------------------
...

Recent Changes
--------------------------------
...


Literature And Previous Applications
------------------------------------

3 main papers

List of other papers

Additional Information
----------------------

For further information contact Regine Hock, (University of Alaska, Fairbanks)
or Carleen Tijm-Reijmer (University Utrecht). Note that the model may
contain errors and the model manual may not be complete or outdated. User
support and further code improvements are available in direct collaboration
with us.
