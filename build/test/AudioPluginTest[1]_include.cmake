if(EXISTS "C:/Users/kauff/Documents/Audio Programming/Cmake JUCE Plugin Template/build/test/AudioPluginTest[1]_tests.cmake")
  include("C:/Users/kauff/Documents/Audio Programming/Cmake JUCE Plugin Template/build/test/AudioPluginTest[1]_tests.cmake")
else()
  add_test(AudioPluginTest_NOT_BUILT AudioPluginTest_NOT_BUILT)
endif()
