---
layout: front_page 
title: DEBaM and DeTIM
subtitle: Installation Guide
repo: http://github.com/fmuzf/melt_mod
---


Programs needed by the Models
--------------------------

* A working C compiler: Since the models is written in C, you'll need
    something that can compile it.
* [Cmake](www.cmake.org): Is used to configure the models's build 


Building DeTIM and DEBaM on OSX
-----------------------

1. DeTIM and DEBaM are distributed as source code only, so you will need to obtain 
Apple's software development tool [Xcode](https://developer.apple.com/xcode/).
Included with Xcode are the compilers needed to build DeTIM and DEBaM.
    
    * The easiest way to obtain Xcode is via the App Store, depending on your
    internet connection, it may take some time for Xcode to download.

    * Once Xcode has downloaded, open the Xcode Application from your
    Applications folder. Follow the instructions asked by the dialog boxes.

    * After Xcode has finished installing, one further step is needed to install
    the command-line tools:

        - Open Xcode. Open the menu Xcode > Preferences
        
        - Open the Downloads tab in the Preferences
        
        - In the Downloads list, select Command Line Tools, and push the
        install button 

2. We use [CMake](cmake.org) to configure DeTIM and DEBaM's build. You may either
install CMake from their [website](http://www.cmake.org/cmake/resources/software.html),
or using your choice of package managers for OS X, i.e. [Homebrew](mxcl.github.com/homebrew/),
[Fink](http://www.finkproject.org/) or [MacPorts](http://www.macports.org/).
If you're already using a package manager, we recommend you use it to install
CMake


Building DeTIM and DEBaM on Linux
-------------------------
2. Your distro should come packaged with a C compiler
1. Use your distribution's package manager to install CMake, e.g. __apt__ 
(Ubuntu, Debian, etc)

        $ apt-get install cmake

    or __rpm__ (Fedora and others)
        
        $ rpm -ivh cmake

Building DeTIM and DEBaM on Windows
---------------------------
1. Cygwin

2. Visual Studio
