---
layout: front_page 
title: DEBaM and DETIM
subtitle: Installation Guide (Version 1.0.0)
repo: http://github.com/regine/meltmodel
---

Prerequisites
-------------
Because the models are distributed only as source, there are a few
prerequisites needed before you can build and use the model. Namely:

* A working C compiler: Since the models is written in C, you'll need
    something to compile it.
* Gnu Make

If you already have these on your machine, skip down to the section 
[Compiling](#compiling)


Setting up Mac OS X
---------------

1. To get a C compiler and Make for OS X, you will need to obtain 
Apple's software development tool [Xcode](https://developer.apple.com/xcode/).
Included with Xcode are the compilers needed to build DEBaM and DETIM.
  * The easiest way to obtain Xcode is via the App Store. It's pretty big,
  so do this somewhere with a fast internet connection. 
  * Once it's downloaded, open Xcode from your Applications folder.
  Follow the instructions asked by the dialog boxes.
  * After Xcode has finished installing, one further step is needed to install
  the command-line tools:
      - Open the menu Xcode > Preferences
      - Open the Downloads tab in the Preferences
      - In the Downloads list, select Command Line Tools, and push the
      install button
2. You're done!
 
Setting up Linux
----------------
This is perhaps the easiest situation in which you could find yourself,
not least because you probably already have everything you need.

Many distros come packaged with a C compiler and Make, but a notable few
don't. If you have the following, run the commands below at the command line

- __Ubuntu/Debian__:

        $ apt-get install build-essential

- __Fedora__:

        $ yum install make automake gcc gcc-c++ 

- __Arch__:

        $ pacman -S base-devel

 
Setting up Windows
---------------------------
On Windows, we have had success getting DEBaM and DETIM running using 
[Cygwin](http://www.cygwin.com/), a Linux-like system for Windows.

Each of the following instructions roughly corresponds to a single dialog
window of Cygwin's installer.

1. Download and run Cygwin's ```setup.exe``` installer; this will
   install Cygwin, as well as the compiler gcc an Make.
2. Click "Next" at the first prompt.
3. When prompted to "Choose A Download Source" select "Install from Internet"
4. Choose your Root Install Directory: unless you are really sure,
   leave this set to the default.
5. Ditto for "Select Local Package Repository"
6. Under "Select Connection Type", let Cygwin know how to connect to
   the internet (usually "Direct Connection").
7. Under "Select a Mirror" it's fine to do nothing here. If your
   download speeds really crawl, you can cancel the installation
   and change this. 
8. Under "Select Packages" expand the "Devel" section by clicking
   on the "+" sign in the box.
  - Select ```gcc```, and ```make```.
9. Click "Next" and follow the remaining installation instructions.

10. Open Cygwin by either clicking the icon on your desktop, or
via the Start Menu. Cygwin will present itself as command prompt
window.

11. Proceed as below in the [Compiling](#compiling) section.

<a id="compiling"></a>
Compiling
---------

#### Getting The Source

Now you'll need to get a copy of the source code (We suggest you download the version master). 
The models are hosted from our [github repository]({{%page.repo%}}).
You can download the source as zip or tar files via:

-  [meltmodel-master.zip]({{%page.repo%}}/zipball/master)
-  [meltmodel-master.tar.gz]({{%page.repo%}}/tarball/master)

Alternately, you can also clone the project with [Git](http://git-scm.com) by
running:

    $ git clone https://github.com/regine/meltmodel.git

from a terminal (i.e. open a terminal and copy the line above (git ....) into the command line). 
If you plan to contribute to the
project and change or add to the source code, you should use git, and spend some time getting
familiar with it. More information about git can be
found at [git-scm](http://git-scm.com/) or at 
[Github](http://help.github.com/articles/).

Note, that the ___master version___ will always be the most up-to-date model version. Earlier
version are called 'release_XXX' with XXX denoting the release number. 
Older versions of the model are marked in git with [tags](http://git-scm.com/book/en/Git-Basics-Tagging)
and can be [reviewed on Github](https://github.com/regine/meltmodel/releases) under 'Branch'.

#### Compiling

Open up a command-line window and navigate to the folder containing the 
model source files.

At the command prompt, do

    $ cd meltmodel
    $ make models

The model executables and required input parameter files will now be located in ```meltmodel/bin```. 
The model source files are under ```meltmodel/src```.
A detailed description of the folders and files after installation which are under the folder ```meltmodel``` 
is given [here]({{page.repo}}/tree/master/organization.md).
.

The Makefile provides several different build targets, for those who
only wish to build parts of the project. These targets are built by
running ```make <target_name>``` at the command line, where ```<target_name>```
is one of the following:

- __debam__ or __detim__: Either of these builds _only_ the model
- __models__: builds both models
- __ascigrid__ or __gridasci__ or __gridtools__: ascigrid and gridasci build the individual executables, grid tools builds both
- __shading__: builds the shading tool
- __utils__: builds both shading and the grid tools
- __all__: builds everything
- __clean__: deletes all built executables and intermediate build products. 

For information regarding running the model, please consult
the model manual, located at [Regine Hock's page](http://gi.alaska.edu/~regine/meltmodel).

#### Additional Model Tools

Additional tools have been developed to help with certain tasks related to the model, for example,
making model calibration more efficient. They are described and can be downloaded at the [External Tools page](tools.html).
