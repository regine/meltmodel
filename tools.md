---
layout: front_page
title: DEBaM and DETIM
subtitle: Some Useful Tools For Use With the Models
---

The following tools were developed by Lyman Gillispie in August 2013:

* __[MultiRun](https://github.com/fmuzf/matlab_hk_MultiRun)__:
A Matlab tool for running the models (detim/debam) multiple times over ranges
of parameter values, and thereby making model calibration more efficient.
The tool generates a series of parameter files (```input.txt```) with different parameter values
and then runs the model with all parameter files. The parameters, their range and increment
are specified by the user. The tool requires Matlab.

* __[Glazer](https://github.com/fmuzf/matlab_hk_glazer)__:
A tool for importing and exporting DEBaM and DETIM
 ```input.txt``` files. Implementations exist for
[Matlab](https://github.com/fmuzf/matlab_hk_glazer)
and [Python](https://github.com/fmuzf/python_hk_glazer).
Glazer can read input.txt files into native data-structures,
as well as output them into valid configuration files.
