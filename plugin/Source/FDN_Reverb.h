
/*
  ==============================================================================

Feedback Delay Network Reverb

Based on Geraint Luft's "Let's write a reverb," talk. This is my own custom implementation of a reverb using his DSP library.
I've also added early refelctions and pre-delay. 
	 
Dependency: it uses Geraint Luft's DSP library https://github.com/Signalsmith-Audio/dsp
This is included in project as my custom library, as I had to change the file windows.h to custom-windows.h to avoid conflict.
Check his license in Geraint-Luff-DSP folder. 


Talk: https://www.youtube.com/watch?v=QWnD30xHjW4
Geraint Luft:  https://signalsmith-audio.co.uk/

	
	


	


  ==============================================================================
*/




#pragma once

#include "delay.h"
#include "mix.h"


#include <cstdlib>

#include <random>
#include <iostream>
#include <type_traits>

// This is a simple delay class which rounds to a whole number of samples.
using Delay = signalsmith::delay::Delay<double, signalsmith::delay::InterpolatorNearest>;




struct randomInRange {
	// Member-based RNG, seeded once and used for all random calls
	static std::mt19937& getRng() {
		static std::mt19937 rng(std::random_device{}());
		return rng;
	}

	static bool bernoulliDistribution() {
		static std::bernoulli_distribution bDist(0.5);
		return bDist(getRng());
	}

	template <typename T>
	static T generateRandomReal(T min, T max) {
		static_assert(std::is_floating_point<T>::value, "Template parameter must be a floating-point type.");
		std::uniform_real_distribution<T> dist(min, max);
		return dist(getRng());  // Use the single RNG instance
	}
};






template<int channels=8>
struct MultiChannelMixedFeedback {
	using Array = std::array<double, channels>;
	double delayMs = 150;
	double decayGain = 0.85;

	std::array<int, channels> delaySamples;
	std::array<Delay, channels> delays;
	
	void configure(double sampleRate) {
		double delaySamplesBase = delayMs*0.001*sampleRate;
		for (int c = 0; c < channels; ++c) {
			double r = c*1.0/channels;
			delaySamples[c] = std::pow(2, r)*delaySamplesBase;
			delays[c].resize(delaySamples[c] + 1);
			delays[c].reset();
		}
	}
	
	Array process(Array input) {
		Array delayed;
		for (int c = 0; c < channels; ++c) {
			delayed[c] = delays[c].read(delaySamples[c]);
		}
		
		
		
		// Mix using a Householder matrix
		signalsmith::mix::Householder<double, channels>::inPlace(delayed.data());  
		
		
		for (int c = 0; c < channels; ++c) {
			double sum = input[c] + delayed[c]*decayGain;
			delays[c].write(sum);
		}
		
		return delayed;
	}
};

template<int channels=8>
struct DiffusionStep {
	using Array = std::array<double, channels>;
	double delayMsRange = 50;
	
	std::array<int, channels> delaySamples;  // read positions
	std::array<Delay, channels> delays;
	std::array<bool, channels> flipPolarity;

	


	// create random delay times (sample time)
	void configure(double sampleRate) {     
		double delaySamplesRange = delayMsRange*0.001*sampleRate;
		
		for (int c = 0; c < channels; ++c) {
			double rangeLow = delaySamplesRange*c/channels;
			double rangeHigh = delaySamplesRange*(c + 1)/channels;
			delaySamples[c] = randomInRange::generateRandomReal<double>(rangeLow, rangeHigh); 
			delays[c].resize(delaySamples[c] + 1);
			delays[c].reset();
			flipPolarity[c] = randomInRange::bernoulliDistribution();  //rand() % 2;  
		}
	}
	
	Array process(Array input) {    // after 2-4 K samples it becomes continues sounding reverbish, and not discret.
		// Delay
		Array delayed;
		for (int c = 0; c < channels; ++c) {
			delays[c].write(input[c]);
			delayed[c] = delays[c].read(delaySamples[c]);
		}
		
	

		// Flip some polarities,  does not shuffle
		for (int c = 0; c < channels; ++c) {
			if (flipPolarity[c]) delayed[c] *= -1;
		}

		// Mix with a Hadamard matrix
		signalsmith::mix::Hadamard<double, channels>::inPlace(delayed.data());

		return delayed;
	} 
};



template<int channels=8, int stepCount=4>
struct DiffuserHalfLengths {
	using Array = std::array<double, channels>;

	std::array<DiffusionStep<channels>, stepCount> steps;

	
	    
	void setDelayMsRange(double diffusionMs) 
	{
		for (auto &step : steps) {
			diffusionMs *= 0.5;
			step.delayMsRange = diffusionMs;
		}
	}
	
	void configure(double sampleRate) {
		for (auto &step : steps) step.configure(sampleRate);
	}
	
	Array process(Array samples) {
		for (auto &step : steps) {
			samples = step.process(samples);
		}
		
		return samples; 
	}
};




template<int channels = 8>
struct EarlyReflections {
	using Array = std::array<double, channels>;

	std::array<Delay, channels> delays;
	std::array<double, channels> gains;
	std::array<int, channels> delaySamples;

	double minDelayMs = 5;
	double maxDelayMs = 30;

	// Configure delay range based on room size
	void configureDelayRange(double minMs, double maxMs, double sampleRate) {
		minDelayMs = minMs;
		maxDelayMs = maxMs;

		configure(sampleRate);
	}

	void configure(double sampleRate) {
		for (int c = 0; c < channels; ++c) {
			double reflectionTimeMs = randomInRange::generateRandomReal<double>(minDelayMs, maxDelayMs);
			delaySamples[c] = reflectionTimeMs * 0.001 * sampleRate;
			delays[c].resize(delaySamples[c] + 1);
			delays[c].reset();

			gains[c] = randomInRange::generateRandomReal<double>(0.2, 0.6);
		}
	}

	Array process(const Array& input) {
		Array earlyReflections;
		for (int c = 0; c < channels; ++c) {
			delays[c].write(input[c]);
			earlyReflections[c] = delays[c].read(delaySamples[c]) * gains[c];
		}

		signalsmith::mix::Hadamard<double, channels>::inPlace(earlyReflections.data());

		return earlyReflections;
	}
};


template<int channels>
struct PreDelay {
	using Array = std::array<double, channels>;

	std::array<Delay, channels> delays;
	std::array<int, channels> delaySamples;
	double preDelayMs = 20;  // Default value for pre-delay

	// Configure each delay line based on sample rate
	void configure(double sampleRate) {
		int delayInSamples = static_cast<int>(preDelayMs * 0.001 * sampleRate);
		for (int c = 0; c < channels; ++c) {
			delaySamples[c] = delayInSamples;
			delays[c].resize(delaySamples[c] + 1);
			delays[c].reset();
		}
	}

	// Set the pre-delay time for all channels
	void setPreDelayMs(double ms, double sampleRate) {
		preDelayMs = ms;
		configure(sampleRate);
	}

	// Process each channel in the array
	Array process(const Array& input) {
		Array delayedOutput;
		for (int c = 0; c < channels; ++c) {
			delays[c].write(input[c]);
			delayedOutput[c] = delays[c].read(delaySamples[c]);
		}
		return delayedOutput;
	}
};


template<int channels=8, int diffusionSteps=4>
struct BasicReverb {
	using Array = std::array<double, channels>;
	
	MultiChannelMixedFeedback<channels> feedback;
	DiffuserHalfLengths<channels, diffusionSteps> diffuser; 
	EarlyReflections<channels> earlyReflections;
	PreDelay<channels> preDelay;  // Multichannel pre-delay

	double dry = 0.5;
	double diffuserGain = 0.3;
	double earlyReflectionGain = 0.0;
	

	double roomSizeMs = 50.0;
	double rt60 = 6.0;
	double sampleRate = 44100.0;

	
	const double scalingFactor = 1.0 / std::sqrt(channels);

	signalsmith::mix::StereoMultiMixer<float, channels> mix;

	BasicReverb() 
	{
		// try differenct values
		diffuser.setDelayMsRange(50);
		updateDecayGain();
	}
	
	void setDry(double dryValue)
	{
		dry = dryValue;
	}

	void setDiffusionGain(double gainValue)
	{
		diffuserGain = gainValue;
	}

	void setEarlyReflections(double wetValue)
	{
		earlyReflectionGain = wetValue;
	}

	void setPreDelay(double timeMs)
	{
		preDelay.setPreDelayMs(timeMs, sampleRate);
	}

	void setRoomSize(double sizeValue)
	{
		roomSizeMs = sizeValue;
		//diffuser.setDelayMsRange(roomSizeMs);
		//diffuser.configure(sampleRate);

		updateDecayGain();

		// Set early reflection delay ranges based on room size
		if (roomSizeMs <= 50) {  // Small room
			earlyReflections.configureDelayRange(5, 15, sampleRate); // Small reflection times
		}
		else if (roomSizeMs <= 100) {  // Medium room
			earlyReflections.configureDelayRange(10, 30, sampleRate);
		}
		else {  // Large room
			earlyReflections.configureDelayRange(20, 50, sampleRate);
		}
	}

	void setDecay(double decayValue)
	{
		rt60 = decayValue;
		updateDecayGain();
	}

	void updateDecayGain()
	{
		feedback.delayMs = roomSizeMs;

		// How long does our signal take to go around the feedback loop?
		double typicalLoopMs = roomSizeMs * 1.5;
		// How many times will it do that during our RT60 period?
		double loopsPerRt60 = rt60 / (typicalLoopMs * 0.001);
		// This tells us how many dB to reduce per loop
		double dbPerCycle = -60 / loopsPerRt60;

		feedback.decayGain = std::pow(10, dbPerCycle * 0.05);   

	}


	void configure(double newSampleRate) 
	{
		feedback.configure(sampleRate);
		diffuser.configure(sampleRate);
		earlyReflections.configure(sampleRate);
		preDelay.configure(sampleRate);
		sampleRate = newSampleRate;
	}
	
	


	// It process by sample. Feed it a buffer writer pointer. Is called from AudioPluginAudioProcessor  processBlock()
	void process(float* ch1, float* ch2, int numSamples) 
	{
		
		// In: store incoming 2 channel input ch1/ch2.
		// Out: is the multichannel output from this reverb process.
		// Out is mixed down to 2 ch In, and then used to overwrite ch1/ch2 
		std::array<double, channels> out = {};
		std::array<float, 2> in = {};
		
		
		for (int i = 0; i < numSamples; i++)
		{
			in[0] = ch1[i];
			in[1] = ch2[i];

			mix.stereoToMulti(in, out);

			// Early reflections
			Array earlyReflection = earlyReflections.process(out);

			// Apply pre-delay to the early reflection output
			earlyReflection = preDelay.process(earlyReflection);

			Array diffuse = diffuser.process(earlyReflection);     
			Array longLasting = feedback.process(diffuse);
			

			
		
			

			for (int c = 0; c < channels; ++c) 
			{													
				//out[c] = dry * out[c] * scalingFactor + diffuserGain * longLasting[c] * scalingFactor + earlyReflection[c] * earlyReflectionGain * scalingFactor; 
				out[c] = (dry * out[c] + diffuserGain * longLasting[c] + earlyReflection[c] * earlyReflectionGain) * scalingFactor;
			}

			

			mix.multiToStereo(out, in);

			ch1[i] = in[0];
			ch2[i] = in[1];
		}
	}

		


};