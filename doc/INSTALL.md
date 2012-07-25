Welcome to MeltMod!

All information about MeltMod can be found at

<http://www2.gi.alaska.edu/~regine/meltmodel.html>
   
To download the latest release use git:

    git clone git@github.com:fmuzf/melt_mod.git

Major installation requirements:

  *  [Cmake](http://www.cmake.org/) 2.8 must be installed and running.

  *  A C - compiler

  * [Python](http://www.python.org/) 2, with the json and argparse modules

To build MeltMod locally (within the source tree), do

    $ cd melt_mod
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make && make test
    
####Cmake
- __OS X__ : We recommend you use a package manager such as [Homebrew](http://mxcl.github.com/homebrew/)
to obtain cmake. If you're using homebrew, do

        $ brew install cmake

- __Linux__ : Use your distribution's package manager to install cmake. E.g. apt (Ubuntu, debian, etc)

        $ apt-get install cmake

####Python
If you are running OS X or Linux, Python is most likely already installed on your system.
It remains though to check that the ``json`` and ``argparse`` modules are included with your installation.
Open a terminal window and open the python interpreter

    $ python
    
Something like the following should be displayed

    Python 2.7.1 (r271:86832, Jun 16 2011, 16:59:05) 
    [GCC 4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2335.15.00)] on darwin
    Type "help", "copyright", "credits" or "license" for more information.
    >>>

Now type:

    >>> import json, argparse

If either module is not included, python will throw an error telling you that
the module was not found.

Luckily, it's pretty easy to install python modules. We'll demonstrate with
the ``argparse`` module, since it's not included with the python 2.6 distributed
with OS X 10.6. Python has a module management tool called ``easy_install``.
At a command line, do
   
    $ sudo easy_install argparse

Some information should print to the screen, telling you that 
``argparse`` is downloading and then informing you
that ``argparse`` has been installed.

The procedure to install the ``json`` package is identical to that for
``argparse``, with all the instances of ``argparse`` replaced by ``json``

Re-open the python interpreter and appempt to import the two modules, 
if python raises an error with a module you have installed with easy_install
there may be an issue with your python environment. If python cannot find the
other module, you may need to install it as well.
