CMake JUCE Audio Plugin Template


Creates an audio plugin (e.g., a VST3 plugin) with JUCE, C++ using CMake. 


* effortless handling of third-party dependencies with the CPM package manager; use the C++ libraries you want together with JUCE
* ready-to-go unit test project with GoogleTest


## Usage

After cloning it locally, you can proceed with the usual CMake workflow.

In the main repo directory execute

```bash
$ cmake -S . -B build
$ cmake --build build
```

The first run will take the most time because the dependencies (CPM, JUCE, and googletest) need to be downloaded.

You can use bundled CMake presets:

```bash
$ cmake --preset default       # uses Visual Studio 17 2022, but its better to use the Ninja build system for build speed. 
$ cmake --build build
$ ctest --preset default
```

Existing presets are `default`, `release`, and `Xcode`.

To run clang-format on every commit, in the main directory execute

```bash
pre-commit install
```

(for this you may need to install `pre-commit` with `pip`: `pip install pre-commit`).


Thanks to Jan Wilczek from https://thewolfsound.com/ (video tutorial) for getting me started with CMake. Also thanks to Sudara at https://melatonin.dev/blog/how-to-use-cmake-with-juce/ 
Other learning resources are: juce cmake doc https://github.com/juce-framework/JUCE/blob/master/docs/CMake%20API.md  and of course CMake documentation at cmake.org.
