# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_SOURCE_DIR = /home/wddxrw/workspace/codebase/C++

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/wddxrw/workspace/codebase/build

# Include any dependencies generated for this target.
include CMakeFiles/test_random_number.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test_random_number.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_random_number.dir/flags.make

CMakeFiles/test_random_number.dir/tests/test_random_number.cc.o: CMakeFiles/test_random_number.dir/flags.make
CMakeFiles/test_random_number.dir/tests/test_random_number.cc.o: /home/wddxrw/workspace/codebase/C++/tests/test_random_number.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wddxrw/workspace/codebase/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_random_number.dir/tests/test_random_number.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) -D__FILE__=\"tests/test_random_number.cc\" $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_random_number.dir/tests/test_random_number.cc.o -c /home/wddxrw/workspace/codebase/C++/tests/test_random_number.cc

CMakeFiles/test_random_number.dir/tests/test_random_number.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_random_number.dir/tests/test_random_number.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) -D__FILE__=\"tests/test_random_number.cc\" $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wddxrw/workspace/codebase/C++/tests/test_random_number.cc > CMakeFiles/test_random_number.dir/tests/test_random_number.cc.i

CMakeFiles/test_random_number.dir/tests/test_random_number.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_random_number.dir/tests/test_random_number.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) -D__FILE__=\"tests/test_random_number.cc\" $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wddxrw/workspace/codebase/C++/tests/test_random_number.cc -o CMakeFiles/test_random_number.dir/tests/test_random_number.cc.s

# Object files for target test_random_number
test_random_number_OBJECTS = \
"CMakeFiles/test_random_number.dir/tests/test_random_number.cc.o"

# External object files for target test_random_number
test_random_number_EXTERNAL_OBJECTS =

/home/wddxrw/workspace/codebase/C++/bin/test_random_number: CMakeFiles/test_random_number.dir/tests/test_random_number.cc.o
/home/wddxrw/workspace/codebase/C++/bin/test_random_number: CMakeFiles/test_random_number.dir/build.make
/home/wddxrw/workspace/codebase/C++/bin/test_random_number: /home/wddxrw/workspace/codebase/C++/lib/libcodebase.so
/home/wddxrw/workspace/codebase/C++/bin/test_random_number: CMakeFiles/test_random_number.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wddxrw/workspace/codebase/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable /home/wddxrw/workspace/codebase/C++/bin/test_random_number"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_random_number.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_random_number.dir/build: /home/wddxrw/workspace/codebase/C++/bin/test_random_number

.PHONY : CMakeFiles/test_random_number.dir/build

CMakeFiles/test_random_number.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_random_number.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_random_number.dir/clean

CMakeFiles/test_random_number.dir/depend:
	cd /home/wddxrw/workspace/codebase/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wddxrw/workspace/codebase/C++ /home/wddxrw/workspace/codebase/C++ /home/wddxrw/workspace/codebase/build /home/wddxrw/workspace/codebase/build /home/wddxrw/workspace/codebase/build/CMakeFiles/test_random_number.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_random_number.dir/depend

