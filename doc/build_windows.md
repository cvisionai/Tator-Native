Windows build instructions
==========================

Static libraries
----------------

To simplify distribution, Boost and Qt must be built as static 
libraries on Windows.  It is recommended to download compiled Boost 
binaries for your compiler if they are available, which typically include 
static libraries.  Most likely Qt will need to be built from 
scratch; see detailed instructions below.

**IMPORTANT: Make sure that you build Qt with the same architecture you
will use for building the application (e.g. all MSVC 32 bit, or all MSVC 
64 bit).**

Building static Qt on Windows
-----------------------------

1\. Make sure that the Windows SDK is installed along with Visual Studio.
    This project uses C++11 features which are only fully implemented on 
    Visual Studio 2015 or later.

2\. Clone the [Qt5 repository][QtRepo] from github.

3\. By default, the dev branch will be checked out.  Change the version to
    v5.9.4:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
git checkout v5.9.4
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

6\. In the subdirectory qtbase\\mkspecs\\common, change msvc-desktop.conf
    such that all occurrences of MD are replaced with MT.

7\. Type the following from the top level directory:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
configure.bat -static -release -prefix "your\working\dir\qtbase"
-platform win32-msvc2015 -qt-zlib -qt-pcre -qt-libpng -qt-libjpeg -qt-freetype
-opengl desktop -no-openssl -opensource -confirm-license -make libs -nomake
tools -nomake examples -nomake tests -sql-odbc -plugin-sql-odbc -skip qtquick1
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

8\. Type:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
nmake
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The Qt5 static libraries should now be built.

Boost Binaries on Windows
------------------------------------
1\. Download the prebuilt binaries for [Boost 1.65.1][BoostBinaries].

2\. Run the installer.

FFMPEG Binaries on Windows
--------------------------
1\. Download the prebuilt binaries for [FFMPEG 3.3.4][FFMPEGBinaries], 
    both the developer and shared versions.

2\. Extract both.  Copy the contents of the bin directory in the shared
    version into the lib directory of the developer version.  Discard 
    the shared version and use the developer version.

Building the application on Windows
-----------------------------------

1\. Create a build directory and run cmake.  For example:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
mkdir build
cd build
cmake .. -G "Visual Studio 15 2017 Win64" ^
-DBOOST_ROOT=C:/local/boost_1_65_1 ^
-DBOOST_LIBRARYDIR=C:/local/boost_1_65_1/lib64-msvc-14.1 ^
-DQt5_DIR=C:/local/qt5/qtbase/lib/cmake/Qt5 ^
-DFFMPEG_LIBRARY_DIR=C:/local/ffmpeg_3_4_2/lib ^
-DFFMPEG_INCLUDE_DIR=C:/local/ffmpeg_3_4_2/include
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Your library paths may differ depending on where you installed them.

3\. From a Visual Studio command prompt, navigate to the build subdirectory
    and type:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
cmake --build . --target INSTALL --config Release
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

4\. When the build completes, the applications will be located in the
    build/inst subdirectory.  If a different installation location is
    desired, cmake can be invoked with the variable CMAKE_INSTALL_PREFIX
    set to the install directory.

[QtRepo]: https://github.com/qt/qt5
[Perl]: https://www.perl.org/
[Python]: https://www.python.org/
[Ruby]: http://rubyinstaller.org/
[zlib]: https://github.com/madler/zlib
[libjpeg]: https://github.com/LuaDist/libjpeg
[libpng]: https://github.com/glennrp/libpng
[BoostBinaries]: https://sourceforge.net/projects/boost/files/boost-binaries/1.65.1/
[FFMPEGBinaries]: https://ffmpeg.zeranoe.com/builds/

