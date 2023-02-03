/*
  ==============================================================================

    DynamicDelayAudioProcessor.cpp
    Created: 2 Feb 2023 2:58:19pm
    Author:  quanjx

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DynamicDelayAudioProcessor::DynamicDelayAudioProcessor():delayBuffer(2,1)
{
    // set default values
    delayLength = 0.5;
    dryMix = 1.0;
    wetMix = 0.0;
    feedback = 0.75;
    delayBufferLength = 1;
    
    delayReadPosition = 0;
    delayWritePosition = 0;
    
    lastUIWidth = 370;
    lastUIHeight = 140;

}

DynamicDelayAudioProcessor::~DynamicDelayAudioProcessor()
{
}

//==============================================================================
const juce::String DynamicDelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int DynamicDelayAudioProcessor::getNumParameters()
{
    return numParameters;
}

float DynamicDelayAudioProcessor::getParameter (int index)
{
    switch (index) {
        case dryMixParam:
            return dryMix;
        case wetMixParam:
            return wetMix;
        case feedbackParam:
            return feedback;
        case delayLengthParam:
            return delayLength;
        default:
            return 0.0f;
    }
}

void DynamicDelayAudioProcessor::setParameter (int index, float newValue)
{
    switch (index) {
        case dryMixParam:
            dryMix = newValue;
            break;
        case wetMixParam:
            wetMix = newValue;
            break;
        case feedbackParam:
            feedback = newValue;
            break;
        case delayLengthParam:
            delayLength = newValue;
            delayReadPosition = (int) (delayWritePosition - (delayLength * getSampleRate()) + delayBufferLength) % delayBufferLength;
            break;
        default:
            break;;
    }
}

const juce::String DynamicDelayAudioProcessor::getParameterName (int index)
{
    switch (index) {
        case dryMixParam:
            return "dry mix";
        case wetMixParam:
            return "wet mix";
        case feedbackParam:
            return "feedback";
        case delayLengthParam:
            return "delay";
        default:
            return juce::String::String();
    }
}

const juce::String DynamicDelayAudioProcessor::getParameterText (int index)
{
    return juce::String();
}

const juce::String DynamicDelayAudioProcessor::getInputChannelName (int channelIndex) const
{
    return juce::String (channelIndex + 1);
}

const juce::String DynamicDelayAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return juce::String (channelIndex + 1);
}

bool DynamicDelayAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool DynamicDelayAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool DynamicDelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DynamicDelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DynamicDelayAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double DynamicDelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DynamicDelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DynamicDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DynamicDelayAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DynamicDelayAudioProcessor::getProgramName (int index)
{
    return juce::String();
}

void DynamicDelayAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DynamicDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    delayBufferLength  = (int) 2.0 * sampleRate;
    if (delayBufferLength < 1)
        delayBufferLength = 1;
    
    delayBuffer.setSize(2, delayBufferLength);
    delayBuffer.clear();
    
    delayReadPosition = (int) (delayWritePosition - (delayLength * getSampleRate()) + delayBufferLength) % delayBufferLength;
}

void DynamicDelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void DynamicDelayAudioProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiMessages)
{
    const int numInputChannels = getNumInputChannels();
    const int numOutputChannels = getNumOutputChannels();
    const int numSamples = buffer.getNumSamples();
    
    int dpr, dpw;

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    for (int channel = 0; channel < numInputChannels; ++channel) {
        float* channelData = buffer.getWritePointer(channel);
        float* delayData = delayBuffer.getWritePointer(juce::jmin(channel, delayBuffer.getNumChannels() - 1));
        
        dpr = delayReadPosition;
        dpw = delayWritePosition;
        
        for (int i = 0; i < numSamples; ++i) {
            const float in = channelData[i];
            float out = 0.0;
            
            out = (dryMix * in + wetMix * delayData[dpr]);
            
            delayData[dpw] = in + (delayData[dpr] * feedback);
            
            if (++dpr >= delayBufferLength)
                dpr = 0;
            if (++dpw >= delayBufferLength)
                dpw = 0;
            
            channelData[i] = out;
        }
    }
    
    delayReadPosition = dpr;
    delayWritePosition = dpw;
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // I've added this to avoid people getting screaming feedback
    // when they first compile the plugin, but obviously you don't need to
    // this code if your algorithm already fills all the output channels.
    for (int i = numInputChannels; i < numOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

}

//==============================================================================
bool DynamicDelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DynamicDelayAudioProcessor::createEditor()
{
    return new DynamicDelayAudioProcessorEditor (*this);
}

//==============================================================================
void DynamicDelayAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DynamicDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DynamicDelayAudioProcessor();
}
