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
include CMakeFiles/test_server_Thread.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test_server_Thread.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_server_Thread.dir/flags.make

CMakeFiles/test_server_Thread.dir/tests/test_server_Thread.cc.o: CMakeFiles/test_server_Thread.dir/flags.make
CMakeFiles/test_server_Thread.dir/tests/test_server_Thread.cc.o: ../tests/test_server_Thread.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wddxrw/workspace/codebase/C++/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_server_Thread.dir/tests/test_server_Thread.cc.o"
	/bin/g++-9  $(CXX_DEFINES) -D__FILE__=\"tests/test_server_Thread.cc\" $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_server_Thread.dir/tests/test_server_Thread.cc.o -c /home/wddxrw/workspace/codebase/C++/tests/test_server_Thread.cc

CMakeFiles/test_server_Thread.dir/tests/test_server_Thread.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_server_Thread.dir/tests/test_server_Thread.cc.i"
	/bin/g++-9 $(CXX_DEFINES) -D__FILE__=\"tests/test_server_Thread.cc\" $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wddxrw/workspace/codebase/C++/tests/test_server_Thread.cc > CMakeFiles/test_server_Thread.dir/tests/test_server_Thread.cc.i

CMakeFiles/test_server_Thread.dir/tests/test_server_Thread.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_server_Thread.dir/tests/test_server_Thread.cc.s"
	/bin/g++-9 $(CXX_DEFINES) -D__FILE__=\"tests/test_server_Thread.cc\" $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wddxrw/workspace/codebase/C++/tests/test_server_Thread.cc -o CMakeFiles/test_server_Thread.dir/tests/test_server_Thread.cc.s

# Object files for target test_server_Thread
test_server_Thread_OBJECTS = \
"CMakeFiles/test_server_Thread.dir/tests/test_server_Thread.cc.o"

# External object files for target test_server_Thread
test_server_Thread_EXTERNAL_OBJECTS =

../bin/test_server_Thread: CMakeFiles/test_server_Thread.dir/tests/test_server_Thread.cc.o
../bin/test_server_Thread: CMakeFiles/test_server_Thread.dir/build.make
../bin/test_server_Thread: ../lib/libcodebase.so
../bin/test_server_Thread: CMakeFiles/test_server_Thread.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wddxrw/workspace/codebase/C++/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/test_server_Thread"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_server_Thread.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_server_Thread.dir/build: ../bin/test_server_Thread

.PHONY : CMakeFiles/test_server_Thread.dir/build

CMakeFiles/test_server_Thread.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_server_Thread.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_server_Thread.dir/clean

CMakeFiles/test_server_Thread.dir/depend:
	cd /home/wddxrw/workspace/codebase/C++/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wddxrw/workspace/codebase/C++ /home/wddxrw/workspace/codebase/C++ /home/wddxrw/workspace/codebase/C++/build /home/wddxrw/workspace/codebase/C++/build /home/wddxrw/workspace/codebase/C++/build/CMakeFiles/test_server_Thread.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_server_Thread.dir/depend

