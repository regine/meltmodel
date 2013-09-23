A Note on the Organization of DETIM and DEBaM
---------------------------------------------

What's in the Folders?
----------------------

There are two sub-directories in the root folder of the project, one called ```src``` and the other ```util```. These two folders contain source code:
- ```src``` : Contains source code for the models.
- ```util``` : Contains source code for utilities which users may
find useful, such as
    * ```ascigrid``` and ```gridasci``` : these take ascii-grid files and
    convert them into the binary format used by the models, and
    can convert them back (developed by Joerg Schulla)
    * shading : generates grid files including shading and potential direct radiation
    based on your DEM.
    * ```visualization```: contains matlab files to visualize model input and
    output (only grids); programs ```grid``` (show grid) and ```grids2diff``` (show 2 grids and the difference)
    and parameter file for both programs ```inputreadgrid.m``` (setting paths, file names etc)

There is a third subdirectory, ```example```, which contains a sample dataset
and parameter file for running the model. Included are samples of a climate data input file and all grid files that are necessary to run the models: digital elevation models of the entire domain, the watershed (area to be computed) and the glacier, slope and aspect grids, firn and initial snow cover and skyview factor grids. 
Adjusted parameter files for the models, and shading tool are included
as well:

__input.txt__ : A sample parameter file for DEBaM and DETIM adjusted to the sample data

__shading.txt__: A sample parameter file for the shading utility.

Note that you need to adjust the input/output paths in the example parameter files and copy the parameter files to the
folder of the executables (folder bin). Then you should be able to run the example.
Grid files are given in binary format as required by the model, but also in asci-format. Sometimes the asci-files
need to be freshly converted into binary format on the platform you are working on (using ascigrid) 
for the model to run.


### After building the Model
Building the model creates two new subdirectories:
- ```build```: Our Makefile uses this directory to store the intermediate
build files produced by the ```c``` compiler; these will predominately be
```.o``` c-object files. The contents of this folder will probably be of
little interest to most users.
- ```bin```: When building an executable, out Makefile places the final
executable in this folder, this includes the model executeables, grid tools
and shading tool.


What're the Other Files For?
----------------------------

__.md files__ : Any of the .md files are simply markdown formatted text files, 
and can be opened and edited in any text editor of your choosing. A quick 
rundown on their contents:
* _README.md_ : Overview and readme for the project, the contents of this file
are also displayed on Github's repository page.
* _INSTALL.md_ : Detailed installation instructions, closely follows the content
of the installation instructions given on the webpage
* _changes.md_ : Rough change-log, if anything major changes, it's a good idea to
write it down here, under a new heading at the top, including the date.
* _organization.md_: this file, describing the organization of the project

__COPYING.txt__ : Includes a copy of the GNU General Public License v.3, this
is the license under which we are distributing the models.

__.gitignore__ : This file contains a list of files which git will not track.
If you'd like git to ignore a file, open this file in a text editor and add
it to the list. Run ```git status``` to make sure  git is in fact ignoring the
file. Similarly, if git doesn't appear to see a file that you know is there,
check to make sure git isn't ignoring it. To let git track a previously ignored
file, simply remove the file's entry from .gitignore.

__Makefile__ : The Makefile tells make what to do when building the models/utilities 

__input.txt__ : A sample parameter file for the programs debam and detim

__shading.txt__: A sample parameter file for the shading program

Note: parameter files need to be copied to the bin-directory and adjusted to your case prior to running the models
