# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/macaira1466/Documents/IN4200

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/macaira1466/Documents/IN4200/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/IN4200.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/IN4200.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/IN4200.dir/flags.make

CMakeFiles/IN4200.dir/main.c.o: CMakeFiles/IN4200.dir/flags.make
CMakeFiles/IN4200.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/macaira1466/Documents/IN4200/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/IN4200.dir/main.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/IN4200.dir/main.c.o   -c /Users/macaira1466/Documents/IN4200/main.c

CMakeFiles/IN4200.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/IN4200.dir/main.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/macaira1466/Documents/IN4200/main.c > CMakeFiles/IN4200.dir/main.c.i

CMakeFiles/IN4200.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/IN4200.dir/main.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/macaira1466/Documents/IN4200/main.c -o CMakeFiles/IN4200.dir/main.c.s

# Object files for target IN4200
IN4200_OBJECTS = \
"CMakeFiles/IN4200.dir/main.c.o"

# External object files for target IN4200
IN4200_EXTERNAL_OBJECTS =

IN4200: CMakeFiles/IN4200.dir/main.c.o
IN4200: CMakeFiles/IN4200.dir/build.make
IN4200: CMakeFiles/IN4200.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/macaira1466/Documents/IN4200/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable IN4200"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/IN4200.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/IN4200.dir/build: IN4200

.PHONY : CMakeFiles/IN4200.dir/build

CMakeFiles/IN4200.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/IN4200.dir/cmake_clean.cmake
.PHONY : CMakeFiles/IN4200.dir/clean

CMakeFiles/IN4200.dir/depend:
	cd /Users/macaira1466/Documents/IN4200/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/macaira1466/Documents/IN4200 /Users/macaira1466/Documents/IN4200 /Users/macaira1466/Documents/IN4200/cmake-build-debug /Users/macaira1466/Documents/IN4200/cmake-build-debug /Users/macaira1466/Documents/IN4200/cmake-build-debug/CMakeFiles/IN4200.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/IN4200.dir/depend

