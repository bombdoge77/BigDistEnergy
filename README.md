# Big Dist Energy
This is a distortion VST plugin made using the JUCE C++ framework.

# Build instructions
To build, open 'Big Dist Energy.jucer' using Projucer (avilable at https://github.com/juce-framework/JUCE), select preferred build tool (Makefile, XCode, Visual Studio) and save.

NOTE: I have currently only built the project with Visual Studio, so I cannot guarantee that the Makefile will work.

## Building with Visual Studio
When building with Visual Studio, open the solution and select 'Build' -> 'Build Solution' in the top menu.

## Building with Makefile
$ cd Builds/LinuxMakefile
$ make

## Output
Building will generate a VST3 file and a standalone executable which can be found in the 'Builds' directory.