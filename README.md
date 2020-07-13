DDK Library
===================

DDK library is just another set of functionalities broading from signal/slot pattern to unformatted arenas.

Its main highlights are:

1.- Smart Pointers such as: shared, unique and lent pointers (borrrow checker like). It comes as well with lendable structure types (wrapping or inheriting).
2.- Variants and optionals: In case of variants it comes with static visitor pattern (no heterogeneous variants allowed by now)
3.- Flagsets
4.- Intrusive lists and stacks
5.- Scoped enums (superseded by class enums)
6.- Tagged pointers (already used by smart pointers)
7.- Executors pattern. It does not contain process executors by now
8.- Custom thread with thread pools as well
9.- Critical section concept. It comes with multiple reader single writer pattern embedded (exclusion area class)
10.- Transformed iterators (supereseded by views)
11.- Dynamic visitors
12.- std like async funcionatlity. In this case we allow to specify where to execute the callable object. It comes with futures and promises
13.- Fibers and coroutines for windows and linux (iOS pending to be tested). It comes with co_iteration as well

Installing CMake
----------------

In order to generate eWAS projects, you need CMake 3.13 or newer. You can download the latest version at this link:

https://cmake.org/download/

Just launch the installer and follow the instructions.

Installing Perl
---------------

In order to generate CMake scripts, you need Perl. You can download the latest version at ActiveState website:

https://www.activestate.com/products/perl/downloads/

Just launch the installer and follow the instructions.

Preconditions
=============

None

Building the Project
====================

- Clone the repository
- execute perl script genPrj-XXX.pl (where XXX stands for platform)

The **ddkXXX.sln** solution will be located in the `build` folder, while generated binaries will be generated in the `bin` folder.