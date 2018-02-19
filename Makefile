# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_SOURCE_DIR = /home/ironlake/work/DISC

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ironlake/work/DISC

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/ironlake/work/DISC/CMakeFiles /home/ironlake/work/DISC/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/ironlake/work/DISC/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named libDisC

# Build rule for target.
libDisC: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 libDisC
.PHONY : libDisC

# fast build rule for target.
libDisC/fast:
	$(MAKE) -f CMakeFiles/libDisC.dir/build.make CMakeFiles/libDisC.dir/build
.PHONY : libDisC/fast

src/DisC.o: src/DisC.c.o

.PHONY : src/DisC.o

# target to build an object file
src/DisC.c.o:
	$(MAKE) -f CMakeFiles/libDisC.dir/build.make CMakeFiles/libDisC.dir/src/DisC.c.o
.PHONY : src/DisC.c.o

src/DisC.i: src/DisC.c.i

.PHONY : src/DisC.i

# target to preprocess a source file
src/DisC.c.i:
	$(MAKE) -f CMakeFiles/libDisC.dir/build.make CMakeFiles/libDisC.dir/src/DisC.c.i
.PHONY : src/DisC.c.i

src/DisC.s: src/DisC.c.s

.PHONY : src/DisC.s

# target to generate assembly for a file
src/DisC.c.s:
	$(MAKE) -f CMakeFiles/libDisC.dir/build.make CMakeFiles/libDisC.dir/src/DisC.c.s
.PHONY : src/DisC.c.s

src/DisC_errors.o: src/DisC_errors.c.o

.PHONY : src/DisC_errors.o

# target to build an object file
src/DisC_errors.c.o:
	$(MAKE) -f CMakeFiles/libDisC.dir/build.make CMakeFiles/libDisC.dir/src/DisC_errors.c.o
.PHONY : src/DisC_errors.c.o

src/DisC_errors.i: src/DisC_errors.c.i

.PHONY : src/DisC_errors.i

# target to preprocess a source file
src/DisC_errors.c.i:
	$(MAKE) -f CMakeFiles/libDisC.dir/build.make CMakeFiles/libDisC.dir/src/DisC_errors.c.i
.PHONY : src/DisC_errors.c.i

src/DisC_errors.s: src/DisC_errors.c.s

.PHONY : src/DisC_errors.s

# target to generate assembly for a file
src/DisC_errors.c.s:
	$(MAKE) -f CMakeFiles/libDisC.dir/build.make CMakeFiles/libDisC.dir/src/DisC_errors.c.s
.PHONY : src/DisC_errors.c.s

src/DisC_gateway.o: src/DisC_gateway.c.o

.PHONY : src/DisC_gateway.o

# target to build an object file
src/DisC_gateway.c.o:
	$(MAKE) -f CMakeFiles/libDisC.dir/build.make CMakeFiles/libDisC.dir/src/DisC_gateway.c.o
.PHONY : src/DisC_gateway.c.o

src/DisC_gateway.i: src/DisC_gateway.c.i

.PHONY : src/DisC_gateway.i

# target to preprocess a source file
src/DisC_gateway.c.i:
	$(MAKE) -f CMakeFiles/libDisC.dir/build.make CMakeFiles/libDisC.dir/src/DisC_gateway.c.i
.PHONY : src/DisC_gateway.c.i

src/DisC_gateway.s: src/DisC_gateway.c.s

.PHONY : src/DisC_gateway.s

# target to generate assembly for a file
src/DisC_gateway.c.s:
	$(MAKE) -f CMakeFiles/libDisC.dir/build.make CMakeFiles/libDisC.dir/src/DisC_gateway.c.s
.PHONY : src/DisC_gateway.c.s

src/DisC_object.o: src/DisC_object.c.o

.PHONY : src/DisC_object.o

# target to build an object file
src/DisC_object.c.o:
	$(MAKE) -f CMakeFiles/libDisC.dir/build.make CMakeFiles/libDisC.dir/src/DisC_object.c.o
.PHONY : src/DisC_object.c.o

src/DisC_object.i: src/DisC_object.c.i

.PHONY : src/DisC_object.i

# target to preprocess a source file
src/DisC_object.c.i:
	$(MAKE) -f CMakeFiles/libDisC.dir/build.make CMakeFiles/libDisC.dir/src/DisC_object.c.i
.PHONY : src/DisC_object.c.i

src/DisC_object.s: src/DisC_object.c.s

.PHONY : src/DisC_object.s

# target to generate assembly for a file
src/DisC_object.c.s:
	$(MAKE) -f CMakeFiles/libDisC.dir/build.make CMakeFiles/libDisC.dir/src/DisC_object.c.s
.PHONY : src/DisC_object.c.s

src/DisC_rest.o: src/DisC_rest.c.o

.PHONY : src/DisC_rest.o

# target to build an object file
src/DisC_rest.c.o:
	$(MAKE) -f CMakeFiles/libDisC.dir/build.make CMakeFiles/libDisC.dir/src/DisC_rest.c.o
.PHONY : src/DisC_rest.c.o

src/DisC_rest.i: src/DisC_rest.c.i

.PHONY : src/DisC_rest.i

# target to preprocess a source file
src/DisC_rest.c.i:
	$(MAKE) -f CMakeFiles/libDisC.dir/build.make CMakeFiles/libDisC.dir/src/DisC_rest.c.i
.PHONY : src/DisC_rest.c.i

src/DisC_rest.s: src/DisC_rest.c.s

.PHONY : src/DisC_rest.s

# target to generate assembly for a file
src/DisC_rest.c.s:
	$(MAKE) -f CMakeFiles/libDisC.dir/build.make CMakeFiles/libDisC.dir/src/DisC_rest.c.s
.PHONY : src/DisC_rest.c.s

src/DisC_util.o: src/DisC_util.c.o

.PHONY : src/DisC_util.o

# target to build an object file
src/DisC_util.c.o:
	$(MAKE) -f CMakeFiles/libDisC.dir/build.make CMakeFiles/libDisC.dir/src/DisC_util.c.o
.PHONY : src/DisC_util.c.o

src/DisC_util.i: src/DisC_util.c.i

.PHONY : src/DisC_util.i

# target to preprocess a source file
src/DisC_util.c.i:
	$(MAKE) -f CMakeFiles/libDisC.dir/build.make CMakeFiles/libDisC.dir/src/DisC_util.c.i
.PHONY : src/DisC_util.c.i

src/DisC_util.s: src/DisC_util.c.s

.PHONY : src/DisC_util.s

# target to generate assembly for a file
src/DisC_util.c.s:
	$(MAKE) -f CMakeFiles/libDisC.dir/build.make CMakeFiles/libDisC.dir/src/DisC_util.c.s
.PHONY : src/DisC_util.c.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... libDisC"
	@echo "... edit_cache"
	@echo "... src/DisC.o"
	@echo "... src/DisC.i"
	@echo "... src/DisC.s"
	@echo "... src/DisC_errors.o"
	@echo "... src/DisC_errors.i"
	@echo "... src/DisC_errors.s"
	@echo "... src/DisC_gateway.o"
	@echo "... src/DisC_gateway.i"
	@echo "... src/DisC_gateway.s"
	@echo "... src/DisC_object.o"
	@echo "... src/DisC_object.i"
	@echo "... src/DisC_object.s"
	@echo "... src/DisC_rest.o"
	@echo "... src/DisC_rest.i"
	@echo "... src/DisC_rest.s"
	@echo "... src/DisC_util.o"
	@echo "... src/DisC_util.i"
	@echo "... src/DisC_util.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

