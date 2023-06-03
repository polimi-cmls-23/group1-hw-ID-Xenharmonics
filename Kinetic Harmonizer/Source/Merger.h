#pragma once
#include <JuceHeader.h>
#include "Parameters.h"

class Merger
{
public:
	Merger() {};
	~Merger() {};

	void prepareToPlay(double sr)
	{
		n.reset(sr, SMOOTHING_TIME_ST);
		shift.reset(sr, SMOOTHING_TIME);
	}

	void setSemitones(float newValue)
	{
		semitones = newValue;
		merge();
	}


	float getShift()
	{
		return shift.getCurrentValue();
	}

	
private:

	void merge()
	{
		auto sum = semitones;
		n.setTargetValue(sum);
		shift.setTargetValue(pow(2, sum/12));
	}

	float semitones = DEFAULT_ST;
	
	SmoothedValue<float, ValueSmoothingTypes::Linear> n;
	SmoothedValue<float, ValueSmoothingTypes::Linear> shift;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Merger)
};
