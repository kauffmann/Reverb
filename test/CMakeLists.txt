cmake_minimum_required(VERSION 3.22)

project(AudioPluginTest VERSION 0.1.1)

# Signals to CMake that we want to run tests from this directory.
enable_testing()



# Creates the test console application. Can't use add_executable, does not work with juceheader.h
JUCE_ADD_CONSOLE_APP(${PROJECT_NAME})

juce_generate_juce_header(${PROJECT_NAME})


target_sources(${PROJECT_NAME} 
PRIVATE
    source/AudioProcessorTest.cpp
)





# Thanks to the fact that we link against the gtest_main library, we don't have to write the main function ourselves.
# Adding target "AudioPlugin" gives me access to all relevant in plugin folder, where source files, JUCE and juceheader is located.
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        AudioPlugin
        GTest::gtest_main

        
)






# Enables all warnings and treats warnings as errors.
# This needs to be set up only for your projects, not 3rd party
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /Wall)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Adds googletest-specific CMake commands at our disposal.
include(GoogleTest)
# Add all tests defined with googletest to the CMake metadata so that these tests are run upon a call to ctest in the test projects' binary directory.
if (CMAKE_GENERATOR STREQUAL Xcode)
# On macOS arm64, all binaries have to be signed before running. In local development, the linker adds an ad-hoc placeholder signature. In Xcode however, the ad-hoc signature is delayed until after the “Run Script” build phase, so the POST_BUILD command added by gtest_discover_tests cannot run. Thus, we need to delay test discovery until run time.
# Source: https://discourse.cmake.org/t/googletest-crash-when-using-cmake-xcode-arm64/5766/8
  gtest_discover_tests(${PROJECT_NAME} DISCOVERY_MODE PRE_TEST)
else()
  gtest_discover_tests(${PROJECT_NAME})
endif()
