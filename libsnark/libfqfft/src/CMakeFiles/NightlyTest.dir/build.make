# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

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
CMAKE_SOURCE_DIR = /home/yuncong/Projects/libfqfft

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yuncong/Projects/libfqfft

# Utility rule file for NightlyTest.

# Include the progress variables for this target.
include src/CMakeFiles/NightlyTest.dir/progress.make

src/CMakeFiles/NightlyTest:
	cd /home/yuncong/Projects/libfqfft/src && /usr/bin/ctest -D NightlyTest

NightlyTest: src/CMakeFiles/NightlyTest
NightlyTest: src/CMakeFiles/NightlyTest.dir/build.make

.PHONY : NightlyTest

# Rule to build all files generated by this target.
src/CMakeFiles/NightlyTest.dir/build: NightlyTest

.PHONY : src/CMakeFiles/NightlyTest.dir/build

src/CMakeFiles/NightlyTest.dir/clean:
	cd /home/yuncong/Projects/libfqfft/src && $(CMAKE_COMMAND) -P CMakeFiles/NightlyTest.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/NightlyTest.dir/clean

src/CMakeFiles/NightlyTest.dir/depend:
	cd /home/yuncong/Projects/libfqfft && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yuncong/Projects/libfqfft /home/yuncong/Projects/libfqfft/src /home/yuncong/Projects/libfqfft /home/yuncong/Projects/libfqfft/src /home/yuncong/Projects/libfqfft/src/CMakeFiles/NightlyTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/NightlyTest.dir/depend
