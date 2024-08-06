#pragma once

#include "../Source/PluginProcessor.h"
#include <gtest/gtest.h>

using namespace juce;

	TEST(PluginTestSuite, PluginProcessorTest)
	{	

		AudioPluginAudioProcessor processor;

		std::string expected = "MyProduct"; // To pass, change foo to time.
		String actual = processor.getName(); // processor.getName();
		EXPECT_EQ(expected, actual); // should  fail


		//ASSERT_FALSE(true);
	}




