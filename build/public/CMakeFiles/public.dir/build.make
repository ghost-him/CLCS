# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

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
CMAKE_SOURCE_DIR = /home/ghost-him/CLionProjects/CLCS

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ghost-him/CLionProjects/CLCS/build

# Include any dependencies generated for this target.
include public/CMakeFiles/public.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include public/CMakeFiles/public.dir/compiler_depend.make

# Include the progress variables for this target.
include public/CMakeFiles/public.dir/progress.make

# Include the compile flags for this target's objects.
include public/CMakeFiles/public.dir/flags.make

public/CMakeFiles/public.dir/resource/Command_Analysis.cc.o: public/CMakeFiles/public.dir/flags.make
public/CMakeFiles/public.dir/resource/Command_Analysis.cc.o: /home/ghost-him/CLionProjects/CLCS/public/resource/Command_Analysis.cc
public/CMakeFiles/public.dir/resource/Command_Analysis.cc.o: public/CMakeFiles/public.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ghost-him/CLionProjects/CLCS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object public/CMakeFiles/public.dir/resource/Command_Analysis.cc.o"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT public/CMakeFiles/public.dir/resource/Command_Analysis.cc.o -MF CMakeFiles/public.dir/resource/Command_Analysis.cc.o.d -o CMakeFiles/public.dir/resource/Command_Analysis.cc.o -c /home/ghost-him/CLionProjects/CLCS/public/resource/Command_Analysis.cc

public/CMakeFiles/public.dir/resource/Command_Analysis.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/public.dir/resource/Command_Analysis.cc.i"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ghost-him/CLionProjects/CLCS/public/resource/Command_Analysis.cc > CMakeFiles/public.dir/resource/Command_Analysis.cc.i

public/CMakeFiles/public.dir/resource/Command_Analysis.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/public.dir/resource/Command_Analysis.cc.s"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ghost-him/CLionProjects/CLCS/public/resource/Command_Analysis.cc -o CMakeFiles/public.dir/resource/Command_Analysis.cc.s

public/CMakeFiles/public.dir/resource/Command_Service.cc.o: public/CMakeFiles/public.dir/flags.make
public/CMakeFiles/public.dir/resource/Command_Service.cc.o: /home/ghost-him/CLionProjects/CLCS/public/resource/Command_Service.cc
public/CMakeFiles/public.dir/resource/Command_Service.cc.o: public/CMakeFiles/public.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ghost-him/CLionProjects/CLCS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object public/CMakeFiles/public.dir/resource/Command_Service.cc.o"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT public/CMakeFiles/public.dir/resource/Command_Service.cc.o -MF CMakeFiles/public.dir/resource/Command_Service.cc.o.d -o CMakeFiles/public.dir/resource/Command_Service.cc.o -c /home/ghost-him/CLionProjects/CLCS/public/resource/Command_Service.cc

public/CMakeFiles/public.dir/resource/Command_Service.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/public.dir/resource/Command_Service.cc.i"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ghost-him/CLionProjects/CLCS/public/resource/Command_Service.cc > CMakeFiles/public.dir/resource/Command_Service.cc.i

public/CMakeFiles/public.dir/resource/Command_Service.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/public.dir/resource/Command_Service.cc.s"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ghost-him/CLionProjects/CLCS/public/resource/Command_Service.cc -o CMakeFiles/public.dir/resource/Command_Service.cc.s

public/CMakeFiles/public.dir/resource/Default.cc.o: public/CMakeFiles/public.dir/flags.make
public/CMakeFiles/public.dir/resource/Default.cc.o: /home/ghost-him/CLionProjects/CLCS/public/resource/Default.cc
public/CMakeFiles/public.dir/resource/Default.cc.o: public/CMakeFiles/public.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ghost-him/CLionProjects/CLCS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object public/CMakeFiles/public.dir/resource/Default.cc.o"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT public/CMakeFiles/public.dir/resource/Default.cc.o -MF CMakeFiles/public.dir/resource/Default.cc.o.d -o CMakeFiles/public.dir/resource/Default.cc.o -c /home/ghost-him/CLionProjects/CLCS/public/resource/Default.cc

public/CMakeFiles/public.dir/resource/Default.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/public.dir/resource/Default.cc.i"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ghost-him/CLionProjects/CLCS/public/resource/Default.cc > CMakeFiles/public.dir/resource/Default.cc.i

public/CMakeFiles/public.dir/resource/Default.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/public.dir/resource/Default.cc.s"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ghost-him/CLionProjects/CLCS/public/resource/Default.cc -o CMakeFiles/public.dir/resource/Default.cc.s

public/CMakeFiles/public.dir/resource/FileManager.cc.o: public/CMakeFiles/public.dir/flags.make
public/CMakeFiles/public.dir/resource/FileManager.cc.o: /home/ghost-him/CLionProjects/CLCS/public/resource/FileManager.cc
public/CMakeFiles/public.dir/resource/FileManager.cc.o: public/CMakeFiles/public.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ghost-him/CLionProjects/CLCS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object public/CMakeFiles/public.dir/resource/FileManager.cc.o"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT public/CMakeFiles/public.dir/resource/FileManager.cc.o -MF CMakeFiles/public.dir/resource/FileManager.cc.o.d -o CMakeFiles/public.dir/resource/FileManager.cc.o -c /home/ghost-him/CLionProjects/CLCS/public/resource/FileManager.cc

public/CMakeFiles/public.dir/resource/FileManager.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/public.dir/resource/FileManager.cc.i"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ghost-him/CLionProjects/CLCS/public/resource/FileManager.cc > CMakeFiles/public.dir/resource/FileManager.cc.i

public/CMakeFiles/public.dir/resource/FileManager.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/public.dir/resource/FileManager.cc.s"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ghost-him/CLionProjects/CLCS/public/resource/FileManager.cc -o CMakeFiles/public.dir/resource/FileManager.cc.s

public/CMakeFiles/public.dir/resource/Init.cc.o: public/CMakeFiles/public.dir/flags.make
public/CMakeFiles/public.dir/resource/Init.cc.o: /home/ghost-him/CLionProjects/CLCS/public/resource/Init.cc
public/CMakeFiles/public.dir/resource/Init.cc.o: public/CMakeFiles/public.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ghost-him/CLionProjects/CLCS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object public/CMakeFiles/public.dir/resource/Init.cc.o"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT public/CMakeFiles/public.dir/resource/Init.cc.o -MF CMakeFiles/public.dir/resource/Init.cc.o.d -o CMakeFiles/public.dir/resource/Init.cc.o -c /home/ghost-him/CLionProjects/CLCS/public/resource/Init.cc

public/CMakeFiles/public.dir/resource/Init.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/public.dir/resource/Init.cc.i"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ghost-him/CLionProjects/CLCS/public/resource/Init.cc > CMakeFiles/public.dir/resource/Init.cc.i

public/CMakeFiles/public.dir/resource/Init.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/public.dir/resource/Init.cc.s"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ghost-him/CLionProjects/CLCS/public/resource/Init.cc -o CMakeFiles/public.dir/resource/Init.cc.s

public/CMakeFiles/public.dir/resource/Language.cc.o: public/CMakeFiles/public.dir/flags.make
public/CMakeFiles/public.dir/resource/Language.cc.o: /home/ghost-him/CLionProjects/CLCS/public/resource/Language.cc
public/CMakeFiles/public.dir/resource/Language.cc.o: public/CMakeFiles/public.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ghost-him/CLionProjects/CLCS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object public/CMakeFiles/public.dir/resource/Language.cc.o"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT public/CMakeFiles/public.dir/resource/Language.cc.o -MF CMakeFiles/public.dir/resource/Language.cc.o.d -o CMakeFiles/public.dir/resource/Language.cc.o -c /home/ghost-him/CLionProjects/CLCS/public/resource/Language.cc

public/CMakeFiles/public.dir/resource/Language.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/public.dir/resource/Language.cc.i"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ghost-him/CLionProjects/CLCS/public/resource/Language.cc > CMakeFiles/public.dir/resource/Language.cc.i

public/CMakeFiles/public.dir/resource/Language.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/public.dir/resource/Language.cc.s"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ghost-him/CLionProjects/CLCS/public/resource/Language.cc -o CMakeFiles/public.dir/resource/Language.cc.s

public/CMakeFiles/public.dir/resource/Log.cc.o: public/CMakeFiles/public.dir/flags.make
public/CMakeFiles/public.dir/resource/Log.cc.o: /home/ghost-him/CLionProjects/CLCS/public/resource/Log.cc
public/CMakeFiles/public.dir/resource/Log.cc.o: public/CMakeFiles/public.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ghost-him/CLionProjects/CLCS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object public/CMakeFiles/public.dir/resource/Log.cc.o"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT public/CMakeFiles/public.dir/resource/Log.cc.o -MF CMakeFiles/public.dir/resource/Log.cc.o.d -o CMakeFiles/public.dir/resource/Log.cc.o -c /home/ghost-him/CLionProjects/CLCS/public/resource/Log.cc

public/CMakeFiles/public.dir/resource/Log.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/public.dir/resource/Log.cc.i"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ghost-him/CLionProjects/CLCS/public/resource/Log.cc > CMakeFiles/public.dir/resource/Log.cc.i

public/CMakeFiles/public.dir/resource/Log.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/public.dir/resource/Log.cc.s"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ghost-him/CLionProjects/CLCS/public/resource/Log.cc -o CMakeFiles/public.dir/resource/Log.cc.s

public/CMakeFiles/public.dir/resource/Message.cc.o: public/CMakeFiles/public.dir/flags.make
public/CMakeFiles/public.dir/resource/Message.cc.o: /home/ghost-him/CLionProjects/CLCS/public/resource/Message.cc
public/CMakeFiles/public.dir/resource/Message.cc.o: public/CMakeFiles/public.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ghost-him/CLionProjects/CLCS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object public/CMakeFiles/public.dir/resource/Message.cc.o"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT public/CMakeFiles/public.dir/resource/Message.cc.o -MF CMakeFiles/public.dir/resource/Message.cc.o.d -o CMakeFiles/public.dir/resource/Message.cc.o -c /home/ghost-him/CLionProjects/CLCS/public/resource/Message.cc

public/CMakeFiles/public.dir/resource/Message.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/public.dir/resource/Message.cc.i"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ghost-him/CLionProjects/CLCS/public/resource/Message.cc > CMakeFiles/public.dir/resource/Message.cc.i

public/CMakeFiles/public.dir/resource/Message.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/public.dir/resource/Message.cc.s"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ghost-him/CLionProjects/CLCS/public/resource/Message.cc -o CMakeFiles/public.dir/resource/Message.cc.s

public/CMakeFiles/public.dir/resource/Security.cc.o: public/CMakeFiles/public.dir/flags.make
public/CMakeFiles/public.dir/resource/Security.cc.o: /home/ghost-him/CLionProjects/CLCS/public/resource/Security.cc
public/CMakeFiles/public.dir/resource/Security.cc.o: public/CMakeFiles/public.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ghost-him/CLionProjects/CLCS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object public/CMakeFiles/public.dir/resource/Security.cc.o"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT public/CMakeFiles/public.dir/resource/Security.cc.o -MF CMakeFiles/public.dir/resource/Security.cc.o.d -o CMakeFiles/public.dir/resource/Security.cc.o -c /home/ghost-him/CLionProjects/CLCS/public/resource/Security.cc

public/CMakeFiles/public.dir/resource/Security.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/public.dir/resource/Security.cc.i"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ghost-him/CLionProjects/CLCS/public/resource/Security.cc > CMakeFiles/public.dir/resource/Security.cc.i

public/CMakeFiles/public.dir/resource/Security.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/public.dir/resource/Security.cc.s"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ghost-him/CLionProjects/CLCS/public/resource/Security.cc -o CMakeFiles/public.dir/resource/Security.cc.s

public/CMakeFiles/public.dir/resource/Setting.cc.o: public/CMakeFiles/public.dir/flags.make
public/CMakeFiles/public.dir/resource/Setting.cc.o: /home/ghost-him/CLionProjects/CLCS/public/resource/Setting.cc
public/CMakeFiles/public.dir/resource/Setting.cc.o: public/CMakeFiles/public.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ghost-him/CLionProjects/CLCS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object public/CMakeFiles/public.dir/resource/Setting.cc.o"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT public/CMakeFiles/public.dir/resource/Setting.cc.o -MF CMakeFiles/public.dir/resource/Setting.cc.o.d -o CMakeFiles/public.dir/resource/Setting.cc.o -c /home/ghost-him/CLionProjects/CLCS/public/resource/Setting.cc

public/CMakeFiles/public.dir/resource/Setting.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/public.dir/resource/Setting.cc.i"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ghost-him/CLionProjects/CLCS/public/resource/Setting.cc > CMakeFiles/public.dir/resource/Setting.cc.i

public/CMakeFiles/public.dir/resource/Setting.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/public.dir/resource/Setting.cc.s"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ghost-him/CLionProjects/CLCS/public/resource/Setting.cc -o CMakeFiles/public.dir/resource/Setting.cc.s

public/CMakeFiles/public.dir/resource/ThreadPool.cc.o: public/CMakeFiles/public.dir/flags.make
public/CMakeFiles/public.dir/resource/ThreadPool.cc.o: /home/ghost-him/CLionProjects/CLCS/public/resource/ThreadPool.cc
public/CMakeFiles/public.dir/resource/ThreadPool.cc.o: public/CMakeFiles/public.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ghost-him/CLionProjects/CLCS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object public/CMakeFiles/public.dir/resource/ThreadPool.cc.o"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT public/CMakeFiles/public.dir/resource/ThreadPool.cc.o -MF CMakeFiles/public.dir/resource/ThreadPool.cc.o.d -o CMakeFiles/public.dir/resource/ThreadPool.cc.o -c /home/ghost-him/CLionProjects/CLCS/public/resource/ThreadPool.cc

public/CMakeFiles/public.dir/resource/ThreadPool.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/public.dir/resource/ThreadPool.cc.i"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ghost-him/CLionProjects/CLCS/public/resource/ThreadPool.cc > CMakeFiles/public.dir/resource/ThreadPool.cc.i

public/CMakeFiles/public.dir/resource/ThreadPool.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/public.dir/resource/ThreadPool.cc.s"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ghost-him/CLionProjects/CLCS/public/resource/ThreadPool.cc -o CMakeFiles/public.dir/resource/ThreadPool.cc.s

public/CMakeFiles/public.dir/resource/User.cc.o: public/CMakeFiles/public.dir/flags.make
public/CMakeFiles/public.dir/resource/User.cc.o: /home/ghost-him/CLionProjects/CLCS/public/resource/User.cc
public/CMakeFiles/public.dir/resource/User.cc.o: public/CMakeFiles/public.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ghost-him/CLionProjects/CLCS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object public/CMakeFiles/public.dir/resource/User.cc.o"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT public/CMakeFiles/public.dir/resource/User.cc.o -MF CMakeFiles/public.dir/resource/User.cc.o.d -o CMakeFiles/public.dir/resource/User.cc.o -c /home/ghost-him/CLionProjects/CLCS/public/resource/User.cc

public/CMakeFiles/public.dir/resource/User.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/public.dir/resource/User.cc.i"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ghost-him/CLionProjects/CLCS/public/resource/User.cc > CMakeFiles/public.dir/resource/User.cc.i

public/CMakeFiles/public.dir/resource/User.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/public.dir/resource/User.cc.s"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ghost-him/CLionProjects/CLCS/public/resource/User.cc -o CMakeFiles/public.dir/resource/User.cc.s

public/CMakeFiles/public.dir/resource/User_Manager.cc.o: public/CMakeFiles/public.dir/flags.make
public/CMakeFiles/public.dir/resource/User_Manager.cc.o: /home/ghost-him/CLionProjects/CLCS/public/resource/User_Manager.cc
public/CMakeFiles/public.dir/resource/User_Manager.cc.o: public/CMakeFiles/public.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ghost-him/CLionProjects/CLCS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object public/CMakeFiles/public.dir/resource/User_Manager.cc.o"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT public/CMakeFiles/public.dir/resource/User_Manager.cc.o -MF CMakeFiles/public.dir/resource/User_Manager.cc.o.d -o CMakeFiles/public.dir/resource/User_Manager.cc.o -c /home/ghost-him/CLionProjects/CLCS/public/resource/User_Manager.cc

public/CMakeFiles/public.dir/resource/User_Manager.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/public.dir/resource/User_Manager.cc.i"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ghost-him/CLionProjects/CLCS/public/resource/User_Manager.cc > CMakeFiles/public.dir/resource/User_Manager.cc.i

public/CMakeFiles/public.dir/resource/User_Manager.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/public.dir/resource/User_Manager.cc.s"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ghost-him/CLionProjects/CLCS/public/resource/User_Manager.cc -o CMakeFiles/public.dir/resource/User_Manager.cc.s

# Object files for target public
public_OBJECTS = \
"CMakeFiles/public.dir/resource/Command_Analysis.cc.o" \
"CMakeFiles/public.dir/resource/Command_Service.cc.o" \
"CMakeFiles/public.dir/resource/Default.cc.o" \
"CMakeFiles/public.dir/resource/FileManager.cc.o" \
"CMakeFiles/public.dir/resource/Init.cc.o" \
"CMakeFiles/public.dir/resource/Language.cc.o" \
"CMakeFiles/public.dir/resource/Log.cc.o" \
"CMakeFiles/public.dir/resource/Message.cc.o" \
"CMakeFiles/public.dir/resource/Security.cc.o" \
"CMakeFiles/public.dir/resource/Setting.cc.o" \
"CMakeFiles/public.dir/resource/ThreadPool.cc.o" \
"CMakeFiles/public.dir/resource/User.cc.o" \
"CMakeFiles/public.dir/resource/User_Manager.cc.o"

# External object files for target public
public_EXTERNAL_OBJECTS =

public/libpublic.a: public/CMakeFiles/public.dir/resource/Command_Analysis.cc.o
public/libpublic.a: public/CMakeFiles/public.dir/resource/Command_Service.cc.o
public/libpublic.a: public/CMakeFiles/public.dir/resource/Default.cc.o
public/libpublic.a: public/CMakeFiles/public.dir/resource/FileManager.cc.o
public/libpublic.a: public/CMakeFiles/public.dir/resource/Init.cc.o
public/libpublic.a: public/CMakeFiles/public.dir/resource/Language.cc.o
public/libpublic.a: public/CMakeFiles/public.dir/resource/Log.cc.o
public/libpublic.a: public/CMakeFiles/public.dir/resource/Message.cc.o
public/libpublic.a: public/CMakeFiles/public.dir/resource/Security.cc.o
public/libpublic.a: public/CMakeFiles/public.dir/resource/Setting.cc.o
public/libpublic.a: public/CMakeFiles/public.dir/resource/ThreadPool.cc.o
public/libpublic.a: public/CMakeFiles/public.dir/resource/User.cc.o
public/libpublic.a: public/CMakeFiles/public.dir/resource/User_Manager.cc.o
public/libpublic.a: public/CMakeFiles/public.dir/build.make
public/libpublic.a: public/CMakeFiles/public.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ghost-him/CLionProjects/CLCS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Linking CXX static library libpublic.a"
	cd /home/ghost-him/CLionProjects/CLCS/build/public && $(CMAKE_COMMAND) -P CMakeFiles/public.dir/cmake_clean_target.cmake
	cd /home/ghost-him/CLionProjects/CLCS/build/public && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/public.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
public/CMakeFiles/public.dir/build: public/libpublic.a
.PHONY : public/CMakeFiles/public.dir/build

public/CMakeFiles/public.dir/clean:
	cd /home/ghost-him/CLionProjects/CLCS/build/public && $(CMAKE_COMMAND) -P CMakeFiles/public.dir/cmake_clean.cmake
.PHONY : public/CMakeFiles/public.dir/clean

public/CMakeFiles/public.dir/depend:
	cd /home/ghost-him/CLionProjects/CLCS/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ghost-him/CLionProjects/CLCS /home/ghost-him/CLionProjects/CLCS/public /home/ghost-him/CLionProjects/CLCS/build /home/ghost-him/CLionProjects/CLCS/build/public /home/ghost-him/CLionProjects/CLCS/build/public/CMakeFiles/public.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : public/CMakeFiles/public.dir/depend

