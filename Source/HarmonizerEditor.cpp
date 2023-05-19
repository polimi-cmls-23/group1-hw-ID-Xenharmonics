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

//[Headers] You can add your own extra header files here...
#include "PluginProcessor.h"
#include "Parameters.h"
//[/Headers]

#include "HarmonizerEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
HarmonizerEditor::HarmonizerEditor (HarmonizerAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState(vts)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    semitoneSlider1.reset(new juce::Slider("Semitone"));
    addAndMakeVisible(semitoneSlider1.get());
    semitoneSlider1->setRange(0, 10, 0);
    semitoneSlider1->setSliderStyle(juce::Slider::IncDecButtons);
    semitoneSlider1->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    semitoneSlider1->setColour(juce::Slider::thumbColourId, juce::Colour(0xff3587a8));

    semitoneSlider1->setBounds(30, 45, 35, 60);

    delaySlider1.reset(new juce::Slider("Delay"));
    addAndMakeVisible(delaySlider1.get());
    delaySlider1->setRange(0, 10, 0);
    delaySlider1->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    delaySlider1->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 16);

    delaySlider1->setBounds(100, 30, 56, 80);

    fbSlider1.reset(new juce::Slider("FB"));
    addAndMakeVisible(fbSlider1.get());
    fbSlider1->setRange(0, 10, 0);
    fbSlider1->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    fbSlider1->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 16);

    fbSlider1->setBounds(200, 30, 56, 80);

    //[UserPreSize]

    semitoneSliderAtt1.reset(new SliderAttachment(valueTreeState, NAME_ST1, *semitoneSlider1));
    delaySliderAtt1.reset(new SliderAttachment(valueTreeState, NAME_DT1, *delaySlider1));
    fbSliderAtt1.reset(new SliderAttachment(valueTreeState, NAME_FB1, *fbSlider1));


    semitoneSlider1->setLookAndFeel(&temaIncDec);

    delaySlider1->setLookAndFeel(&temaBellissimo);

    fbSlider1->setLookAndFeel(&temaBellissimo);

    internalPath1.startNewSubPath(0.0f, 700.0f);
    internalPath1.lineTo(0.0f, 0.0f);
    internalPath1.lineTo(750.0f, 0.0f);
    internalPath1.lineTo(750.0f, 700.0f);
    internalPath1.closeSubPath();

    //[/UserPreSize]

    setSize (300, 150);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

HarmonizerEditor::~HarmonizerEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    
    semitoneSliderAtt1 = nullptr;
    delaySliderAtt1 = nullptr;
    fbSliderAtt1 = nullptr;

    semitoneSlider1->setLookAndFeel(nullptr);
    delaySlider1->setLookAndFeel(nullptr);
    fbSlider1->setLookAndFeel(nullptr);
    

    //[/Destructor_pre]

    semitoneSlider1 = nullptr;
    delaySlider1 = nullptr;
    fbSlider1 = nullptr;

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void HarmonizerEditor::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll(juce::Colour(0xff171717));
    {
        float x = 0, y = 0;
        juce::Colour fillColour1 = juce::Colour(0xff171717), fillColour2 = juce::Colour(0xff9400d3);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setGradientFill(juce::ColourGradient(fillColour1,
            125.0f - 0.0f + x,
            24.0f - 0.0f + y,
            fillColour2,
            125.0f - 0.0f + x,
            175.0f - 0.0f + y,
            false));
        g.fillPath(internalPath1, juce::AffineTransform::translation(x, y));
    }


    {
        int x = 20, y = 15, width = 56, height = 20;
        juce::String text(TRANS("St "));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular").withExtraKerningFactor(0.250f));
        g.drawText(text, x, y, width, height,
            juce::Justification::centred, true);
    }

    {
        int x = 90, y = 15, width = 80, height = 20;
        juce::String text(TRANS("Delay"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular").withExtraKerningFactor(0.250f));
        g.drawText(text, x, y, width, height,
            juce::Justification::centred, true);
    }

    {
        int x = 200, y = 15, width = 60, height = 20;
        juce::String text(TRANS("Fb"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular").withExtraKerningFactor(0.250f));
        g.drawText(text, x, y, width, height,
            juce::Justification::centred, true);
    }

    
}

void HarmonizerEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}






//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="HarmonizerEditor" componentName=""
                 parentClasses="public juce::AudioProcessorEditor" constructorParams="HarmonizerAudioProcessor&amp; p, AudioProcessorValueTreeState&amp; vts"
                 variableInitialisers="AudioProcessorEditor (&amp;p), processor (p), valueTreeState(vts)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="800" initialHeight="800">
  <BACKGROUND backgroundColour="ff232a2e">
    
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

//[EndFile] You can add extra defines here...
//[/EndFile]

