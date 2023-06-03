#pragma once
#include <JuceHeader.h>
#include "Parameters.h"

class DelayBase
{
public:
	DelayBase() {};
	~DelayBase() {};

	void prepareToPlay(double sampleRate, int maxNumSamples)
	{
		sr = sampleRate;
		memorySize = roundToInt(MAX_DELAY_TIME * sampleRate) + maxNumSamples;
		delayMemory.setSize(2, memorySize);
		initialize();
	}

	void releaseResurces()
	{
		delayMemory.setSize(0, 0);
		memorySize = 0;
	}

	void processBlock(AudioBuffer<float>& buffer)
	{
		store(buffer);
		moveDelayedTo(buffer);
		updateWriteHead(buffer.getNumSamples());
	}

protected:

	virtual void initialize() = 0;

	void store(const AudioBuffer<float>& buffer)
	{
		const auto numInputSamples = buffer.getNumSamples();
		const auto numChannels = buffer.getNumChannels();

		for (int ch = 0; ch < numChannels; ++ch)
		{
			if (writeIndex + numInputSamples <= memorySize)
				delayMemory.copyFrom(ch, writeIndex, buffer, ch, 0, numInputSamples);
			else
			{
				const int fittingSamples = memorySize - writeIndex;
				const int remainingSamples = numInputSamples - fittingSamples;

				delayMemory.copyFrom(ch, writeIndex, buffer, ch, 0, fittingSamples);
				delayMemory.copyFrom(ch, 0, buffer, ch, fittingSamples, remainingSamples);
			}
		}
	}

	virtual void moveDelayedTo(AudioBuffer<float>& buffer) = 0;

	void updateWriteHead(int leap)
	{
		writeIndex += leap;
		writeIndex %= memorySize;
	}

	AudioBuffer<float> delayMemory;
	int memorySize = 0;
	int writeIndex = 0;
	double sr = 1;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayBase)
};

class ModDelay : public DelayBase
{
public:
	ModDelay() {}
	~ModDelay() {}

	void setFeedback(float newValue)
	{
		feedback.setTargetValue(newValue);
	}

	void processBlock(AudioBuffer<float>& buffer, AudioBuffer<float>& delayTimeBuffer)
	{
		store(buffer);
		moveDelayedTo(buffer, delayTimeBuffer);
		updateWriteHead(buffer.getNumSamples());
	}

private:

	void moveDelayedTo(AudioBuffer<float>& buffer) override
	{
		// If you reach this assertion you're trying to use the wrong overload
		jassertfalse;
	}

	void moveDelayedTo(AudioBuffer<float>& buffer, AudioBuffer<float>& delayTimeBuffer)
	{
		const auto numOutputSamples = buffer.getNumSamples();
		const auto numChannels = buffer.getNumChannels();
		const auto numModChannels = delayTimeBuffer.getNumChannels();

		auto* outputData = buffer.getArrayOfWritePointers();
		auto* delayData = delayMemory.getArrayOfWritePointers();
		auto* delayTimeArray = delayTimeBuffer.getArrayOfReadPointers();

		for (int smp = 0; smp < numOutputSamples; ++smp)
		{
			auto fb = feedback.getNextValue();

			for (int ch = 0; ch < numChannels; ++ch)
			{
				auto dt = delayTimeArray[jmin(ch, numModChannels - 1)][smp];

				auto actualWriteIndex = (writeIndex + smp) % memorySize;
				auto actualReadIndex = memorySize + actualWriteIndex - (dt * sr);

				auto integerPart = (int)actualReadIndex;
				auto fractionalPart = actualReadIndex - integerPart;

				auto A = (integerPart + memorySize) % memorySize;
				auto B = (A + 1) % memorySize;

				const auto alpha = fractionalPart / (2.0f - fractionalPart);

				auto sampleValue = alpha * (delayData[ch][B] - oldSample[ch]) + delayData[ch][A];
				oldSample[ch] = sampleValue;

				// Scrivo sul buffer di output
				outputData[ch][smp] = sampleValue;

				// Scrivo sul delay buffer il feedback
				delayData[ch][actualWriteIndex] += sampleValue * fb;
			}
		}
	}

	void initialize() override
	{
		feedback.reset(sr, FBK_SMOOTHING);
	}

	float oldSample[2] = { 0.0f, 0.0f };

	SmoothedValue<float, ValueSmoothingTypes::Linear> feedback;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModDelay)
};