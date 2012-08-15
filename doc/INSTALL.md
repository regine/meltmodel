### Welcome to MeltMod!
All information about MeltMod can be found at <http://www2.gi.alaska.edu/~regine/meltmodel.html>

### Major installation requirements:
  *  A C compiler
  *  [Cmake](http://www.cmake.org/) >= 2.8 must be installed and running.

#### 1. A C compiler
- __Mac OS X__: You will need to download and install [XCode](https://developer.apple.com/xcode)

- __Linux__: Your distro should include this, if not use your package manager to install gcc and g++.
    
#### 2. [Cmake](http://www.cmake.org/)
- __OS X__ : We recommend you use a package manager such as [Homebrew](http://mxcl.github.com/homebrew/)
(or [MacPorts](http://www.macports.org/), or [Fink](http://www.finkproject.org/)) to obtain cmake.

    If you're using Homebrew, do

        $ brew install cmake

- __Linux__ : Use your distribution's package manager to install cmake, e.g. apt (Ubuntu, Debian, etc)

        $ apt-get install cmake

    or __rpm__ (Fedora and others)
        
        $ rpm -ivh cmake

#### 3. Get MeltMod

You have two options for obtaining MeltMod:
    1. Download the source zipball from [Github](http://github.com/fmuzf/melt_mod/), unpack it, and build
    2. Use [git]() to download the source.

We recommend using git, a powerful version control system. By using git, 
importing changes and bug fixes from our repository becomes easy. Furthermore, if you
find and fix any bugs, or have any changes you'd like to make to the model, git provides
a relatively streamlined way to do this.

To install git, use whichever package manager your OS provides:

- __OS X__ : If you're using Homebrew, you already have git. If you're using MacPorts
or Fink, install git using these.

- __Linux__ : Use your distribution's package manager to install cmake, e.g. apt (Ubuntu, Debian, etc)

        $ apt-get install git

    or __rpm__ (Fedora and others)
        
        $ rpm -ivh git


To download the latest release using git:

    git clone git@github.com:fmuzf/melt_mod.git

#### 3. Build MeltMod
To build MeltMod, open a terminal window and navigate to the directory containing the downloaded MeltMod files.

    $ cd melt_mod
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make && make install
    $ cd ..

The binaries for the model should be located in melt_mod/bin/
