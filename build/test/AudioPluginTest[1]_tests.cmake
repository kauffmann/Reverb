add_test([=[PluginTestSuite.PluginProcessorTest]=]  [==[C:/Users/kauff/Documents/Audio Programming/Cmake JUCE Plugin Template/build/test/AudioPluginTest_artefacts/Debug/AudioPluginTest.exe]==] [==[--gtest_filter=PluginTestSuite.PluginProcessorTest]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[PluginTestSuite.PluginProcessorTest]=]  PROPERTIES WORKING_DIRECTORY [==[C:/Users/kauff/Documents/Audio Programming/Cmake JUCE Plugin Template/build/test]==] SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  AudioPluginTest_TESTS PluginTestSuite.PluginProcessorTest)
