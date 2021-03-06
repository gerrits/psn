# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/gerrits/Documents/Code/im_proj/psn/mosquitto

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/gerrits/Documents/Code/im_proj/psn/mosquitto/src

# Include any dependencies generated for this target.
include client/CMakeFiles/mosquitto_sub.dir/depend.make

# Include the progress variables for this target.
include client/CMakeFiles/mosquitto_sub.dir/progress.make

# Include the compile flags for this target's objects.
include client/CMakeFiles/mosquitto_sub.dir/flags.make

client/CMakeFiles/mosquitto_sub.dir/sub_client.c.o: client/CMakeFiles/mosquitto_sub.dir/flags.make
client/CMakeFiles/mosquitto_sub.dir/sub_client.c.o: ../client/sub_client.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/gerrits/Documents/Code/im_proj/psn/mosquitto/src/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object client/CMakeFiles/mosquitto_sub.dir/sub_client.c.o"
	cd /home/gerrits/Documents/Code/im_proj/psn/mosquitto/src/client && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/mosquitto_sub.dir/sub_client.c.o   -c /home/gerrits/Documents/Code/im_proj/psn/mosquitto/client/sub_client.c

client/CMakeFiles/mosquitto_sub.dir/sub_client.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mosquitto_sub.dir/sub_client.c.i"
	cd /home/gerrits/Documents/Code/im_proj/psn/mosquitto/src/client && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/gerrits/Documents/Code/im_proj/psn/mosquitto/client/sub_client.c > CMakeFiles/mosquitto_sub.dir/sub_client.c.i

client/CMakeFiles/mosquitto_sub.dir/sub_client.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mosquitto_sub.dir/sub_client.c.s"
	cd /home/gerrits/Documents/Code/im_proj/psn/mosquitto/src/client && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/gerrits/Documents/Code/im_proj/psn/mosquitto/client/sub_client.c -o CMakeFiles/mosquitto_sub.dir/sub_client.c.s

client/CMakeFiles/mosquitto_sub.dir/sub_client.c.o.requires:
.PHONY : client/CMakeFiles/mosquitto_sub.dir/sub_client.c.o.requires

client/CMakeFiles/mosquitto_sub.dir/sub_client.c.o.provides: client/CMakeFiles/mosquitto_sub.dir/sub_client.c.o.requires
	$(MAKE) -f client/CMakeFiles/mosquitto_sub.dir/build.make client/CMakeFiles/mosquitto_sub.dir/sub_client.c.o.provides.build
.PHONY : client/CMakeFiles/mosquitto_sub.dir/sub_client.c.o.provides

client/CMakeFiles/mosquitto_sub.dir/sub_client.c.o.provides.build: client/CMakeFiles/mosquitto_sub.dir/sub_client.c.o

# Object files for target mosquitto_sub
mosquitto_sub_OBJECTS = \
"CMakeFiles/mosquitto_sub.dir/sub_client.c.o"

# External object files for target mosquitto_sub
mosquitto_sub_EXTERNAL_OBJECTS =

client/mosquitto_sub: client/CMakeFiles/mosquitto_sub.dir/sub_client.c.o
client/mosquitto_sub: lib/libmosquitto.so.1.3.1
client/mosquitto_sub: /usr/lib/x86_64-linux-gnu/libssl.so
client/mosquitto_sub: /usr/lib/x86_64-linux-gnu/libcrypto.so
client/mosquitto_sub: client/CMakeFiles/mosquitto_sub.dir/build.make
client/mosquitto_sub: client/CMakeFiles/mosquitto_sub.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable mosquitto_sub"
	cd /home/gerrits/Documents/Code/im_proj/psn/mosquitto/src/client && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mosquitto_sub.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
client/CMakeFiles/mosquitto_sub.dir/build: client/mosquitto_sub
.PHONY : client/CMakeFiles/mosquitto_sub.dir/build

client/CMakeFiles/mosquitto_sub.dir/requires: client/CMakeFiles/mosquitto_sub.dir/sub_client.c.o.requires
.PHONY : client/CMakeFiles/mosquitto_sub.dir/requires

client/CMakeFiles/mosquitto_sub.dir/clean:
	cd /home/gerrits/Documents/Code/im_proj/psn/mosquitto/src/client && $(CMAKE_COMMAND) -P CMakeFiles/mosquitto_sub.dir/cmake_clean.cmake
.PHONY : client/CMakeFiles/mosquitto_sub.dir/clean

client/CMakeFiles/mosquitto_sub.dir/depend:
	cd /home/gerrits/Documents/Code/im_proj/psn/mosquitto/src && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gerrits/Documents/Code/im_proj/psn/mosquitto /home/gerrits/Documents/Code/im_proj/psn/mosquitto/client /home/gerrits/Documents/Code/im_proj/psn/mosquitto/src /home/gerrits/Documents/Code/im_proj/psn/mosquitto/src/client /home/gerrits/Documents/Code/im_proj/psn/mosquitto/src/client/CMakeFiles/mosquitto_sub.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : client/CMakeFiles/mosquitto_sub.dir/depend

