#pragma once
#include <JuceHeader.h>
#include "Filter.h"
#include "ChannelStrip.h"
#include "Delay.h"
#include "Merger.h"
#include "Oscillator.h"
#include "Parameters.h"

class PitchShift
{
public:
	PitchShift() {};
	~PitchShift() {};

	void pitchSetParameters()
	{
		filter.setFrequency(DEFAULT_FILTER);
		filter.setQuality(DEFAULT_QFILTER);

		merger.setSemitones(DEFAULT_ST);

		delay.setFeedback(DEFAULT_FB);

		LFO.setFrequency(DEFAULT_FREQ);
		LFO.setWaveform(DEFAULT_WF);
		timeAdapter.setShift(1.0f);
		timeAdapter.setParameter(DEFAULT_DT);
	}

	void prepareToPlay(double sr, int samplesPerBlock)
	{
		filter.prepareToPlay(sr);
		LFO.prepareToPlay(sr);
	    copyDualMono.setSize(2, samplesPerBlock);
		copyDualMono.clear();
		modulationSignal.setSize(2, samplesPerBlock);
		modulationSignal.clear();
		phasor.setSize(2, samplesPerBlock);
		phasor.clear();
		delay.prepareToPlay(sr, samplesPerBlock);

		channelStripVoice.prepareToPlay(sr);
		timeAdapter.prepareToPlay(sr);
	}

	void releaseResources() {

		delay.releaseResurces();
		copyDualMono.setSize(0, 0);
		modulationSignal.setSize(0, 0);
		phasor.setSize(0, 0);
	}


	void setFeedback(float newValue)
	{
		delay.setFeedback(newValue);
	}

	void setDelay(float newValue)
	{
		timeAdapter.setParameter(newValue);
	}

	void setSemitones(float newValue, int sr)
	{
		merger.setSemitones(newValue);
		timeAdapter.setShift(merger.getShift());
		float maxFreq = sr * 0.49999;
		float ffreq = (merger.getShift() <= 1.0 ? maxFreq : maxFreq / merger.getShift());
		filter.setFrequency(ffreq);
	}



	void processBlock(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& dualmono, int numSamples, int inCh, int outCh)
	{
		auto shift = merger.getShift();

		copyDualMono.copyFrom(0, 0, dualmono, 0, 0, numSamples);
		copyDualMono.copyFrom(1, 0, dualmono, 1, 0, numSamples);

		filter.processBlock(copyDualMono, numSamples);
		LFO.setWaveform(shift);

		// Generare una modulante
		LFO.getNextAudioBlock(modulationSignal, numSamples);

		phasor.copyFrom(0, 0, modulationSignal, 0, 0, numSamples);
		phasor.copyFrom(1, 0, modulationSignal, 1, 0, numSamples);

		// Scalare modulante
		timeAdapter.processBlock(modulationSignal, numSamples);
		
		// Applicare delay
		delay.processBlock(copyDualMono, modulationSignal);

		channelStripVoice.moltiplicator(copyDualMono, phasor, numSamples);

		copyDualMono.addFrom(0, 0, copyDualMono, 1, 0, numSamples);
		copyDualMono.copyFrom(1, 0, copyDualMono, 0, 0, numSamples);
		copyDualMono.applyGain(0.5f);

		channelStripVoice.processBlock(copyDualMono, inCh, outCh);
		
		// Somma sul buffer di output
		buffer.addFrom(0, 0, copyDualMono, 0, 0, numSamples);

		if (inCh == 2) {   //trasformo in stereo
			buffer.addFrom(1, 0, copyDualMono, 1, 0, numSamples);
		}
		
	}
	
	juce::AudioBuffer<float> copyDualMono;



private:

	Filter filter;
	Merger merger;
	ChannelStrip channelStripVoice;
	ModDelay delay;
	NaiveOscillator LFO;
	ParameterModulation timeAdapter;
	juce::AudioBuffer<float> modulationSignal;
	juce::AudioBuffer<float> phasor;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PitchShift)
};

