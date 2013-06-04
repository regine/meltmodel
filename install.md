---
layout: front_page 
title: DEBaM and DETIM
subtitle: Installation Guide
repo: http://github.com/regine/meltmodel
---


Prerequisites
-------------
Because the models are distributed only as source, there are a few
prerequisites needed before you can build and use the model. Namely:

* A working C compiler: Since the models is written in C, you'll need
    something to compile it.
* [Cmake](http://www.cmake.org): Is used to prepare and configure the compilation 

If you already have these on your machine, skip down to the section 
[Compiling](#compiling)


Setting up Mac OS X
---------------

1. To get a C compiler for OS X, you will need to obtain 
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
2. You have two options for installing CMake: 
* Install the binary OS X package from their
 [website](http://www.cmake.org/cmake/resources/software.html),
  or 
  * Use your choice of package managers for OS X, i.e. [Homebrew](http://mxcl.github.com/homebrew/),
  [Fink](http://www.finkproject.org/) or [MacPorts](http://www.macports.org/).

  If you're already using a package manager, we recommend you use it to install
  CMake.


Setting up Linux
----------------
This is perhaps the easiest situation in which you could find yourself,
not least because you probably already have everything you need.

1. Your distro should come packaged with a C compiler
2. Use your distribution's package manager to install CMake, e.g. __apt__ 
(Ubuntu, Debian, etc)

        $ apt-get install cmake

    or __rpm__ (Fedora and others)
        
        $ rpm -ivh cmake


Setting up Windows
---------------------------
On Windows, we have had success getting DEBaM and DETIM running using 
[Cygwin](http://www.cygwin.com/), a Linux-like system for Windows.
__Imprtant:__ If you choose to use Cygwin, you *must* install CMake using
Cygwin, otherwise CMake will be oblivious to the fact that you have
installed a compiler.

Each of the following instructions roughly corresponds to a single dialog
window of Cygwin's installer.

1. Download and run Cygwin's ```setup.exe``` installer; this will
   install Cygwin, as well as the compiler gcc and CMake.
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
8. Under "Select Packages" exapnd the "Devel" section by clicking
   on the "+" sign in the box.
  - Select ```gcc``` and ```cmake```.
9. Click "Next" and follow the remaining installation instructions.

10. Open Cygwin by either clicking the icon on your desktop, or
via the Start Menu. Cygwin will present itself as command prompt
window.

11. Proceed as below in the [Compiling](#compiling) section.

<a id="compiling"></a>
Compiling
---------

#### Getting The Source

Now you'll need to get a copy of the source code (We suggest you download the version release-0.2). 
The models are hosted from our [github repository]({{%page.repo%}}).
You can download the source as zip or tar files via:

-  [meltmodel-release\_0.2.zip]({{%page.repo%}}/zipball/release_0.2)
-  [meltmodel-release\_0.2.tar.gz]({{%page.repo%}}/tarball/release_0.2)
-  [meltmodel-dev.zip]({{%page.repo%}}/zipball/dev)
-  [meltmodel-dev.tar.gz]({{%page.repo%}}/tarball/dev)

Alternately, you can also clone the project with [Git](http://git-scm.com) by
running (command line in terminal):

    $ git clone https://github.com/regine/meltmodel.git

from a terminal. If you plan to contribute to the
project, you should use git, and spend some time getting
familiar with it. More information about git can be
found at [git-scm](http://git-scm.com/) or at 
[Github](http://help.github.com/articles/).


#### Compiling

Open up a command-line window and navigate to the folder containing the 
model source files.

At the command prompt, do

    $ cd meltmodel
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make install
    $ cd ..

The model executables will now be located in ```meltmodel/bin```.

For information regarding running the model, please consult
the manual, located at [Regine Hock's page](http://gi.alaska.edu/~regine/meltmodel).

