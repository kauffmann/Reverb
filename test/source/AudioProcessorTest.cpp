#pragma once

#include "../Source/PluginProcessor.h"
#include <gtest/gtest.h>

using namespace juce;


	TEST(PluginTestSuite, PluginProcessorTest)
	{	

		AudioPluginAudioProcessor processor;

		std::string expected = "MyProduct"; 
		String actual = processor.getName(); // return juce string 
		EXPECT_EQ(expected, actual);

		EXPECT_EQ(true, processor.acceptsMidi());
		
		//ASSERT_FALSE(true);
	}




