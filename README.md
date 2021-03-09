# KOBSWeather
Mysql driven INDI weather driver

DEPENDENCIES:

- sudo apt install build-essential devscripts debhelper fakeroot cdbs software-properties-common cmake pkg-config 
- sudo add-apt-repository ppa:mutlaqja/ppa 
- sudo apt install libindi-dev libnova-dev libz-dev libgsl-dev

CONTENTS:

- INDI driver and xml files

INSTALLING:

In a work directory of your choosing on the RPI or (linux) system

git clone https://github.com/sifank/KOBSweather.git

Binary files for the Raspberry Pi are included:

- cd [install path/]PowerStar
- sudo make install

To compile from scratch:

- cd [install_path/]KOBSweather
- mkdir build; cd build
- cmake ../
- make clean; make
- sudo make install

NOTES:

- On the INDI options page, you will need to set the mysql parameters then save
