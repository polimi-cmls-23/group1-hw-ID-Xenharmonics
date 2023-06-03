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

	void processBlock(AudioBuffer<float>& buffer, int inCh, int outCh)
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

	void moltiplicator(AudioBuffer<float>& buffer, AudioBuffer<float>& phasor, const int numSamples)
	{
		const int numCh = buffer.getNumChannels();
		auto bufferData = buffer.getArrayOfWritePointers();
		auto phasorData = phasor.getArrayOfReadPointers();
		const float pi = MathConstants<float>::pi;

		for (int smp = 0; smp < numSamples; ++smp)
		{
			for (int ch = 0; ch < numCh; ++ch)
			{
				bufferData[ch][smp] *= (-0.5f * cos(2.0f * pi * phasorData[ch][smp]) + 0.5f);
			}
		}
	}

	// END WIND FUNCTION

	void setMute(float newValue)
	{
		active = newValue < 0.5f;
		updateState();
	}

	void setLevel(float newValue)
	{
		gain = Decibels::decibelsToGain(newValue);
		updateState();
	}

	void setPan(float newValue)
	{
		pan = newValue / 100.0f;
		updateState();
	}


private:

	void updateState()
	{
		auto l = active * gain;
		monoGain.setTargetValue(l);
		leftGain.setTargetValue(l * sqrt(1.0f - pan));
		rightGain.setTargetValue(l * sqrt(pan));
	}

	bool active = 1 - DEFAULT_MUTE;
	float gain = Decibels::decibelsToGain(DEFAULT_LEVEL);
	float pan = DEFAULT_PAN;


	SmoothedValue<float, ValueSmoothingTypes::Linear> leftGain;
	SmoothedValue<float, ValueSmoothingTypes::Linear> rightGain;
	SmoothedValue<float, ValueSmoothingTypes::Linear> monoGain;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChannelStrip)
};