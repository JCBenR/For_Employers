# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.18.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.18.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/justinsiebenhaar/justinSiebenhaar/CoinGame

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/justinsiebenhaar/justinSiebenhaar/CoinGame/build

# Include any dependencies generated for this target.
include CMakeFiles/CoinGame.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/CoinGame.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CoinGame.dir/flags.make

CMakeFiles/CoinGame.dir/src/main.cpp.o: CMakeFiles/CoinGame.dir/flags.make
CMakeFiles/CoinGame.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/justinsiebenhaar/justinSiebenhaar/CoinGame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/CoinGame.dir/src/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CoinGame.dir/src/main.cpp.o -c /Users/justinsiebenhaar/justinSiebenhaar/CoinGame/src/main.cpp

CMakeFiles/CoinGame.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CoinGame.dir/src/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/justinsiebenhaar/justinSiebenhaar/CoinGame/src/main.cpp > CMakeFiles/CoinGame.dir/src/main.cpp.i

CMakeFiles/CoinGame.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CoinGame.dir/src/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/justinsiebenhaar/justinSiebenhaar/CoinGame/src/main.cpp -o CMakeFiles/CoinGame.dir/src/main.cpp.s

CMakeFiles/CoinGame.dir/src/Character.cpp.o: CMakeFiles/CoinGame.dir/flags.make
CMakeFiles/CoinGame.dir/src/Character.cpp.o: ../src/Character.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/justinsiebenhaar/justinSiebenhaar/CoinGame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/CoinGame.dir/src/Character.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CoinGame.dir/src/Character.cpp.o -c /Users/justinsiebenhaar/justinSiebenhaar/CoinGame/src/Character.cpp

CMakeFiles/CoinGame.dir/src/Character.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CoinGame.dir/src/Character.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/justinsiebenhaar/justinSiebenhaar/CoinGame/src/Character.cpp > CMakeFiles/CoinGame.dir/src/Character.cpp.i

CMakeFiles/CoinGame.dir/src/Character.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CoinGame.dir/src/Character.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/justinsiebenhaar/justinSiebenhaar/CoinGame/src/Character.cpp -o CMakeFiles/CoinGame.dir/src/Character.cpp.s

# Object files for target CoinGame
CoinGame_OBJECTS = \
"CMakeFiles/CoinGame.dir/src/main.cpp.o" \
"CMakeFiles/CoinGame.dir/src/Character.cpp.o"

# External object files for target CoinGame
CoinGame_EXTERNAL_OBJECTS =

CoinGame: CMakeFiles/CoinGame.dir/src/main.cpp.o
CoinGame: CMakeFiles/CoinGame.dir/src/Character.cpp.o
CoinGame: CMakeFiles/CoinGame.dir/build.make
CoinGame: /usr/local/lib/libsfml-graphics.2.5.1.dylib
CoinGame: /usr/local/lib/libsfml-window.2.5.1.dylib
CoinGame: /usr/local/lib/libsfml-system.2.5.1.dylib
CoinGame: CMakeFiles/CoinGame.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/justinsiebenhaar/justinSiebenhaar/CoinGame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable CoinGame"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CoinGame.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/CoinGame.dir/build: CoinGame

.PHONY : CMakeFiles/CoinGame.dir/build

CMakeFiles/CoinGame.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/CoinGame.dir/cmake_clean.cmake
.PHONY : CMakeFiles/CoinGame.dir/clean

CMakeFiles/CoinGame.dir/depend:
	cd /Users/justinsiebenhaar/justinSiebenhaar/CoinGame/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/justinsiebenhaar/justinSiebenhaar/CoinGame /Users/justinsiebenhaar/justinSiebenhaar/CoinGame /Users/justinsiebenhaar/justinSiebenhaar/CoinGame/build /Users/justinsiebenhaar/justinSiebenhaar/CoinGame/build /Users/justinsiebenhaar/justinSiebenhaar/CoinGame/build/CMakeFiles/CoinGame.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/CoinGame.dir/depend

