# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.26

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\mingw64\bin\cmake.exe

# The command to remove a file.
RM = C:\mingw64\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "D:\ispring\study\second_course\project practice\Pokemon"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "D:\ispring\study\second_course\project practice\Pokemon\build"

# Include any dependencies generated for this target.
include CMakeFiles/Pokemon.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Pokemon.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Pokemon.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Pokemon.dir/flags.make

CMakeFiles/Pokemon.dir/main.cpp.obj: CMakeFiles/Pokemon.dir/flags.make
CMakeFiles/Pokemon.dir/main.cpp.obj: CMakeFiles/Pokemon.dir/includes_CXX.rsp
CMakeFiles/Pokemon.dir/main.cpp.obj: D:/ispring/study/second_course/project\ practice/Pokemon/main.cpp
CMakeFiles/Pokemon.dir/main.cpp.obj: CMakeFiles/Pokemon.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="D:\ispring\study\second_course\project practice\Pokemon\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Pokemon.dir/main.cpp.obj"
	C:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Pokemon.dir/main.cpp.obj -MF CMakeFiles\Pokemon.dir\main.cpp.obj.d -o CMakeFiles\Pokemon.dir\main.cpp.obj -c "D:\ispring\study\second_course\project practice\Pokemon\main.cpp"

CMakeFiles/Pokemon.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Pokemon.dir/main.cpp.i"
	C:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "D:\ispring\study\second_course\project practice\Pokemon\main.cpp" > CMakeFiles\Pokemon.dir\main.cpp.i

CMakeFiles/Pokemon.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Pokemon.dir/main.cpp.s"
	C:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "D:\ispring\study\second_course\project practice\Pokemon\main.cpp" -o CMakeFiles\Pokemon.dir\main.cpp.s

# Object files for target Pokemon
Pokemon_OBJECTS = \
"CMakeFiles/Pokemon.dir/main.cpp.obj"

# External object files for target Pokemon
Pokemon_EXTERNAL_OBJECTS =

Pokemon.exe: CMakeFiles/Pokemon.dir/main.cpp.obj
Pokemon.exe: CMakeFiles/Pokemon.dir/build.make
Pokemon.exe: C:/SFML-2.6.1/lib/libsfml-system-s-d.a
Pokemon.exe: C:/SFML-2.6.1/lib/libsfml-window-s-d.a
Pokemon.exe: C:/SFML-2.6.1/lib/libsfml-graphics-s-d.a
Pokemon.exe: C:/SFML-2.6.1/lib/libsfml-audio-s-d.a
Pokemon.exe: C:/SFML-2.6.1/lib/libsfml-window-s-d.a
Pokemon.exe: C:/SFML-2.6.1/lib/libfreetype.a
Pokemon.exe: C:/SFML-2.6.1/lib/libsfml-system-s-d.a
Pokemon.exe: C:/SFML-2.6.1/lib/libopenal32.a
Pokemon.exe: C:/SFML-2.6.1/lib/libvorbisfile.a
Pokemon.exe: C:/SFML-2.6.1/lib/libvorbisenc.a
Pokemon.exe: C:/SFML-2.6.1/lib/libvorbis.a
Pokemon.exe: C:/SFML-2.6.1/lib/libogg.a
Pokemon.exe: C:/SFML-2.6.1/lib/libFLAC.a
Pokemon.exe: CMakeFiles/Pokemon.dir/linkLibs.rsp
Pokemon.exe: CMakeFiles/Pokemon.dir/objects1.rsp
Pokemon.exe: CMakeFiles/Pokemon.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="D:\ispring\study\second_course\project practice\Pokemon\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Pokemon.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Pokemon.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Pokemon.dir/build: Pokemon.exe
.PHONY : CMakeFiles/Pokemon.dir/build

CMakeFiles/Pokemon.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Pokemon.dir\cmake_clean.cmake
.PHONY : CMakeFiles/Pokemon.dir/clean

CMakeFiles/Pokemon.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "D:\ispring\study\second_course\project practice\Pokemon" "D:\ispring\study\second_course\project practice\Pokemon" "D:\ispring\study\second_course\project practice\Pokemon\build" "D:\ispring\study\second_course\project practice\Pokemon\build" "D:\ispring\study\second_course\project practice\Pokemon\build\CMakeFiles\Pokemon.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/Pokemon.dir/depend

