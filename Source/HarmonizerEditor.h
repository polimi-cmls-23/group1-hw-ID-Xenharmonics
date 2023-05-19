/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.1.6

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Meter.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
//typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class HarmonizerEditor  : public juce::AudioProcessorEditor
{
public:
    //==============================================================================
    HarmonizerEditor (HarmonizerAudioProcessor& p, juce::AudioProcessorValueTreeState& vts);
    ~HarmonizerEditor() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.

    HarmonizerAudioProcessor& processor;
    juce::AudioProcessorValueTreeState& valueTreeState;

    std::unique_ptr<SliderAttachment> semitoneSliderAtt1;
    std::unique_ptr<SliderAttachment> delaySliderAtt1;
    std::unique_ptr<SliderAttachment> fbSliderAtt1;

   

    MyLookAndFeel temaBellissimo;
    
    MyIncDecSlider temaIncDec;
    //[/UserVariables]

    //==============================================================================
    
    std::unique_ptr<juce::Slider> semitoneSlider1;
    std::unique_ptr<juce::Slider> delaySlider1;
    std::unique_ptr<juce::Slider> fbSlider1;
    
    juce::Path internalPath1;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HarmonizerEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

