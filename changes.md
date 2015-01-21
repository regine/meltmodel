NOTE: Changes in the code can be due to
* a) errors  (_ERROR_)
* b) new features that offer major new options/functionality (_NEW_)
* c) program technical improvements with no effect on results (_TECHNICAL_).

The term in brackets is used to mark each change.

#### 21 January 2015: (ERROR, L. Gillispie)
* v2.1.1
  - fixes an un-allocated pointer error which caused segmentation faults in certain circumstances

#### 15 January 2015: (NEW, C. Reijmer)
* v2.1.0
  - changes in subsurface model: renmaing variables and writing more variables to output so that the water and energy balance can be tracked better.

#### 6 November 2013: (NEW, R. Hock)
* v2.0.2
 - change in output file "modelperformance.txt", discharge volume discharge is set to -9999 if discharge measurements contain missing values during simulation period (if nstepsdis < nsteps)

#### 5 November 2013: (ERROR, R. Hock)
* v2.0.1
 - ERROR fixed in computing number of valid time steps of discharge data. Affects r2 slightly and cumulative measured discharge volume in discharge output. (Version number not changed).


#### 10/31/2013 (NEW, ERROR, R. Hock)
* v2.0.0
- CHANGE IN input.txt: all parameters removed that had to do with parameter optimization (between discharge and snow model parameters)
- NEW OUTPUT: time series of glacierwide ice heat flux, mass balance and cumulative mass balance added to 'areamean.txt'
- NEW OUTPUT: file with modeled and measured point balances ('pointbalances.txt'). A file with measured balances (measuredpointbalances.txt) is read and the program computes the pointbalances for the same locations and periods each stake's mass balance refers to.
- ERROR FIXED: cumulative mass balances for stakes (cummassbal.txt) was reset to 0 at end of mass balance year. Fixed so that it is continuous.
- ERROR FIXED: precipitation correction factor was not applied when precipitation was read from file for energy balance model.
- TECHNICAL: filenames of default output files changes:
     melting.txt        → cumablation.txt,
     specificmassbal.txt → seasonalmassbal.txt,
     res.out  	        → modellog.txt
- TECHNICAL: NEW: Longwave incoming radiation grid array is used even if LWin is spatially constant

#### 10/20/2013 (NEW, R. Hock)
* v1.1.0
- Added "model performance" output file which records one line of discharge r^2 and r^2 (ln) values and other parameters
to the text file "model_performance.txt"

#### 10/07/2013  (TECHNICAL, R. Hock)
* v1.0.1
  Some values of the 'hidden options' in variab.h have been changed to reflect a more general case.

#### 09/20/2013 (ERROR, C. Reijmer)

* v1.0.1
   Bug fixed in the skin layer formulation responsible for the lack of closure of the surface energy balance as found in the output.
- Added MBsum in order to have mass balance output consistent with other mass balance parameters output.
- Made changes (together with Torbjorn Ostby) to functions meltlayermice and resetgridwinter to remove
some bugs related to the sr50 output. Correspondence between mass balance output and sr50 has improved.
- Made changes to functions subsurf and surftempskin to correct a bug resulting in wrong surface temperature to be used to calculate the sensible and latent heat flux. As a result energy balance was not 0 for T<0. This is fixed now.

#### 09/18/2013 (L. Gillispie)
* v1.0.0 Versioning started

#### 09/11/2013  (TECHNICAL, L. Gillispie)
* Switched build system from CMake to Makefile.


#### 08/26/2013  (TECHNICAL, L. Gillispie)
* File and path name-lengths have been made uniform. There are three preprocessor
  macros, located in 'variab.h' and 'variabex.h' called:
    - ```FNAME_LEN``` : Length of filenames. Set currently to 80.
    - ```PNAME_LEN``` : Length of paths/directory names. Set to 201.
    - ```PFNAME_LEN``` : Max length of fully qualified path+filenames,
      this should be set equal to ```FNAME_LEN + PNAME_LEN```, currently
      set to 281.
  To declare a new string, for say a filename, do the following:
  ```c
    char filename[FNAME_LEN];
    char pathtofile[PNAME_LEN];
    char fullfilename[PFNAME_LEN];
  ```

#### 6/21/2013  (NEW, R. Hock)
* Added added glacierwide cumulative balances to areamean.txt; increase array sizes of names in variab.h
  Purpose: allow comparison e.g. with GRACE time series; no change in
  input.txt, column is added to areamean.txt;
  Before: mass balance only computed if if( ((winterbalyes == 1) &&
  (summerbalyes == 1)) || (maxmeltstakes > 0) ), NEW: if condition
  removed.

#### 5/23/2013  (NEW, C. Reijmer)
* Subsurface model: Added option to use skin-Temperature calculations in computing surface temperatures.
  Use of this option requires changing paramters in variab.h and recompile code ("hidden parameters").

#### 11/4/2012  (TECHNICAL, L. Gillispie)
* Fixed mismatching declarations in headers.

#### 8/31/2012
* Configuration filenmae changed from ```input.dat``` to ```input.txt```.
* Added sample ```shading.txt``` shading configuration file.
* Added Matlab visualization functions ```inputreadagrid.m``` and ```grids2diff.m```,
* Updated Matlab vis tool ``grid.m```

#### 8/28/2012
* Added sample ```input.dat``
* Added Matlab grid visualization tool.

#### Major changes since 11/2011 (L. Gillispie)

* Github hosting!

* Executable rename:
    - ```degree``` is now known as the Distributed Enhanced Temperature Index
    Model (DeTIM), and has executable ```detim```

    - ```meltmod``` is now know as the Distributed Energy Balance Model (DEBaM),
    and has executable ```debam```


* Build system in place! MeltMod now uses cmake to configure its builds.
    - Adds portability, cmake generates makefiles with the current environment
        in mind.
    - Out of source builds: Cmake will build everything in a folder outside of
        the source tree, and thus keep the source tree unmuddied by built files.
    - All of the relevant tools: ascigrid/gridasci/shading can be build via
        the same cmake build system
    - That being said, there have been some issues with cmake. In particular,
        since meltmod's code contains many circular dependencies, these can
        cause the model to fail to build unexpectedly. (See issue #16 for some
        discussion of this)

* Multiple headers: every source file now has its own header file, which is included where needed.
    - Speeds up compile time when changes occur only in a few files
    - Makes the control structure of the program more clear
    - Allows us to make certain functionality more modular

* Bug fixes: closed the following issues
    - issue #10 : ``readprecipindexmap`` was reading the wrong file, this is fixed
    - issue #11 : ``shayes`` is turned off when executing the temperature index model
    - issue #12 : ``methodsurftemp`` is immediately set to ``1`` when running the
        temperature index model, this was happening later, and screwing up some
        initialization parameters
    - issue #13 : fixed typo in snow-cover initialization
    - issue #14 : fixed bug in simulation mode, wherein large arrays were getting allocated
        but never used

Which brings us to...

* Project Reorganization:
    - All model source files are now located in the ```src/``` folder in the
        project root
    - Utility programs related to the project are located in the ```util/``` folder
    - Tests and their related file are located in the ```test/``` folder

* Code reorganization
    - All functions relating to matrix and tensor manipulation have been moved into
        it's own file: grid.c/h and included where necessary. Before, these were
        spread through a few different files, having them in one place makes sense.
    - issue #9: Redundant long ints ```nrl,nrh,ncl,nch,ndl``` and ```ndh``` used for
        matrix/tensor sizing were removed, matrix
        allocation and deallocation now take integers as arguments.
