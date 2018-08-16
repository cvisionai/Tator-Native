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
1\. Download the prebuilt binaries for [Boost 1.62][BoostBinaries].

2\. Run the installer.

FFMPEG Binaries on Windows
--------------------------
1\. Download the prebuilt binaries for [FFMPEG 3.3.2][FFMPEGBinaries], 
    both the developer and shared versions.

2\. Extract both.  Copy the contents of the bin directory in the shared
    version into the lib directory of the developer version.  Discard 
    the shared version and use the developer version.

Building the application on Windows
-----------------------------------

1\. It is recommended on Windows to use the CMake GUI to configure the
    build.  Open the CMake GUI, set the source directory to the top level
    Tator repository directory, and set the build directory to a
    subdirectory of this called *build*.  Something like:

    Where is the source code: C:/local/Tator
    Where to build the binaries: C:/local/Tator/build

2\. Press configure, allow CMake to create the build directory, and select
    the same generator that was used to build the dependencies.

3\. Tator uses find_package commands to find dependencies.  Sometimes
these libraries require hints to be found properly, especially on Windows.
The first time Tator is built, it will generate a file at
cmake/TatorFindLibsHints.cmake which contains the hints for these
libraries.  The default values are set to the environment variables for
the system, however if they are not set you can modify this file manually
to point to the proper directories.  
For example,  

    set( CMAKE_PREFIX_PATH "D:/Projects/qt5/qtbase")
    set( BOOST_ROOT "D:/Projects/boost_1_62_0"  )
    set( BOOST_LIBRARYDIR "D:/Projects/boost_1_62_0/lib64-msvc-14.0" )
    set( FFMPEG_LIBRARY_DIR "D:/Projects/ffmpeg/lib")
    set( FFMPEG_INCLUDE_DIR "D:/Projects/ffmpeg/include")

After the file is generated it will not be overwritten next time Tator is 
built, so this manual modification is only necessary for fresh builds.  
Modify this file until the libraries are found and the configure step 
completes successfully.

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

[QtRepo]: https://github.com/qt/qt5
[Perl]: https://www.perl.org/
[Python]: https://www.python.org/
[Ruby]: http://rubyinstaller.org/
[zlib]: https://github.com/madler/zlib
[libjpeg]: https://github.com/LuaDist/libjpeg
[libpng]: https://github.com/glennrp/libpng
[BoostBinaries]: https://sourceforge.net/projects/boost/files/boost-binaries/1.62.0/
[FFMPEGBinaries]: https://ffmpeg.zeranoe.com/builds/

