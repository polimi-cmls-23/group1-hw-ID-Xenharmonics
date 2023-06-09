#include "PluginProcessor.h"
#include "Parameters.h"
#include "HarmonizerEditor.h"

#define DEFAULT_FILTER 20000.0f
#define DEFAULT_QFILTER 0.707f

HarmonizerAudioProcessor::HarmonizerAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
    ), parameters(*this, nullptr, "APVTS", Parameters::createParameterLayout())

{

    parameters.addParameterListener(NAME_MUTE, this);
    parameters.addParameterListener(NAME_LEVEL, this);
    parameters.addParameterListener(NAME_PAN, this);

    parameters.addParameterListener(NAME_MUTE1, this);
    parameters.addParameterListener(NAME_LEVEL1, this);
    parameters.addParameterListener(NAME_PAN1, this);
    parameters.addParameterListener(NAME_ST1, this);
    parameters.addParameterListener(NAME_DT1, this);
    parameters.addParameterListener(NAME_FB1, this);
       
    parameters.addParameterListener(NAME_DW, this);
    parameters.addParameterListener(NAME_FREQ, this);
    parameters.addParameterListener(NAME_MOD, this);

    channelStripOriginal.setMute(DEFAULT_MUTE);
    channelStripOriginal.setLevel(DEFAULT_LEVEL);
    channelStripOriginal.setPan(DEFAULT_PAN);

    drywetter.setDryWetRatio(DEFAULT_DW);

    pitchShift1.pitchSetParameters();

    connect(8000);
    if (!connect(8000))                       // [3]
        std::cout << "Error" << std::endl;
    OSCReceiver::addListener(this, "/data");
}

HarmonizerAudioProcessor::~HarmonizerAudioProcessor()
{
}


void HarmonizerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    sr = sampleRate;
    drywetter.prepareToPlay(sampleRate, samplesPerBlock);
    dualmono.setSize(2, samplesPerBlock);
    dualmono.clear();

    channelStripOriginal.prepareToPlay(sampleRate);
    pitchShift1.prepareToPlay(sr, samplesPerBlock);

}

void HarmonizerAudioProcessor::releaseResources()
{
    
    drywetter.releaseResources();
    dualmono.setSize(0, 0);

    pitchShift1.releaseResources();
}

void HarmonizerAudioProcessor::oscMessageReceived(const OSCMessage& message)
{

    if (message[0].isString())
    {
        String theMessage = message[0].getString();

        juce::NormalisableRange<float> range(0.0f, 5.0f, 0.001f);
        juce::NormalisableRange<float> range2(-12.0f, 12.0f, 0.0f);
        juce::NormalisableRange<float> range3(0.0f, 1.0f, 1.0f);

        int pos_x = theMessage.indexOf(" ");
        String x_axis = theMessage.substring(0, pos_x);
        String x_rest = theMessage.substring(pos_x + 1);
        String z_axis = x_rest;
        
        float x = x_axis.getFloatValue();
        x = mapf(x, -90.0f, 90.0f, -12.0f, 12.0f);
        x = range2.convertTo0to1(x);
        float z = z_axis.getFloatValue();
        z = mapf(z, -90.0f, 90.0f, 0.0f, 1.0f);
        z = range3.convertTo0to1(z);

        parameters.getParameter(NAME_ST1)->beginChangeGesture();
        parameters.getParameter(NAME_ST1)->setValueNotifyingHost(x);
        parameters.getParameter(NAME_ST1)->endChangeGesture();
        
        parameters.getParameter(NAME_DW)->beginChangeGesture();
        parameters.getParameter(NAME_DW)->setValueNotifyingHost(z);
        parameters.getParameter(NAME_DW)->endChangeGesture();

    }

}



void HarmonizerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    const auto numSamples = buffer.getNumSamples();

    // Salvo il segnale in input pulito
    drywetter.setDry(buffer);


    if (totalNumInputChannels == 1) {   
        dualmono.copyFrom(0, 0, buffer, 0, 0, numSamples);
        dualmono.copyFrom(1, 0, buffer, 0, 0, numSamples);
    }
    else {
        dualmono.copyFrom(0, 0, buffer, 0, 0, numSamples);
        dualmono.copyFrom(1, 0, buffer, 1, 0, numSamples);
        dualmono.addFrom(0, 0, dualmono, 1, 0, numSamples);
        dualmono.copyFrom(1, 0, dualmono, 0, 0, numSamples);
        dualmono.applyGain(0.5f);
    }

    channelStripOriginal.processBlock(buffer, totalNumInputChannels, totalNumOutputChannels);
    envelope.set(jmax(buffer.getMagnitude(0, buffer.getNumSamples()), envelope.get()));

    pitchShift1.processBlock(buffer, dualmono, numSamples, totalNumInputChannels, totalNumOutputChannels);
    envelopeVoice1.set(jmax(pitchShift1.copyDualMono.getMagnitude(0, pitchShift1.copyDualMono.getNumSamples()), envelopeVoice1.get()));

    // Miscelo il segnale pulito salvato in drywetter con quello processato da delay
    drywetter.merge(buffer);

        
}

//==============================================================================

juce::AudioProcessorEditor* HarmonizerAudioProcessor::createEditor()
{
    return new HarmonizerEditor (*this, parameters); 
}

void HarmonizerAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void HarmonizerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(ValueTree::fromXml(*xmlState));
}

void HarmonizerAudioProcessor::parameterChanged(const String& paramID, float newValue)
{
    if (paramID == NAME_MUTE)
        channelStripOriginal.setMute(newValue);

    if (paramID == NAME_LEVEL)
        channelStripOriginal.setLevel(newValue);

    if (paramID == NAME_PAN)
        channelStripOriginal.setPan(newValue);

    if (paramID == NAME_DW)
        drywetter.setDryWetRatio(newValue);

    if (paramID == NAME_MUTE1)
        pitchShift1.setMute(newValue);

    if (paramID == NAME_LEVEL1)
        pitchShift1.setLevel(newValue);

    if (paramID == NAME_PAN1)
        pitchShift1.setPan(newValue);

    if (paramID == NAME_DT1)
        pitchShift1.setDelay(newValue);

    if (paramID == NAME_FB1)
        pitchShift1.setFeedback(newValue);

    if (paramID == NAME_ST1)
        pitchShift1.setSemitones(newValue, sr);


}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HarmonizerAudioProcessor();
}











