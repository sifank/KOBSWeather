# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/sifan/INDI/KOBSWeather

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sifan/INDI/KOBSWeather/build

# Include any dependencies generated for this target.
include CMakeFiles/indi_KOBSweather.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/indi_KOBSweather.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/indi_KOBSweather.dir/flags.make

CMakeFiles/indi_KOBSweather.dir/KOBSweather.cpp.o: CMakeFiles/indi_KOBSweather.dir/flags.make
CMakeFiles/indi_KOBSweather.dir/KOBSweather.cpp.o: ../KOBSweather.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sifan/INDI/KOBSWeather/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/indi_KOBSweather.dir/KOBSweather.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/indi_KOBSweather.dir/KOBSweather.cpp.o -c /home/sifan/INDI/KOBSWeather/KOBSweather.cpp

CMakeFiles/indi_KOBSweather.dir/KOBSweather.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/indi_KOBSweather.dir/KOBSweather.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sifan/INDI/KOBSWeather/KOBSweather.cpp > CMakeFiles/indi_KOBSweather.dir/KOBSweather.cpp.i

CMakeFiles/indi_KOBSweather.dir/KOBSweather.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/indi_KOBSweather.dir/KOBSweather.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sifan/INDI/KOBSWeather/KOBSweather.cpp -o CMakeFiles/indi_KOBSweather.dir/KOBSweather.cpp.s

CMakeFiles/indi_KOBSweather.dir/KOBSweather.cpp.o.requires:

.PHONY : CMakeFiles/indi_KOBSweather.dir/KOBSweather.cpp.o.requires

CMakeFiles/indi_KOBSweather.dir/KOBSweather.cpp.o.provides: CMakeFiles/indi_KOBSweather.dir/KOBSweather.cpp.o.requires
	$(MAKE) -f CMakeFiles/indi_KOBSweather.dir/build.make CMakeFiles/indi_KOBSweather.dir/KOBSweather.cpp.o.provides.build
.PHONY : CMakeFiles/indi_KOBSweather.dir/KOBSweather.cpp.o.provides

CMakeFiles/indi_KOBSweather.dir/KOBSweather.cpp.o.provides.build: CMakeFiles/indi_KOBSweather.dir/KOBSweather.cpp.o


# Object files for target indi_KOBSweather
indi_KOBSweather_OBJECTS = \
"CMakeFiles/indi_KOBSweather.dir/KOBSweather.cpp.o"

# External object files for target indi_KOBSweather
indi_KOBSweather_EXTERNAL_OBJECTS =

indi_KOBSweather: CMakeFiles/indi_KOBSweather.dir/KOBSweather.cpp.o
indi_KOBSweather: CMakeFiles/indi_KOBSweather.dir/build.make
indi_KOBSweather: /usr/lib/libindidriver.so
indi_KOBSweather: /usr/lib/aarch64-linux-gnu/libindiAlignmentDriver.so
indi_KOBSweather: /usr/lib/aarch64-linux-gnu/libnova.so
indi_KOBSweather: /usr/lib/aarch64-linux-gnu/libgsl.so
indi_KOBSweather: /usr/lib/aarch64-linux-gnu/libgslcblas.so
indi_KOBSweather: CMakeFiles/indi_KOBSweather.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sifan/INDI/KOBSWeather/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable indi_KOBSweather"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/indi_KOBSweather.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/indi_KOBSweather.dir/build: indi_KOBSweather

.PHONY : CMakeFiles/indi_KOBSweather.dir/build

CMakeFiles/indi_KOBSweather.dir/requires: CMakeFiles/indi_KOBSweather.dir/KOBSweather.cpp.o.requires

.PHONY : CMakeFiles/indi_KOBSweather.dir/requires

CMakeFiles/indi_KOBSweather.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/indi_KOBSweather.dir/cmake_clean.cmake
.PHONY : CMakeFiles/indi_KOBSweather.dir/clean

CMakeFiles/indi_KOBSweather.dir/depend:
	cd /home/sifan/INDI/KOBSWeather/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sifan/INDI/KOBSWeather /home/sifan/INDI/KOBSWeather /home/sifan/INDI/KOBSWeather/build /home/sifan/INDI/KOBSWeather/build /home/sifan/INDI/KOBSWeather/build/CMakeFiles/indi_KOBSweather.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/indi_KOBSweather.dir/depend

