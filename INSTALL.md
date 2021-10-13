Basic Installation
==================

  These are generic installation instructions.

  cmake attempts to guess correct values for various system-dependent
  variables used during compilation.  It uses those values to create 
  a `Makefile' in each directory of the package.  It may also create one
  or more `.h' files containing system-dependent definitions..

  The simplest way to compile this package is:

  1. `cd' to the directory containing the package's source code and type
     `cmake' to configure the package for your system.

     Running `cmake' takes awhile.  While running, it prints some
     messages telling which features it is checking for.

  2. Type `make' to compile the package.

  3. Optionally, type `make check' to run any self-tests that come with
     the package.

  4. Type `make install' to install the programs and any data files and
     documentation.

  5. You can remove the program binaries and object files from the
     source code directory by typing `make clean'.

Options
=======

- `enable-sanitize-options`: additional compiler flags (gcc specific).
- `enable-portable-build`: do not use processor specific flags. The
  generated binaries can be used on other machines as long as the
  architecture and the compiler version are the same.
- `enable-fast-math`: use advance options to optimize code further.
  Compiler can then ignore IEEE 754 compatibility.
- `enable-glibcxx-debug`: use the debug version of the STL implemented
  by the GLIB



cmake useful variables
======================

- `CMAKE_BUILD_TYPE`: two values are supported 'Release' and 'Debug'
- `CASTEM_INSTALL_PATH`: specify where the castem has been installed
- `Qt_DIR`: path to the `cmake` files associated with `Qt`
- `TFEL_INSTALL_PATH`: path to where `TFEL` is installed. No required if
  `tfel-config` is available.

cmake typical usage
===================

cmake ../tfel-plot -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/home/th202608/codes/tfel-plot/master/install

main targets:
=============

The main generated targets are:

- `all` : build the libraries
- `doc-html`: generate the `doxygen` documentation
- `doc-pdf`: generate all the documentation in pdf
- `website`: generate the `TFEL` website
- `doc`: generate all the documentation
- `check`: build tests and execute them
- `install`: install the libraries
