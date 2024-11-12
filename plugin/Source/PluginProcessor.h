#pragma once

#include <JuceHeader.h>
#include "FDN_Reverb.h"
#include "mix.h"

//#include <juce_audio_processors/juce_audio_processors.h>

class AudioPluginAudioProcessor : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener {
public:
	AudioPluginAudioProcessor();
	~AudioPluginAudioProcessor() override;

	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

	void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
	using AudioProcessor::processBlock;

	juce::AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;

	const juce::String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;

	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const juce::String getProgramName(int index) override;
	void changeProgramName(int index, const juce::String& newName) override;

	inline juce::String getParamID(juce::AudioProcessorParameter* param)
	{
		if (auto paramWithID = dynamic_cast<juce::AudioProcessorParameterWithID*>(param))
			return paramWithID->paramID;

		return param->getName(50);
	}

	void getStateInformation(juce::MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

	

private:
	
 		 //  <channels,diffusion steps>	
	BasicReverb<8, 4> reverb = BasicReverb<8, 4>();

	// Parameters
	juce::AudioProcessorValueTreeState apvts;


	// Add a function to help with setting parameters
	juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

	// Listener callback when parameters change
	void parameterChanged(const juce::String& parameterID, float newValue) override;

	
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessor)
};
