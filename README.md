FishAnnotator                                                  {#mainpage}
============

Introduction
============

**FishAnnotator** is an application that facilitates annotation of videos 
and images of fish.  Eventually it will be expanded to automatically create
annotations using computer vision algorithms.

Building and testing
====================

Dependencies
------------

FishAnnotator uses the following 3rd party libraries:

* [Qt5][Qt5]
* [Boost][Boost]
* [OpenCV][OpenCV]
* [FFMPEG][FFMPEG]

[CMake][CMake] is also required to build.

**Please find the appropriate install section for your OS. Static builds are only required on Windows.**

To simplify distribution, Boost, Qt and OpenCV must be built as static 
libraries.  It is recommended to download compiled Boost binaries for your
compiler if they are available, which typically include static libraries.
Most likely Qt and OpenCV will need to be built from scratch; see detailed
instructions below.

**IMPORTANT: Make sure that you build Qt and OpenCV with the same compiler you
will use for building the application (e.g. all MSVC 32 bit, or all MSVC 64 bit).**

Building the documentation requires installing [doxygen][doxysite] and
[LaTeX][latexsite].  
Building the installer requires installing [NSIS][nsissite].

Building static Qt on Windows
-----------------------------

1\. Make sure that the Windows SDK is installed along with Visual Studio.
    This project uses C++11 features which are only fully implemented on 
    Visual Studio 2015 or later.

2\. Clone the [Qt5 repository][QtRepo] from github.

3\. By default, the dev branch will be checked out.  Change the version to
    v5.8.0:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
git checkout v5.8.0
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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

7\. Type the following from the top level directory:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
configure.bat -static -release -prefix "your\working\dir\qtbase"
-platform win32-msvc2015 -qt-zlib -qt-pcre -qt-libpng -qt-libjpeg -qt-freetype
-opengl desktop -no-openssl -opensource -confirm-license -make libs -nomake
tools -nomake examples -nomake tests -sql-odbc -plugin-sql-odbc
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

8\. Type:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
nmake
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The Qt5 static libraries should now be built.

Building static OpenCV on Windows
---------------------------------

1\. Clone the [OpenCV repository][OpenCVRepo] from github.

2\. By default, the master branch will be checked out.  Change the version
    to v3.2.0:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
git checkout v3.2.0
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

3\. Navigate to the top level opencv directory and create a subdirectory
    called build.

4\. From a Visual Studio command prompt, navigate to the build directory
    and type:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
cmake -DBUILD_SHARED_LIBS=OFF ..
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

5\. Type:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
cmake --build . --target INSTALL --config Release
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The OpenCV static libraries should now be built.

Boost Binaries on Windows
------------------------------------
1\. Download the prebuilt binaries for [Boost 1.62][BoostBinaries].

2\. Run the installer.

FFMPEG Binaries on Windows
--------------------------
1\. Download the prebuilt binaries for [FFMPEG 3.3.2][FFMPEGBinaries], 
    both the developer and shared versions.

2\. Extract both.  Copy the contents of the lib directory in the shared
    version into the lib directory of the developer version.  Discard 
    the shared version and use the developer version.

Building the application on Windows
-----------------------------------

1\. It is recommended on Windows to use the CMake GUI to configure the
    build.  Open the CMake GUI, set the source directory to the top level
    FishAnnotator repository directory, and set the build directory to a
    subdirectory of this called *build*.  Something like:

    Where is the source code: C:/local/FishAnnotator
    Where to build the binaries: C:/local/FishAnnotator/build

2\. Press configure, allow CMake to create the build directory, and select
    the same generator that was used to build the dependencies.

3\. FishAnnotator uses find_package commands to find dependencies.  Sometimes
these libraries require hints to be found properly, especially on Windows.
The first time FishAnnotator is built, it will generate a file at
cmake/FishDetFindLibsHints.cmake which contains the hints for these
libraries.  The default values are set to the environment variables for
the system, however if they are not set you can modify this file manually
to point to the proper directories.  
For example,  

    set( CMAKE_PREFIX_PATH "D:/Projects/qt5/qtbase")
    set( BOOST_ROOT "D:/Projects/boost_1_62_0"  )
    set( BOOST_LIBRARYDIR "D:/Projects/boost_1_62_0/lib64-msvc-14.0" )
    set( CMAKE_CL_64 TRUE)
    set( OpenCV_DIR "D:/Projects/opencv/build/install")
    set( FFMPEG_LIBRARY_DIR "D:/Projects/ffmpeg/lib")
    set( FFMPEG_INCLUDE_DIR "D:/Projects/ffmpeg/include")

After the file is generated it will
  not be overwritten next FishAnnotator is built, so this manual modification
  is only necessary for fresh builds.  Modify this file until the libraries
  are found and the configure step completes successfully.

4\. Press generate.

5\. From a Visual Studio command prompt, navigate to the build subdirectory
    and type:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
cmake --build . --target INSTALL --config Release
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

6\. When the build completes, the applications will be located in the
    build/inst subdirectory.  If a different installation location is
    desired, cmake can be invoked with the variable CMAKE_INSTALL_PREFIX
    set to the install directory.

Building the application on Mac
-------------------------------

1\. Install required libraries:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
brew install qt5
brew install homebrew/science/opencv3 --with-ffmpeg
brew install boost
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

2\. Run cmake to generate library hints file (build will fail on a fresh build):
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
mkdir build
cd build
cmake ..
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

3\. Update the library hints file (cmake/FishDetFindLibsHints.cmake)
    to point to brew's qt5 and opencv:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cmake}
# Qt5
set( CMAKE_PREFIX_PATH "/usr/local/opt/qt5/lib/cmake" )

# Boost
set( BOOST_ROOT  )
set( BOOST_LIBRARYDIR  )

# OpenCV
set( CMAKE_CL_64  )
set( OpenCV_DIR "/usr/local/opt/opencv3/share/OpenCV" )
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

4\. Build the application:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
cmake ..
make -j4
make install
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Building the application on Ubuntu
----------------------------------

1\. Install required libraries:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
sudo apt install libopencv-dev qtbase5-dev libboost-dev
sudo add-apt-repository ppa:jonathonf/ffmpeg-3
sudo apt update && sudo apt upgrade
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

2\. Build the application.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
mkdir build
cd build
cmake ..
make -j4
make install
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Building documentation
----------------------

After CMake has been run, documentation can be built by typing:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
cmake --build . --target doc --config Release
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

or on linux/mac:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
make doc
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

from the build subdirectory.  This will generate doxygen documentation
in the doxygen subdirectory of the top level repository.  For the html
output, simply open index.html in your favorite browser.  For the pdf
output from latex, open refman.pdf.

A standalone user manual can be built by typing:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
cmake --build . --target manual --config Release
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

or on linux/mac:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
make manual
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

from the build subdirectory.  This will generate a user manual at
doxygen/manual/latex/refman.pdf.

Building installer (any platform)
---------------------------------

After FishAnnotator has been built and installed, the installer can be built
by typing the following from a command prompt:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
cpack
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The installer will be located in the top level build directory when it is
finished building.

Running unit tests
------------------

After CMake has been run the ALL_BUILD or INSTALL target has been built,
unit tests can be run by typing:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
cmake --build . --target RUN_TESTS --config Release
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

or on linux/mac:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
make test
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Developer Info
==============

This project uses the [Google C++ style guide][StyleGuide].  Refer to this for
coding practices and naming conventions.

[Qt5]: https://www.qt.io/developers/
[Boost]: http://www.boost.org
[OpenCV]: http://opencv.org/
[FFMPEG]: http://ffmpeg.org/
[CMake]: https://cmake.org/
[QtRepo]: https://github.com/qt/qt5
[Perl]: https://www.perl.org/
[Python]: https://www.python.org/
[Ruby]: http://rubyinstaller.org/
[zlib]: https://github.com/madler/zlib
[libjpeg]: https://github.com/LuaDist/libjpeg
[libpng]: https://github.com/glennrp/libpng
[OpenCVRepo]: https://github.com/opencv/opencv
[BoostBinaries]: https://sourceforge.net/projects/boost/files/boost-binaries/1.62.0/
[FFMPEGBinaries]: https://ffmpeg.zeranoe.com/builds/
[doxysite]: https://sourceforge.net/projects/doxygen/
[latexsite]: https://miktex.org/
[nsissite]: http://nsis.sourceforge.net/Main_Page
[StyleGuide]: https://google.github.io/styleguide/cppguide.html
