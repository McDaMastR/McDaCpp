# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.19.7/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.19.7/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/build

# Include any dependencies generated for this target.
include CMakeFiles/TicTacToe.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/TicTacToe.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TicTacToe.dir/flags.make

CMakeFiles/TicTacToe.dir/src/main.cpp.o: CMakeFiles/TicTacToe.dir/flags.make
CMakeFiles/TicTacToe.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/TicTacToe.dir/src/main.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TicTacToe.dir/src/main.cpp.o -c /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/main.cpp

CMakeFiles/TicTacToe.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TicTacToe.dir/src/main.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/main.cpp > CMakeFiles/TicTacToe.dir/src/main.cpp.i

CMakeFiles/TicTacToe.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TicTacToe.dir/src/main.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/main.cpp -o CMakeFiles/TicTacToe.dir/src/main.cpp.s

CMakeFiles/TicTacToe.dir/src/renderer.cpp.o: CMakeFiles/TicTacToe.dir/flags.make
CMakeFiles/TicTacToe.dir/src/renderer.cpp.o: ../src/renderer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/TicTacToe.dir/src/renderer.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TicTacToe.dir/src/renderer.cpp.o -c /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/renderer.cpp

CMakeFiles/TicTacToe.dir/src/renderer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TicTacToe.dir/src/renderer.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/renderer.cpp > CMakeFiles/TicTacToe.dir/src/renderer.cpp.i

CMakeFiles/TicTacToe.dir/src/renderer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TicTacToe.dir/src/renderer.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/renderer.cpp -o CMakeFiles/TicTacToe.dir/src/renderer.cpp.s

CMakeFiles/TicTacToe.dir/src/vertexArray.cpp.o: CMakeFiles/TicTacToe.dir/flags.make
CMakeFiles/TicTacToe.dir/src/vertexArray.cpp.o: ../src/vertexArray.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/TicTacToe.dir/src/vertexArray.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TicTacToe.dir/src/vertexArray.cpp.o -c /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vertexArray.cpp

CMakeFiles/TicTacToe.dir/src/vertexArray.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TicTacToe.dir/src/vertexArray.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vertexArray.cpp > CMakeFiles/TicTacToe.dir/src/vertexArray.cpp.i

CMakeFiles/TicTacToe.dir/src/vertexArray.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TicTacToe.dir/src/vertexArray.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vertexArray.cpp -o CMakeFiles/TicTacToe.dir/src/vertexArray.cpp.s

CMakeFiles/TicTacToe.dir/src/indexBuffer.cpp.o: CMakeFiles/TicTacToe.dir/flags.make
CMakeFiles/TicTacToe.dir/src/indexBuffer.cpp.o: ../src/indexBuffer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/TicTacToe.dir/src/indexBuffer.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TicTacToe.dir/src/indexBuffer.cpp.o -c /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/indexBuffer.cpp

CMakeFiles/TicTacToe.dir/src/indexBuffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TicTacToe.dir/src/indexBuffer.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/indexBuffer.cpp > CMakeFiles/TicTacToe.dir/src/indexBuffer.cpp.i

CMakeFiles/TicTacToe.dir/src/indexBuffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TicTacToe.dir/src/indexBuffer.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/indexBuffer.cpp -o CMakeFiles/TicTacToe.dir/src/indexBuffer.cpp.s

CMakeFiles/TicTacToe.dir/src/shader.cpp.o: CMakeFiles/TicTacToe.dir/flags.make
CMakeFiles/TicTacToe.dir/src/shader.cpp.o: ../src/shader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/TicTacToe.dir/src/shader.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TicTacToe.dir/src/shader.cpp.o -c /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/shader.cpp

CMakeFiles/TicTacToe.dir/src/shader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TicTacToe.dir/src/shader.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/shader.cpp > CMakeFiles/TicTacToe.dir/src/shader.cpp.i

CMakeFiles/TicTacToe.dir/src/shader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TicTacToe.dir/src/shader.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/shader.cpp -o CMakeFiles/TicTacToe.dir/src/shader.cpp.s

CMakeFiles/TicTacToe.dir/src/vertexBuffer.cpp.o: CMakeFiles/TicTacToe.dir/flags.make
CMakeFiles/TicTacToe.dir/src/vertexBuffer.cpp.o: ../src/vertexBuffer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/TicTacToe.dir/src/vertexBuffer.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TicTacToe.dir/src/vertexBuffer.cpp.o -c /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vertexBuffer.cpp

CMakeFiles/TicTacToe.dir/src/vertexBuffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TicTacToe.dir/src/vertexBuffer.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vertexBuffer.cpp > CMakeFiles/TicTacToe.dir/src/vertexBuffer.cpp.i

CMakeFiles/TicTacToe.dir/src/vertexBuffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TicTacToe.dir/src/vertexBuffer.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vertexBuffer.cpp -o CMakeFiles/TicTacToe.dir/src/vertexBuffer.cpp.s

CMakeFiles/TicTacToe.dir/src/vertexBufferLayout.cpp.o: CMakeFiles/TicTacToe.dir/flags.make
CMakeFiles/TicTacToe.dir/src/vertexBufferLayout.cpp.o: ../src/vertexBufferLayout.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/TicTacToe.dir/src/vertexBufferLayout.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TicTacToe.dir/src/vertexBufferLayout.cpp.o -c /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vertexBufferLayout.cpp

CMakeFiles/TicTacToe.dir/src/vertexBufferLayout.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TicTacToe.dir/src/vertexBufferLayout.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vertexBufferLayout.cpp > CMakeFiles/TicTacToe.dir/src/vertexBufferLayout.cpp.i

CMakeFiles/TicTacToe.dir/src/vertexBufferLayout.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TicTacToe.dir/src/vertexBufferLayout.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vertexBufferLayout.cpp -o CMakeFiles/TicTacToe.dir/src/vertexBufferLayout.cpp.s

CMakeFiles/TicTacToe.dir/src/texture.cpp.o: CMakeFiles/TicTacToe.dir/flags.make
CMakeFiles/TicTacToe.dir/src/texture.cpp.o: ../src/texture.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/TicTacToe.dir/src/texture.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TicTacToe.dir/src/texture.cpp.o -c /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/texture.cpp

CMakeFiles/TicTacToe.dir/src/texture.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TicTacToe.dir/src/texture.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/texture.cpp > CMakeFiles/TicTacToe.dir/src/texture.cpp.i

CMakeFiles/TicTacToe.dir/src/texture.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TicTacToe.dir/src/texture.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/texture.cpp -o CMakeFiles/TicTacToe.dir/src/texture.cpp.s

CMakeFiles/TicTacToe.dir/src/vertexBufferElement.cpp.o: CMakeFiles/TicTacToe.dir/flags.make
CMakeFiles/TicTacToe.dir/src/vertexBufferElement.cpp.o: ../src/vertexBufferElement.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/TicTacToe.dir/src/vertexBufferElement.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TicTacToe.dir/src/vertexBufferElement.cpp.o -c /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vertexBufferElement.cpp

CMakeFiles/TicTacToe.dir/src/vertexBufferElement.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TicTacToe.dir/src/vertexBufferElement.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vertexBufferElement.cpp > CMakeFiles/TicTacToe.dir/src/vertexBufferElement.cpp.i

CMakeFiles/TicTacToe.dir/src/vertexBufferElement.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TicTacToe.dir/src/vertexBufferElement.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vertexBufferElement.cpp -o CMakeFiles/TicTacToe.dir/src/vertexBufferElement.cpp.s

CMakeFiles/TicTacToe.dir/src/blender.cpp.o: CMakeFiles/TicTacToe.dir/flags.make
CMakeFiles/TicTacToe.dir/src/blender.cpp.o: ../src/blender.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/TicTacToe.dir/src/blender.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TicTacToe.dir/src/blender.cpp.o -c /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/blender.cpp

CMakeFiles/TicTacToe.dir/src/blender.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TicTacToe.dir/src/blender.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/blender.cpp > CMakeFiles/TicTacToe.dir/src/blender.cpp.i

CMakeFiles/TicTacToe.dir/src/blender.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TicTacToe.dir/src/blender.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/blender.cpp -o CMakeFiles/TicTacToe.dir/src/blender.cpp.s

CMakeFiles/TicTacToe.dir/src/events.cpp.o: CMakeFiles/TicTacToe.dir/flags.make
CMakeFiles/TicTacToe.dir/src/events.cpp.o: ../src/events.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/TicTacToe.dir/src/events.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TicTacToe.dir/src/events.cpp.o -c /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/events.cpp

CMakeFiles/TicTacToe.dir/src/events.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TicTacToe.dir/src/events.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/events.cpp > CMakeFiles/TicTacToe.dir/src/events.cpp.i

CMakeFiles/TicTacToe.dir/src/events.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TicTacToe.dir/src/events.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/events.cpp -o CMakeFiles/TicTacToe.dir/src/events.cpp.s

CMakeFiles/TicTacToe.dir/src/window.cpp.o: CMakeFiles/TicTacToe.dir/flags.make
CMakeFiles/TicTacToe.dir/src/window.cpp.o: ../src/window.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/TicTacToe.dir/src/window.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TicTacToe.dir/src/window.cpp.o -c /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/window.cpp

CMakeFiles/TicTacToe.dir/src/window.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TicTacToe.dir/src/window.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/window.cpp > CMakeFiles/TicTacToe.dir/src/window.cpp.i

CMakeFiles/TicTacToe.dir/src/window.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TicTacToe.dir/src/window.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/window.cpp -o CMakeFiles/TicTacToe.dir/src/window.cpp.s

CMakeFiles/TicTacToe.dir/src/board.cpp.o: CMakeFiles/TicTacToe.dir/flags.make
CMakeFiles/TicTacToe.dir/src/board.cpp.o: ../src/board.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/TicTacToe.dir/src/board.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TicTacToe.dir/src/board.cpp.o -c /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/board.cpp

CMakeFiles/TicTacToe.dir/src/board.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TicTacToe.dir/src/board.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/board.cpp > CMakeFiles/TicTacToe.dir/src/board.cpp.i

CMakeFiles/TicTacToe.dir/src/board.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TicTacToe.dir/src/board.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/board.cpp -o CMakeFiles/TicTacToe.dir/src/board.cpp.s

CMakeFiles/TicTacToe.dir/src/piece.cpp.o: CMakeFiles/TicTacToe.dir/flags.make
CMakeFiles/TicTacToe.dir/src/piece.cpp.o: ../src/piece.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/TicTacToe.dir/src/piece.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TicTacToe.dir/src/piece.cpp.o -c /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/piece.cpp

CMakeFiles/TicTacToe.dir/src/piece.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TicTacToe.dir/src/piece.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/piece.cpp > CMakeFiles/TicTacToe.dir/src/piece.cpp.i

CMakeFiles/TicTacToe.dir/src/piece.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TicTacToe.dir/src/piece.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/piece.cpp -o CMakeFiles/TicTacToe.dir/src/piece.cpp.s

CMakeFiles/TicTacToe.dir/src/dearImGui.cpp.o: CMakeFiles/TicTacToe.dir/flags.make
CMakeFiles/TicTacToe.dir/src/dearImGui.cpp.o: ../src/dearImGui.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building CXX object CMakeFiles/TicTacToe.dir/src/dearImGui.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TicTacToe.dir/src/dearImGui.cpp.o -c /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/dearImGui.cpp

CMakeFiles/TicTacToe.dir/src/dearImGui.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TicTacToe.dir/src/dearImGui.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/dearImGui.cpp > CMakeFiles/TicTacToe.dir/src/dearImGui.cpp.i

CMakeFiles/TicTacToe.dir/src/dearImGui.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TicTacToe.dir/src/dearImGui.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/dearImGui.cpp -o CMakeFiles/TicTacToe.dir/src/dearImGui.cpp.s

CMakeFiles/TicTacToe.dir/src/vendor/stb_image/stb_image.cpp.o: CMakeFiles/TicTacToe.dir/flags.make
CMakeFiles/TicTacToe.dir/src/vendor/stb_image/stb_image.cpp.o: ../src/vendor/stb_image/stb_image.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Building CXX object CMakeFiles/TicTacToe.dir/src/vendor/stb_image/stb_image.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TicTacToe.dir/src/vendor/stb_image/stb_image.cpp.o -c /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vendor/stb_image/stb_image.cpp

CMakeFiles/TicTacToe.dir/src/vendor/stb_image/stb_image.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TicTacToe.dir/src/vendor/stb_image/stb_image.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vendor/stb_image/stb_image.cpp > CMakeFiles/TicTacToe.dir/src/vendor/stb_image/stb_image.cpp.i

CMakeFiles/TicTacToe.dir/src/vendor/stb_image/stb_image.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TicTacToe.dir/src/vendor/stb_image/stb_image.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vendor/stb_image/stb_image.cpp -o CMakeFiles/TicTacToe.dir/src/vendor/stb_image/stb_image.cpp.s

CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_demo.cpp.o: CMakeFiles/TicTacToe.dir/flags.make
CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_demo.cpp.o: ../src/vendor/imGui/imgui_demo.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_17) "Building CXX object CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_demo.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_demo.cpp.o -c /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vendor/imGui/imgui_demo.cpp

CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_demo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_demo.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vendor/imGui/imgui_demo.cpp > CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_demo.cpp.i

CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_demo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_demo.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vendor/imGui/imgui_demo.cpp -o CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_demo.cpp.s

CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_draw.cpp.o: CMakeFiles/TicTacToe.dir/flags.make
CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_draw.cpp.o: ../src/vendor/imGui/imgui_draw.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_18) "Building CXX object CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_draw.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_draw.cpp.o -c /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vendor/imGui/imgui_draw.cpp

CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_draw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_draw.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vendor/imGui/imgui_draw.cpp > CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_draw.cpp.i

CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_draw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_draw.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vendor/imGui/imgui_draw.cpp -o CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_draw.cpp.s

CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_impl_glfw.cpp.o: CMakeFiles/TicTacToe.dir/flags.make
CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_impl_glfw.cpp.o: ../src/vendor/imGui/imgui_impl_glfw.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_19) "Building CXX object CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_impl_glfw.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_impl_glfw.cpp.o -c /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vendor/imGui/imgui_impl_glfw.cpp

CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_impl_glfw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_impl_glfw.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vendor/imGui/imgui_impl_glfw.cpp > CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_impl_glfw.cpp.i

CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_impl_glfw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_impl_glfw.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vendor/imGui/imgui_impl_glfw.cpp -o CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_impl_glfw.cpp.s

CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_impl_opengl3.cpp.o: CMakeFiles/TicTacToe.dir/flags.make
CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_impl_opengl3.cpp.o: ../src/vendor/imGui/imgui_impl_opengl3.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_20) "Building CXX object CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_impl_opengl3.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_impl_opengl3.cpp.o -c /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vendor/imGui/imgui_impl_opengl3.cpp

CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_impl_opengl3.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_impl_opengl3.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vendor/imGui/imgui_impl_opengl3.cpp > CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_impl_opengl3.cpp.i

CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_impl_opengl3.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_impl_opengl3.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vendor/imGui/imgui_impl_opengl3.cpp -o CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_impl_opengl3.cpp.s

CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_tables.cpp.o: CMakeFiles/TicTacToe.dir/flags.make
CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_tables.cpp.o: ../src/vendor/imGui/imgui_tables.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_21) "Building CXX object CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_tables.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_tables.cpp.o -c /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vendor/imGui/imgui_tables.cpp

CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_tables.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_tables.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vendor/imGui/imgui_tables.cpp > CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_tables.cpp.i

CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_tables.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_tables.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vendor/imGui/imgui_tables.cpp -o CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_tables.cpp.s

CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_widgets.cpp.o: CMakeFiles/TicTacToe.dir/flags.make
CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_widgets.cpp.o: ../src/vendor/imGui/imgui_widgets.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_22) "Building CXX object CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_widgets.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_widgets.cpp.o -c /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vendor/imGui/imgui_widgets.cpp

CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_widgets.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_widgets.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vendor/imGui/imgui_widgets.cpp > CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_widgets.cpp.i

CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_widgets.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_widgets.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vendor/imGui/imgui_widgets.cpp -o CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_widgets.cpp.s

CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui.cpp.o: CMakeFiles/TicTacToe.dir/flags.make
CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui.cpp.o: ../src/vendor/imGui/imgui.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_23) "Building CXX object CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui.cpp.o -c /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vendor/imGui/imgui.cpp

CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vendor/imGui/imgui.cpp > CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui.cpp.i

CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/src/vendor/imGui/imgui.cpp -o CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui.cpp.s

# Object files for target TicTacToe
TicTacToe_OBJECTS = \
"CMakeFiles/TicTacToe.dir/src/main.cpp.o" \
"CMakeFiles/TicTacToe.dir/src/renderer.cpp.o" \
"CMakeFiles/TicTacToe.dir/src/vertexArray.cpp.o" \
"CMakeFiles/TicTacToe.dir/src/indexBuffer.cpp.o" \
"CMakeFiles/TicTacToe.dir/src/shader.cpp.o" \
"CMakeFiles/TicTacToe.dir/src/vertexBuffer.cpp.o" \
"CMakeFiles/TicTacToe.dir/src/vertexBufferLayout.cpp.o" \
"CMakeFiles/TicTacToe.dir/src/texture.cpp.o" \
"CMakeFiles/TicTacToe.dir/src/vertexBufferElement.cpp.o" \
"CMakeFiles/TicTacToe.dir/src/blender.cpp.o" \
"CMakeFiles/TicTacToe.dir/src/events.cpp.o" \
"CMakeFiles/TicTacToe.dir/src/window.cpp.o" \
"CMakeFiles/TicTacToe.dir/src/board.cpp.o" \
"CMakeFiles/TicTacToe.dir/src/piece.cpp.o" \
"CMakeFiles/TicTacToe.dir/src/dearImGui.cpp.o" \
"CMakeFiles/TicTacToe.dir/src/vendor/stb_image/stb_image.cpp.o" \
"CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_demo.cpp.o" \
"CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_draw.cpp.o" \
"CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_impl_glfw.cpp.o" \
"CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_impl_opengl3.cpp.o" \
"CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_tables.cpp.o" \
"CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_widgets.cpp.o" \
"CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui.cpp.o"

# External object files for target TicTacToe
TicTacToe_EXTERNAL_OBJECTS =

TicTacToe: CMakeFiles/TicTacToe.dir/src/main.cpp.o
TicTacToe: CMakeFiles/TicTacToe.dir/src/renderer.cpp.o
TicTacToe: CMakeFiles/TicTacToe.dir/src/vertexArray.cpp.o
TicTacToe: CMakeFiles/TicTacToe.dir/src/indexBuffer.cpp.o
TicTacToe: CMakeFiles/TicTacToe.dir/src/shader.cpp.o
TicTacToe: CMakeFiles/TicTacToe.dir/src/vertexBuffer.cpp.o
TicTacToe: CMakeFiles/TicTacToe.dir/src/vertexBufferLayout.cpp.o
TicTacToe: CMakeFiles/TicTacToe.dir/src/texture.cpp.o
TicTacToe: CMakeFiles/TicTacToe.dir/src/vertexBufferElement.cpp.o
TicTacToe: CMakeFiles/TicTacToe.dir/src/blender.cpp.o
TicTacToe: CMakeFiles/TicTacToe.dir/src/events.cpp.o
TicTacToe: CMakeFiles/TicTacToe.dir/src/window.cpp.o
TicTacToe: CMakeFiles/TicTacToe.dir/src/board.cpp.o
TicTacToe: CMakeFiles/TicTacToe.dir/src/piece.cpp.o
TicTacToe: CMakeFiles/TicTacToe.dir/src/dearImGui.cpp.o
TicTacToe: CMakeFiles/TicTacToe.dir/src/vendor/stb_image/stb_image.cpp.o
TicTacToe: CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_demo.cpp.o
TicTacToe: CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_draw.cpp.o
TicTacToe: CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_impl_glfw.cpp.o
TicTacToe: CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_impl_opengl3.cpp.o
TicTacToe: CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_tables.cpp.o
TicTacToe: CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui_widgets.cpp.o
TicTacToe: CMakeFiles/TicTacToe.dir/src/vendor/imGui/imgui.cpp.o
TicTacToe: CMakeFiles/TicTacToe.dir/build.make
TicTacToe: /usr/local/lib/libglfw.3.3.dylib
TicTacToe: /usr/local/lib/libGLEW.2.2.0.dylib
TicTacToe: CMakeFiles/TicTacToe.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_24) "Linking CXX executable TicTacToe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TicTacToe.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TicTacToe.dir/build: TicTacToe

.PHONY : CMakeFiles/TicTacToe.dir/build

CMakeFiles/TicTacToe.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/TicTacToe.dir/cmake_clean.cmake
.PHONY : CMakeFiles/TicTacToe.dir/clean

CMakeFiles/TicTacToe.dir/depend:
	cd /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/build /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/build /Users/sethmcdonald/dev/McDaStuff/McDaC++/Projects/TicTacToe/build/CMakeFiles/TicTacToe.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/TicTacToe.dir/depend

