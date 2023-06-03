/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 7.0.5

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

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class HarmonizerEditor  : public juce::AudioProcessorEditor,
                          public juce::Slider::Listener
{
public:
    //==============================================================================
    HarmonizerEditor (HarmonizerAudioProcessor& p, AudioProcessorValueTreeState& vts);
    ~HarmonizerEditor() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;

    // Binary resources:
    static const char* kineticHarmonizer_png;
    static const int kineticHarmonizer_pngSize;
    static const char* byXenharmonics_png;
    static const int byXenharmonics_pngSize;
    static const char* dry_png;
    static const int dry_pngSize;
    static const char* wet_png;
    static const int wet_pngSize;
    static const char* side_png;
    static const int side_pngSize;
    static const char* byXenharmonicsSq_png;
    static const int byXenharmonicsSq_pngSize;
    static const char* black_png;
    static const int black_pngSize;
    static const char* sider_png;
    static const int sider_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.

    HarmonizerAudioProcessor& processor;
    AudioProcessorValueTreeState& valueTreeState;

    std::unique_ptr<ButtonAttachment> muteButtonAtt;
    std::unique_ptr<SliderAttachment> gainSliderAtt;
    std::unique_ptr<SliderAttachment> panSliderAtt;
    std::unique_ptr<SliderAttachment> dryWetAtt;

    std::unique_ptr<ButtonAttachment> muteButtonAtt1;
    std::unique_ptr<SliderAttachment> gainSliderAtt1;
    std::unique_ptr<SliderAttachment> panSliderAtt1;
    std::unique_ptr<SliderAttachment> semitoneSliderAtt1;
    std::unique_ptr<SliderAttachment> delaySliderAtt1;
    std::unique_ptr<SliderAttachment> fbSliderAtt1;



    MyLookAndFeel temaBellissimo;
    MyLinearSlider temaSlider;
    MyHorizontalSlider temaPan;
    CustomButton temaButton;
    MyIncDecSlider temaIncDec;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Slider> gainSlider;
    std::unique_ptr<Meter> meterComponent;
    std::unique_ptr<juce::Slider> panSlider;
    std::unique_ptr<juce::Slider> gainSlider1;
    std::unique_ptr<Meter> meterComponent1;
    std::unique_ptr<juce::Slider> panSlider1;
    std::unique_ptr<juce::Slider> dryWet;
    std::unique_ptr<juce::Slider> semitoneSlider1;
    std::unique_ptr<juce::Slider> delaySlider1;
    std::unique_ptr<juce::Slider> fbSlider1;
    std::unique_ptr<juce::ToggleButton> muteButton;
    std::unique_ptr<juce::ToggleButton> muteButton1;
    juce::Image cachedImage_wet_png_1;
    juce::Image cachedImage_dry_png_2;
    juce::Image cachedImage_sider_png_3;
    juce::Image cachedImage_side_png_4;
    juce::Image cachedImage_byXenharmonicsSq_png_5;
    juce::Image cachedImage_kineticHarmonizer_png_6;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HarmonizerEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

