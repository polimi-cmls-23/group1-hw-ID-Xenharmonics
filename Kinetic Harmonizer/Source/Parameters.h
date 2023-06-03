#pragma once

#include <JuceHeader.h>
#include <string> 

// Costants
#define SMOOTHING_TIME_CH 0.02f // Channel Strip
#define LEVEL_SMOOTHING_TIME 0.02f // Dry/Wet

#define MAX_DELAY_TIME 5.00f
#define TIME_SMOOTHING 0.02f
#define FBK_SMOOTHING  0.02f

#define GLIDE_TIME 0.01
#define SMOOTHING_TIME 0.04

#define SMOOTHING_TIME_ST 0.04

static const int NUM_VOICE = 3;

// Parameters (I NOMI DEVONO DIVENTARE static const String

static const String NAME_MUTE   = "muteOriginal";
static const String NAME_LEVEL  = "levelOriginal";
static const String NAME_PAN    = "panOriginal";
static const String NAME_DW = "dw";

static const String NAME_ST = "semitones";
static const String NAME_CENT = "cent";
static const String NAME_DT = "delayTime";
static const String NAME_FB = "feedback";

static const String NAME_MUTE1 = "mute1";
static const String NAME_LEVEL1 = "level1";
static const String NAME_PAN1 = "pan1";
static const String NAME_ST1 = "semitones1";
static const String NAME_DT1 = "delayTime1";
static const String NAME_FB1 = "feedback1";



static const String NAME_FREQ   = "lfoFreq";
static const String NAME_MOD    = "timeMod";
static const String NAME_WF     = "waveform";


#define DEFAULT_FILTER 1000.0f
#define DEFAULT_QFILTER 0.707f
#define DEFAULT_MUTE   0
#define DEFAULT_LEVEL  0.0f
#define DEFAULT_PAN    50.0f
#define DEFAULT_DW     1.0f
#define DEFAULT_DT	   0.0f
#define DEFAULT_FB     0.0f
#define DEFAULT_FREQ   7.0f
#define DEFAULT_MOD    1.0f
#define DEFAULT_WF     0
#define DEFAULT_ST     0.0f


namespace Parameters
{
	static AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
	{
		std::vector<std::unique_ptr<RangedAudioParameter>> params;

		params.push_back(std::make_unique<AudioParameterBool>(NAME_MUTE, "Mute", DEFAULT_MUTE)); 
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_LEVEL, "Gain", -48.0f, 12.0f, DEFAULT_LEVEL));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_PAN, "Pan", NormalisableRange<float>(0.0f, 100.0f, 1.0f, 1.0f), DEFAULT_PAN));


		params.push_back(std::make_unique<AudioParameterBool>(NAME_MUTE1, "Mute Voice1", DEFAULT_MUTE));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_LEVEL1, "Gain Voice1", -48.0f, 12.0f, DEFAULT_LEVEL));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_PAN1, "Pan Voice1", NormalisableRange<float>(0.0f, 100.0f, 1.0f, 1.0f), DEFAULT_PAN));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_ST1, "Semitones Voice1", -12.0f, 12.0f, DEFAULT_ST));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_DT1, "Delay time (s) Voice1", NormalisableRange<float>(0.0f, MAX_DELAY_TIME, 0.001f, 0.3f), DEFAULT_DT));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_FB1, "Feedback Voice1", 0.0f, 1.0f, DEFAULT_FB));


		params.push_back(std::make_unique<AudioParameterFloat>(NAME_DW, "Dry/Wet", 0.0f, 1.0f, DEFAULT_DW));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_FREQ, "LFO Freq (Hz)", NormalisableRange<float>(0.1f, 20.0f, 0.01f, 0.3f), DEFAULT_FREQ));
		

		return { params.begin(), params.end() };
	}
}
