Building installer (any platform)
=================================

Building the installer on Windows requires installing [NSIS][nsissite].

When running cmake, the option BUILD_INSTALLER must be enabled.

After Tator has been built and installed, the installer can be built
by typing the following from a command prompt:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
cpack
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The installer will be located in the top level build directory when it is
finished building.  CPack is included with CMake.

[nsissite]: http://nsis.sourceforge.net/Main_Page

