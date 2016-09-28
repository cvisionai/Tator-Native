FishDetector                                                   {#mainpage}
============

Introduction
============

**FishDetector** is an application that aids and performs video analysis
on videos of fish.

Building and testing
====================

Dependencies
------------

FishDetector uses the following 3rd party libraries:

* [Qt][Qt]
* [Boost.PropertyTree][BoostPropertyTree]
* [OpenCV][OpenCV]

To simplify distribution, Qt and OpenCV must be built as static libraries.
Boost.PropertyTree is include only, so the Boost libraries do not need to 
be built.

Building static Qt on Windows
-----------------------------

1\. Make sure that the Windows SDK is installed along with Visual Studio.

2\. Clone the [Qt5 repository][QtRepo] from github.

3\. By default, the dev branch will be checked out.  If desired, change
    your branch to a tagged release version.

4\. Navigate to the top level repository directory and type:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
git submodule update --init
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This will clone all the submodules of Qt and check out the branch 
corresponding to your current branch.

5\. Install the following script languages:

* [Perl][Perl]
* [Python][Python]
* [Ruby][Ruby]

6\. In the subdirectory qtbase\mkspecs\common, change msvc-desktop.conf
    such that all occurrences of MD are replaced with MT.

7\. Type the following:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
configure.bat -static -debug-and-release -prefix "your\working\dir\qtbase"
-platform win32-msvc2015 -qt-zlib -qt-pcre -qt-libpng -qt-libjpeg -qt-freetype
-opengl desktop -no-openssl -opensource -confirm-license -make libs -nomake
tools -nomake examples -nomake tests
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

8\. Type:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
nmake
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The Qt5 static libraries should now be built.


