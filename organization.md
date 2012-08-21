A Note on the Organization of DETIM and DEBaM
---------------------------------------------

What's in the Folders?
----------------------

You've probably noticed that there are two sub-directories
in the root folder of the project, one called 'src' and the
other 'util'. These two models contain source code:
- src : Contains source code for the models.
- util : Contains source code for utilities which users may
find useful, such as
    * ascigrid and gridasci : these take ascii-grid files and
    convert them into the binary format used by the models, and
    can convert them back.
    * shading : generates shade and theoretical incoming radiation
    based on your DEM.


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

__COPYING.txt__ : Includes a copy of the GNU General Public License v.3, this
is the license under which we are distributing the models.

__.gitignore__ : This file contains a list of files which git will not track.
If you'd like git to ignore a file, open this file in a text editor and add
it to the list. Run ```git status``` to make sure  git is in fact ignoring the
file. Similarly, if git doesn't appear to see a file that you know is there,
check to make sure git isn't ignoring it. To let git track a previously ignored
file, simply remove the file's entry from .gitignore.

__CMakeLists.txt__ :  CMake uses this file, and every file in the project with
this filename, as configuration files. This file tells CMake what to do, 
which files and subdirectories to track, and what the compiler should do with
them. Altering these directly affects the manner in which the model is compiled,
so care is advised when editing these.