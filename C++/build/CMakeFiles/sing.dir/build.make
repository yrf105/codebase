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
CMAKE_BINARY_DIR = /home/wddxrw/workspace/codebase/C++/build

# Include any dependencies generated for this target.
include CMakeFiles/sing.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/sing.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sing.dir/flags.make

CMakeFiles/sing.dir/tests/test_tuple.cc.o: CMakeFiles/sing.dir/flags.make
CMakeFiles/sing.dir/tests/test_tuple.cc.o: ../tests/test_tuple.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wddxrw/workspace/codebase/C++/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/sing.dir/tests/test_tuple.cc.o"
	/bin/g++-9  $(CXX_DEFINES) -D__FILE__=\"tests/test_tuple.cc\" $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sing.dir/tests/test_tuple.cc.o -c /home/wddxrw/workspace/codebase/C++/tests/test_tuple.cc

CMakeFiles/sing.dir/tests/test_tuple.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sing.dir/tests/test_tuple.cc.i"
	/bin/g++-9 $(CXX_DEFINES) -D__FILE__=\"tests/test_tuple.cc\" $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wddxrw/workspace/codebase/C++/tests/test_tuple.cc > CMakeFiles/sing.dir/tests/test_tuple.cc.i

CMakeFiles/sing.dir/tests/test_tuple.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sing.dir/tests/test_tuple.cc.s"
	/bin/g++-9 $(CXX_DEFINES) -D__FILE__=\"tests/test_tuple.cc\" $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wddxrw/workspace/codebase/C++/tests/test_tuple.cc -o CMakeFiles/sing.dir/tests/test_tuple.cc.s

# Object files for target sing
sing_OBJECTS = \
"CMakeFiles/sing.dir/tests/test_tuple.cc.o"

# External object files for target sing
sing_EXTERNAL_OBJECTS =

../bin/sing: CMakeFiles/sing.dir/tests/test_tuple.cc.o
../bin/sing: CMakeFiles/sing.dir/build.make
../bin/sing: ../lib/libcodebase.so
../bin/sing: CMakeFiles/sing.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wddxrw/workspace/codebase/C++/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/sing"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sing.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sing.dir/build: ../bin/sing

.PHONY : CMakeFiles/sing.dir/build

CMakeFiles/sing.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sing.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sing.dir/clean

CMakeFiles/sing.dir/depend:
	cd /home/wddxrw/workspace/codebase/C++/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wddxrw/workspace/codebase/C++ /home/wddxrw/workspace/codebase/C++ /home/wddxrw/workspace/codebase/C++/build /home/wddxrw/workspace/codebase/C++/build /home/wddxrw/workspace/codebase/C++/build/CMakeFiles/sing.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sing.dir/depend

