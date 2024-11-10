
A feedback delay network reverb based on Geraint Luft's "Let's write a reverb," talk.
	 
	Dependency: it uses Geraint Luft's DSP library https://github.com/Signalsmith-Audio/dsp
	This is included in project as my custom library, as I had to change the file windows.h to custom-windows.h to avoid conflict.

	It use JUCE https://juce.com/ Please check license if you are using this. 

	Talk: https://www.youtube.com/watch?v=QWnD30xHjW4
	Geraint Luft:  https://signalsmith-audio.co.uk/

Creates an audio plugin (e.g., a VST3 plugin) with JUCE, C++ using CMake. 





## Usage

After cloning it locally, you can proceed with the usual CMake workflow.

In the main repo directory execute

```bash
$ cmake -S . -B build
$ cmake --build build
```

The first run will take the most time because the dependencies (CPM, JUCE, and googletest) need to be downloaded.

On Mac/Xcode you must first run config from terminal, creating a .xcodeproj file you can open in xcode(cmake -S . -B build -G Xcode).
In visual studio and visual studio code you can do this within editor IDE, using build in terminal.



