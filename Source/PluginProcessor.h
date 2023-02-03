/*
  ==============================================================================

    DynamicDelayAudioProcessor.h
    Created: 2 Feb 2023 2:58:19pm
    Author:  quanjx

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class DynamicDelayAudioProcessor  : public juce::AudioProcessor
{
public:
    DynamicDelayAudioProcessor();
    ~DynamicDelayAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (juce::AudioSampleBuffer&, juce::MidiBuffer&) override;
    
    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    
    //==============================================================================
    const juce::String getName() const override;

    int getNumParameters() override;
    float getParameter (int index) override;
    void setParameter (int index, float newValue) override;

    const juce::String getParameterName (int index) override;
    const juce::String getParameterText (int index) override;

    const juce::String getInputChannelName (int channelIndex) const override;
    const juce::String getOutputChannelName (int channelIndex) const override;
    bool isInputChannelStereoPair (int index) const override;
    bool isOutputChannelStereoPair (int index) const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool silenceInProducesSilenceOut() const override;
    double getTailLengthSeconds() const override;
    
    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    int lastUIWidth, lastUIHeight;
    
    enum Parameters
    {
        delayLengthParam = 0,
        dryMixParam,
        wetMixParam,
        feedbackParam,
        numParameters
    };
    
    float delayLength;
    float dryMix;
    float wetMix;
    float feedback;

private:
    // circular buffer variables
    juce::AudioSampleBuffer delayBuffer;
    int delayBufferLength;
    int delayReadPosition;
    int delayWritePosition;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DynamicDelayAudioProcessor)
};
