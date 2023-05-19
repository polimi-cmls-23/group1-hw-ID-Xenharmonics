#include "PluginProcessor.h"
#include "Parameters.h"
#include "HarmonizerEditor.h"

#define DEFAULT_FILTER 20000.0f
#define DEFAULT_QFILTER 0.707f

HarmonizerAudioProcessor::HarmonizerAudioProcessor()
    : parameters(*this, nullptr, "APVTS", Parameters::createParameterLayout())
{
    parameters.addParameterListener(NAME_ST1, this);
    parameters.addParameterListener(NAME_DT1, this);
    parameters.addParameterListener(NAME_FB1, this);
   
    parameters.addParameterListener(NAME_FREQ, this);
    parameters.addParameterListener(NAME_MOD, this);

    drywetter.setDryWetRatio(DEFAULT_DW);

    pitchShift1.pitchSetParameters();
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

    

    pitchShift1.processBlock(buffer, dualmono, numSamples, totalNumInputChannels, totalNumOutputChannels);
    
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
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void HarmonizerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

void HarmonizerAudioProcessor::parameterChanged(const juce::String& paramID, float newValue)
{

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









