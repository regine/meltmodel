---
layout: front_page 
title: DEBaM and DeTIM
subtitle: Installation Guide
repo: http://github.com/fmuzf/melt_mod
---


Programs needed by the Models
--------------------------
Because the models are distributed only as source, there are a few
prerequisites needed before you can build and use the model. Namely:

* A working C compiler: Since the models is written in C, you'll need
    something to compile it.
* [Cmake](www.cmake.org): Is used to configure the models' build to your system.

If you already have these on your machine, skip down to the section 
"Compiling the Models"


Setting up OS X
---------------

1. To get a C compiler for OS X, you will need to obtain 
Apple's software development tool [Xcode](https://developer.apple.com/xcode/).
Included with Xcode are the compilers needed to build DeTIM and DEBaM.
    
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

* Install the binary OS X package from their website:
 [website](http://www.cmake.org/cmake/resources/software.html),
or 
* Use your choice of package managers for OS X, i.e. [Homebrew](mxcl.github.com/homebrew/),
[Fink](http://www.finkproject.org/) or [MacPorts](http://www.macports.org/).

If you're already using a package manager, we recommend you use it to install
CMake.


Setting up Linux
----------------
This is perhaps the easiest situation in which you could find yourself,
not least because you probably already have everything you need.

2. Your distro should come packaged with a C compiler
1. Use your distribution's package manager to install CMake, e.g. __apt__ 
(Ubuntu, Debian, etc)

        $ apt-get install cmake

    or __rpm__ (Fedora and others)
        
        $ rpm -ivh cmake

Setting up Windows
---------------------------
1. Cygwin

2. Visual Studio


Compiling the Models
--------------------

