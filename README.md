CMake JUCE Audio Plugin Template


Creates an audio plugin (e.g., a VST3 plugin) with JUCE, C++ using CMake. 


* Handling of third-party dependencies with the CPM package manager, downloading JUCE and GoogleTest
* Add unit test project with GoogleTest
* Add assets to project (replace current content with your own)
* Add include/source files to project (replace current content with your own)


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



To run clang-format on every commit, in the main directory execute

```bash
pre-commit install
```

(for this you may need to install `pre-commit` with `pip`: `pip install pre-commit`).


Thanks to Jan Wilczek from https://thewolfsound.com/ (video tutorial) for getting me started with CMake. Also thanks to Sudara at https://melatonin.dev/blog/how-to-use-cmake-with-juce/ 
Other learning resources are: juce Cmake API doc https://github.com/juce-framework/JUCE/blob/master/docs/CMake%20API.md  and of course CMake documentation at cmake.org.
