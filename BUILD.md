# HOW TO BUILD CTF (April 1, 2026 04/01/2026)

Welcome! This Markdown file teaches you on how to build Cereon Template Framework
using it's preferred build system! (CMake)

## Prerequisites

Here are the prerequisites that you need before building Cereon Template Framework:
1. CMake >= version 3.16
2. Visual Studio (>= 2022/17) with Desktop development with C++ (for Windows)
3. Clang (>= 18) (for Windows and Linux)
4. GCC (>= 15) (for Linux)

## Preparation

Once you have installed the following requirements listed above, we will fire up the terminal.

If your a first time programmer, make sure to follow the following requirements below:

[ * ] I promise that I will not be amazed that I can open a terminal.

[ * ] I promise that I will not say "I'm a cool hacker man" after opening the terminal.

[ * ] I promise that I will not complain when the tutorial is missing some stuff that I needed to do but didn't know how.

Anyways, once you have launched your terminal, we will type some commands to generate projects
and build them through CMake. You can use your favorite IDE if it has native or third-party support
for CMake projects like the Cereon Template Framework library. But in this tutorial, we will use the
command-line interface.

## Build Steps

### 1. Generate projects

Time to generate our projects! To generate them, you can type in `cmake -B build -S .` into your terminal.

On Linux, you can configure it so that it builds the output in the Release configuration by typing `cmake -B build -S. -DCMAKE_BUILD_TYPE=Release` in the terminal.

### 2. Build Build Build

To start building the project, there are ways on building the project but in this part, I'll do it my way.

On Windows:
You can simply build the project by typing this: "cmake --build build"
If you want to build the project in Release, type this: "cmake --build build --config Release"

On Linux:
You can simply build the project by typing this: "cmake --build build --parallel"

### 3. Getting the output

The project is configured to place the project's output in where the project build files are placed.

In this case, we made it so that project build files are placed on a folder called "build".

You can get the build output on "build/Bin". If your on Windows, make sure to get the "build/Lib" folder as well. Make sure to copy the header (include) files in "Source/CTF/ctf" as well and KEEP the directory structure.

