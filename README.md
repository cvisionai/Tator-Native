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

[CMake][CMake] is also required to build.

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

7\. Type the following from the top level directory:

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

Building static OpenCV on Windows
---------------------------------

1\. Clone the [OpenCV repository][OpenCVRepo] from github.

2\. By default, the master branch will be checked out.  If desired, change
    your branch to a tagged release version.

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

Building static Qt on Mac
-------------------------

1\. Make sure that XCode is installed.

2\. Clone the [Qt5 repository][QtRepo] from github.

3\. By default, the dev branch will be checked out.  If desired, change
    your branch to a tagged release version.

4\. Navigate to the top level repository directory and type:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
git submodule update --init
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This will clone all the submodules of Qt and check out the branch
corresponding to your current branch.

5\. Install the following script languages with brew:

* [Perl][Perl]
* [Python][Python]
* [Ruby][Ruby]

6\. Type the following from the top level directory:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
./configure -static -debug-and-release -prefix "your/working/dir/qtbase"
-platform macx-clang -qt-zlib -qt-pcre -qt-libpng -qt-libjpeg -qt-freetype
-opengl desktop -no-openssl -opensource -confirm-license -make libs -nomake
tools -nomake examples -nomake tests
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

7\. Type:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
make -j4 && make install
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The Qt5 static libraries should now be built.

8\. It is possible that some 3rd party libraries will NOT be built, such
    as [zlib][zlib], [libjpeg][libjpeg], and [libpng][libpng].  You will
    need to clone and build static versions of these.  Each of them can
    be built by typing:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
mkdir build
cd build
cmake -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=./inst ..
make -j4
make install
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

from the top level cloned directory.

Building static OpenCV on Mac
-----------------------------

1\. Install ffmpeg with the following options enabled:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
brew install ffmpeg --with-fdk-aac --with-ffplay --with-freetype
--with-libass --with-libquvi --with-libvorbis --with-libvpx --with-opus
--with-x265
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1\. Clone the [OpenCV repository][OpenCVRepo] from github.

2\. By default, the master branch will be checked out.  If desired, change
    your branch to a tagged release version.

3\. Navigate to the top level opencv directory and create a subdirectory
    called build.

4\. From a Visual Studio command prompt, navigate to the build directory
    and type:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
cmake -DBUILD_SHARED_LIBS=OFF -DWITH_FFMPEG=ON
-DCMAKE_INSTALL_PREFIX=./inst ..
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

5\. Type:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
make -j4 && make install
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The OpenCV static libraries should now be built.

Building static Qt on Ubuntu
----------------------------

1\. Clone the [Qt5 repository][QtRepo] from github.

2\. By default, the dev branch will be checked out.  If desired, change
    your branch to a tagged release version.

3\. Navigate to the top level repository directory and type:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
git submodule update --init
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This will clone all the submodules of Qt and check out the branch
corresponding to your current branch.

4\. Install necessary packages with apt:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
sudo apt install ruby perl python freeglut3 freeglut3-dev libglew1.5 
libglew1.5-dev libglu1-mesa libglu1-mesa-dev libgl1-mesa-glx 
libgl1-mesa-dev
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

5\. Type the following from the top level directory:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
./configure -static -release -prefix "your/working/dir/qtbase"
-qt-zlib -qt-pcre -qt-libpng -qt-libjpeg -qt-freetype -qt-harfbuzz
-opengl desktop -no-openssl -opensource -confirm-license -make libs -nomake
tools -nomake examples -nomake tests
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

6\. Type:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
make -j4 && make install
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The Qt5 static libraries should now be built.

7\. Important: Do not link to the 3rd party image libraries provided
    in the Qt build.  To ensure compatibility across that application,
    use the static libraries from the OpenCV build in the directory
    build/3rdparty/lib.

Building static OpenCV on Ubuntu 
--------------------------------

1\. Install required video packages:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
sudo apt install ffmpeg libavcodec-dev libavfilter-dev libavformat-dev
libavutil-dev libpostproc-dev libswscale-dev
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

2\. Clone the [OpenCV repository][OpenCVRepo] from github.

3\. By default, the master branch will be checked out.  If desired, change
    your branch to a tagged release version.

4\. Navigate to the top level opencv directory and create a subdirectory
    called build.

5\. From a Visual Studio command prompt, navigate to the build directory
    and type:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
cmake -DBUILD_SHARED_LIBS=OFF -DWITH_FFMPEG=ON
-DCMAKE_INSTALL_PREFIX=./inst ..
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

5\. Type:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
make -j4 && make install
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The OpenCV static libraries should now be built.

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

Building the application on Mac/Ubuntu
--------------------------------------

1\. From top level FishAnnotator directory:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
mkdir build
cd build
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

2\. FishAnnotator uses find_package commands to find dependencies.  Sometimes
these libraries require hints to be found properly, especially on Windows.
The first time FishAnnotator is built, it will generate a file at
cmake/FishDetFindLibsHints.cmake which contains the hints for these
libraries.  The default values are set to the environment variables for
the system, however if they are not set you can modify this file manually
to point to the proper directories.  
For example,  

    set( CMAKE_PREFIX_PATH "/Users/jtakahashi/dev/qt5/qtbase" )
    set( BOOST_ROOT  )
    set( BOOST_LIBRARYDIR  )
    set( CMAKE_CL_64  )
    set( OpenCV_DIR "/Users/jtakahashi/dev/opencv/build/inst" )
    set( ZLIB_ROOT "/Users/jtakahashi/dev/zlib/build/inst" )
    set( JPEG_LIBRARIES "/Users/jtakahashi/dev/libjpeg/build/inst/lib/libjpeg.a" )
    set( PNG_LIBRARIES "/Users/jtakahashi/dev/libpng/build/inst/lib/libpng.a" )

After the file is generated it will
not be overwritten next time FishAnnotator is built, so this manual 
modification is only necessary for fresh builds.  Modify this file until 
the libraries are found and the configure step completes successfully.

3\. Type:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
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

Building installer
------------------

After FishAnnotator has been built and installed, the installer can be built
by typing the following from a Visual Studio command prompt:

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
[CMake]: https://cmake.org/
[QtRepo]: https://github.com/qt/qt5
[Perl]: https://www.perl.org/
[Python]: https://www.python.org/
[Ruby]: http://rubyinstaller.org/
[zlib]: https://github.com/madler/zlib
[libjpeg]: https://github.com/LuaDist/libjpeg
[libpng]: https://github.com/glennrp/libpng
[OpenCVRepo]: https://github.com/opencv/opencv
[doxysite]: https://sourceforge.net/projects/doxygen/
[latexsite]: https://miktex.org/
[nsissite]: http://nsis.sourceforge.net/Main_Page
[StyleGuide]: https://google.github.io/styleguide/cppguide.html
