Ubuntu build instructions
=========================

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

