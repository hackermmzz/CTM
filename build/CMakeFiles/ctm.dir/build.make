# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/mmzz/CTM

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/mmzz/CTM/build

# Include any dependencies generated for this target.
include CMakeFiles/ctm.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/ctm.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ctm.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ctm.dir/flags.make

CMakeFiles/ctm.dir/Lexifier.cpp.o: CMakeFiles/ctm.dir/flags.make
CMakeFiles/ctm.dir/Lexifier.cpp.o: /root/mmzz/CTM/Lexifier.cpp
CMakeFiles/ctm.dir/Lexifier.cpp.o: CMakeFiles/ctm.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/root/mmzz/CTM/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ctm.dir/Lexifier.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ctm.dir/Lexifier.cpp.o -MF CMakeFiles/ctm.dir/Lexifier.cpp.o.d -o CMakeFiles/ctm.dir/Lexifier.cpp.o -c /root/mmzz/CTM/Lexifier.cpp

CMakeFiles/ctm.dir/Lexifier.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ctm.dir/Lexifier.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/mmzz/CTM/Lexifier.cpp > CMakeFiles/ctm.dir/Lexifier.cpp.i

CMakeFiles/ctm.dir/Lexifier.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ctm.dir/Lexifier.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/mmzz/CTM/Lexifier.cpp -o CMakeFiles/ctm.dir/Lexifier.cpp.s

CMakeFiles/ctm.dir/Parser.cpp.o: CMakeFiles/ctm.dir/flags.make
CMakeFiles/ctm.dir/Parser.cpp.o: /root/mmzz/CTM/Parser.cpp
CMakeFiles/ctm.dir/Parser.cpp.o: CMakeFiles/ctm.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/root/mmzz/CTM/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/ctm.dir/Parser.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ctm.dir/Parser.cpp.o -MF CMakeFiles/ctm.dir/Parser.cpp.o.d -o CMakeFiles/ctm.dir/Parser.cpp.o -c /root/mmzz/CTM/Parser.cpp

CMakeFiles/ctm.dir/Parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ctm.dir/Parser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/mmzz/CTM/Parser.cpp > CMakeFiles/ctm.dir/Parser.cpp.i

CMakeFiles/ctm.dir/Parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ctm.dir/Parser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/mmzz/CTM/Parser.cpp -o CMakeFiles/ctm.dir/Parser.cpp.s

CMakeFiles/ctm.dir/debug.cpp.o: CMakeFiles/ctm.dir/flags.make
CMakeFiles/ctm.dir/debug.cpp.o: /root/mmzz/CTM/debug.cpp
CMakeFiles/ctm.dir/debug.cpp.o: CMakeFiles/ctm.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/root/mmzz/CTM/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/ctm.dir/debug.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ctm.dir/debug.cpp.o -MF CMakeFiles/ctm.dir/debug.cpp.o.d -o CMakeFiles/ctm.dir/debug.cpp.o -c /root/mmzz/CTM/debug.cpp

CMakeFiles/ctm.dir/debug.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ctm.dir/debug.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/mmzz/CTM/debug.cpp > CMakeFiles/ctm.dir/debug.cpp.i

CMakeFiles/ctm.dir/debug.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ctm.dir/debug.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/mmzz/CTM/debug.cpp -o CMakeFiles/ctm.dir/debug.cpp.s

CMakeFiles/ctm.dir/main.cpp.o: CMakeFiles/ctm.dir/flags.make
CMakeFiles/ctm.dir/main.cpp.o: /root/mmzz/CTM/main.cpp
CMakeFiles/ctm.dir/main.cpp.o: CMakeFiles/ctm.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/root/mmzz/CTM/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/ctm.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ctm.dir/main.cpp.o -MF CMakeFiles/ctm.dir/main.cpp.o.d -o CMakeFiles/ctm.dir/main.cpp.o -c /root/mmzz/CTM/main.cpp

CMakeFiles/ctm.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ctm.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/mmzz/CTM/main.cpp > CMakeFiles/ctm.dir/main.cpp.i

CMakeFiles/ctm.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ctm.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/mmzz/CTM/main.cpp -o CMakeFiles/ctm.dir/main.cpp.s

# Object files for target ctm
ctm_OBJECTS = \
"CMakeFiles/ctm.dir/Lexifier.cpp.o" \
"CMakeFiles/ctm.dir/Parser.cpp.o" \
"CMakeFiles/ctm.dir/debug.cpp.o" \
"CMakeFiles/ctm.dir/main.cpp.o"

# External object files for target ctm
ctm_EXTERNAL_OBJECTS =

ctm: CMakeFiles/ctm.dir/Lexifier.cpp.o
ctm: CMakeFiles/ctm.dir/Parser.cpp.o
ctm: CMakeFiles/ctm.dir/debug.cpp.o
ctm: CMakeFiles/ctm.dir/main.cpp.o
ctm: CMakeFiles/ctm.dir/build.make
ctm: CMakeFiles/ctm.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/root/mmzz/CTM/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable ctm"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ctm.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ctm.dir/build: ctm
.PHONY : CMakeFiles/ctm.dir/build

CMakeFiles/ctm.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ctm.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ctm.dir/clean

CMakeFiles/ctm.dir/depend:
	cd /root/mmzz/CTM/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/mmzz/CTM /root/mmzz/CTM /root/mmzz/CTM/build /root/mmzz/CTM/build /root/mmzz/CTM/build/CMakeFiles/ctm.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/ctm.dir/depend

