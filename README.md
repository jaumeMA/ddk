System Requirements
===================

These instructions assume you're working on a x64 PC with Windows 10 or newer.

Installing Visual Studio
------------------------

In order to build eWAS projects, you need Visual Studio 2015 compiler. Depending on which language you want to work (english or spanish), you can install it from either of these locations:

- \\titan\ISOs\Visual Studio Enterprise 2015 English\SW_DVD9_NTRL_Visual_Studio_Ent_2015_English_FPP_VL_MLF_X20-29941.ISO
- \\titan\ISOs\Visual Studio Enterprise 2015 Spanish\SW_DVD9_NTRL_Visual_Studio_Ent_2015_Spanish_FPP_VL_MLF_X20-29949.ISO

Just mount the iso and launch the installer, then follow the instructions.

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

Have installed on your PC the following libraries:

- boost
- Qt (version 4 or 5)

All libraries shall be compiled with Visual Studio 2015 in 64 bits. You can either get the sources from the following repository:

https://bitbucket.org/ewasgtd/3rd_party.git

and compile them yourself, or get the precompiled binaries from the following locations:

- W:\Encargos en curso\Airlines\I1111-14.00 - eWAS demonstrator\Tecnica\GTD\SW\COTS\BOOST\1.71\boost-1.71.zip
- W:\Encargos en curso\Airlines\I1111-14.00 - eWAS demonstrator\Tecnica\GTD\SW\COTS\Qt\5.13\qt-5.13.1_vs2015.zip

Have the environment variables `BOOST_DIR` and `QTDIR` set respectively to your boost and Qt installation directory:
- <boost_directory>\BOOST\1.71\boost
- <qt_directory>\Qt\5.13\qt-5.13.1
If there is any error during the cmake execution try adding to PATH the Qt installation directory.
Ensure that the "\bin" directory of QT is added to the path.

Building the Project
====================

Clone the repository located at the following url:

https://stash.dev.onair.aero/scm/ewas/ewas_win.git

eWAS Pilot v3.2.0
-----------------

This is the version under development.

To run this version, checkout ***development*** branch into your local repository and run **genPrj-Pilot.pl** perl script

The **eWasPilot.sln** solution will be located in the `build` folder, while the executable **eWas.exe** will be generated in the `bin` folder.

eWAS Dispatch v1.8.0
--------------------

This is the version under development.

To run this version, checkout ***development*** branch into your local repository and run **genPrj-Dispatch.pl** perl script

The **eWasDispatch.sln** solution will be located in the `build` folder, while the executable **eWasDispatch.exe** will be generated in the `bin` folder.

eWAS Ground Services v3.40.0
----------------------------

This is the version under development.

To run this version, checkout ***development*** branch into your local repository and run **genPrj-WinServices.pl** perl script

The **eWasWinServices.sln** solution will be located in the `build` folder, while the executables **DSPPilot.exe**,
**DSPDispatch.exe**, **DSPAlert.exe**, **DDP.exe** and **DDI.exe** will be generated in the `bin` folder.
