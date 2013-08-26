08/26/2013
----------
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

Major changes since 11/2011
---------------------------

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
