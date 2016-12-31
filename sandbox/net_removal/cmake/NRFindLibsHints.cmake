# This file is used to generate a library hints file in the
# source tree without requiring these hints to be part of 
# version control
# 
# The hints file is generated only if it does not already
# exist, allowing for one time configuration on a given
# machine.

# Qt5
set( CMAKE_PREFIX_PATH "C:/local/qt5/qtbase" )

# Boost
set( BOOST_ROOT "C:/local/boost_1_62_0" )
set( BOOST_LIBRARYDIR "C:/local/boost_1_62_0/lib64-msvc-14.0" )

# OpenCV
set( CMAKE_CL_64 TRUE )
set( OpenCV_DIR "C:/local/opencv/build/install" )

