#pragma once
#include <JuceHeader.h>
#include "Parameters.h"


class ChannelStrip 
{
public:
	ChannelStrip() {};
	~ChannelStrip() {};

	void prepareToPlay(double sampleRate)
	{
		leftGain.reset(sampleRate, SMOOTHING_TIME_CH);
		rightGain.reset(sampleRate, SMOOTHING_TIME_CH);
		monoGain.reset(sampleRate, SMOOTHING_TIME_CH);

		updateState();
	}

	void processBlock(juce::AudioBuffer<float>& buffer, int inCh, int outCh)
	{
		auto numSamples = buffer.getNumSamples();

    	// Mono In - Mono Out
		if ((inCh + outCh) == 2)
		{
			monoGain.applyGain(buffer, numSamples);
		}
		else // Stereo In - Stereo Out || Mono In - Stero Out || Stereo In - Mono Out
		{
			leftGain.applyGain(buffer.getWritePointer(0), numSamples);
			rightGain.applyGain(buffer.getWritePointer(1), numSamples);;
		}

		// Stereo In - Mono Out
		if (inCh > outCh)
			buffer.addFrom(0, 0, buffer, 1, 0, numSamples);
	}

    // START WIND FUNCTION

	void moltiplicator(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& phasor, const int numSamples)
	{
		const int numCh = buffer.getNumChannels();
		auto bufferData = buffer.getArrayOfWritePointers();
		auto phasorData = phasor.getArrayOfReadPointers();
		const float pi = juce::MathConstants<float>::pi;

		for (int smp = 0; smp < numSamples; ++smp)
		{
			for (int ch = 0; ch < numCh; ++ch)
			{
				bufferData[ch][smp] *= (-0.5f * cos(2.0f * pi * phasorData[ch][smp]) + 0.5f);
			}
		}
	}

	// END WIND FUNCTION


private:

	void updateState()
	{
		auto l = active * gain;
		monoGain.setTargetValue(l);
		leftGain.setTargetValue(l * sqrt(0.5f));
		rightGain.setTargetValue(l * sqrt(0.5f));
	}

	bool active = 1 - DEFAULT_MUTE;
	float gain = juce::Decibels::decibelsToGain(DEFAULT_LEVEL);
	float pan = DEFAULT_PAN;


	juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> leftGain;
	juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> rightGain;
	juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> monoGain;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChannelStrip)
};