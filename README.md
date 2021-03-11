# KOBSWeather
INDI weather driver - uses a python script to gather parameters

DEPENDENCIES:

- sudo apt install build-essential devscripts debhelper fakeroot cdbs software-properties-common cmake pkg-config
- sudo add-apt-repository ppa:mutlaqja/ppa 
- sudo apt install libindi-dev libnova-dev libz-dev libgsl-dev

ISSUE:
- Latest INDI release has an orphaned link:  /usr/lib/libindidriver.so points to nowhere!
  - To fis: remove it, use 'locate libindidriver.so' to find the most recent version and create a link to it.
  - Example: 
    - sudo rm /usr/lib/libindidriver.so
    - sudo ln -s /usr/lib/x86_64-linux-gnu/libindidriver.so.1.8.9   /usr/lib/libindidriver.so

CONTENTS:

- INDI driver and xml files

INSTALLING:

In a work directory of your choosing on the RPI or (linux) system

- git clone https://github.com/sifank/KOBSWeather.git
- cd [install_path/]KOBSweather
- mkdir build; cd build
- cmake ../
- make clean; make
- sudo make install

NOTES:
- The included python script (KOBSweather.py) gathers parameters from a mysql database and outputs a csv list of values
- You can modify this to gather the data from your own weather devices
- If you have different, more or less parameters, you need to modify both the python script and the INDI driver to match
