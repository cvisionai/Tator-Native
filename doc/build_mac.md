Mac build instructions
======================

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

