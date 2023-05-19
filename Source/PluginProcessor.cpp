#include "PluginProcessor.h"
#include "Parameters.h"
#include "HarmonizerEditor.h"
#include <string>

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
    
    connect(8000);
        if (! connect (8000))                       // [3]
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


void HarmonizerAudioProcessor::oscMessageReceived (const OSCMessage &message)
    {
        
        if (message[0].isString())
        {
            String theMessage = message[0].getString();
            
            int pos_x = theMessage.indexOf(" ");
            String x_axis = theMessage.substring(0, pos_x);
            String x_rest = theMessage.substring(pos_x + 1);
            int pos_y = x_rest.indexOf(" ");
            String y_axis = x_rest.substring(0, pos_y);
            String y_rest = x_rest.substring(pos_y + 1);
            String z_axis = y_rest;
            
            juce::NormalisableRange<float> range(0.0f, 5.0f, 0.01f);
            juce::NormalisableRange<float> range2(-12.0f, 12.0f, 0.0f);
            
            float x = x_axis.getFloatValue();
            x = range2.convertTo0to1(x);
            float y = y_axis.getFloatValue();
            y = range.convertTo0to1(y);
            float z = z_axis.getFloatValue();
            
            /*int precision = 2;
            // Round the float value to the desired precision
            y = std::round(y * std::pow(10, precision)) / std::pow(10, precision);*/
            
            parameters.getParameter(NAME_ST1)->beginChangeGesture();
            parameters.getParameter(NAME_ST1)->setValueNotifyingHost(x);
            parameters.getParameter(NAME_ST1)->endChangeGesture();
            
            parameters.getParameter(NAME_DT1)->beginChangeGesture();
            parameters.getParameter(NAME_DT1)->setValueNotifyingHost(y);
            parameters.getParameter(NAME_DT1)->endChangeGesture();
            
            parameters.getParameter(NAME_FB1)->beginChangeGesture();
            parameters.getParameter(NAME_FB1)->setValueNotifyingHost(z);
            parameters.getParameter(NAME_FB1)->endChangeGesture();
            
            
            std::cout << x << " " << y << " " << z << std::endl;
            //
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









