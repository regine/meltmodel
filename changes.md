Note: Changes in the code can be due to
a) errors  (ERROR)
b) new features that offer new options/functionality (NEW)
c) program technical improvements with no effect on results (TECHNICAL).
The term in brackets is used to mark each change.



### 10/07/2013  (TECHNICAL, R. Hock)
* v1.0.1
  Some values of the 'hidden options' in variab.h have been changed to reflect a more general case.

### 09/20/2013 (ERROR, C. Reijmer)

* v1.0.1
   Bug fixed in the skin layer formulation responsible for the lack of closure of the surface energy balance as found in the output.
- Added MBsum in order to have mass balance output consistent with other mass balance parameters output.
- Made changes (together with Torbjorn Ostby) to functions meltlayermice and resetgridwinter to remove 
some bugs related to the sr50 output. Correspondence between mass balance output and sr50 has improved.
- Made changes to functions subsurf and surftempskin to correct a bug resulting in wrong surface temperature to be used to calculate the sensible and latent heat flux. As a result energy balance was not 0 for T<0. This is fixed now.

### 09/18/2013
* v1.0.0

### 09/11/2013  (TECHNICAL)
* Switched build system from CMake to Makefile (L. Gillispie).


### 08/26/2013  (TECHNICAL, L. Gillispie)
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

### 6/21/2013  (NEW, R. Hock)
* Added added glacierwide cumulative balances to areamean.txt; increase array sizes of names in variab.h
  Purpose: allow comparison e.g. with GRACE time series; no change in
  input.txt, column is added to areamean.txt;
  Before: mass balance only computed if if( ((winterbalyes == 1) &&
  (summerbalyes == 1)) || (maxmeltstakes > 0) ), NEW: if condition
  removed.

### 5/23/2013  (NEW, C. Reijmer)
* Subsurface model: Added option to use skin-Temperature calculations in computing surface temperatures.
  Use of this option requires changing paramters in variab.h and recompile code ("hidden parameters").

### 11/4/2012  (TECHNICAL, L. Gillispie)
* Fixed mismatching declarations in headers.

### 8/31/2012
* Configuration filenmae changed from ```input.dat``` to ```input.txt```.
* Added sample ```shading.txt``` shading configuration file.
* Added Matlab visualization functions ```inputreadagrid.m``` and ```grids2diff.m```,
* Updated Matlab vis tool ``grid.m```

### 8/28/2012
* Added sample ```input.dat``
* Added Matlab grid visualization tool.

### Major changes since 11/2011 (L. Gillispie)

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
