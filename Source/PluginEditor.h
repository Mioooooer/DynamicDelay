/*
  ==============================================================================

    DynamicDelayAudioProcessorEditor.h
    Created: 2 Feb 2023 3:18:09pm
    Author:  quanjx

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class DynamicDelayAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener, public juce::Timer
{
public:
    DynamicDelayAudioProcessorEditor(DynamicDelayAudioProcessor&);
    ~DynamicDelayAudioProcessorEditor() override;

    void timerCallback();
    void paint (juce::Graphics&);
    void resized() override;
    void sliderValueChanged (juce::Slider*);

private:
    DynamicDelayAudioProcessor& processor;
    juce::Label delayLengthLabel, feedbackLabel, dryMixLabel, wetMixLabel, crossLengthLabel;
    juce::Slider delayLengthSlider, feedbackSlider, dryMixSlider, wetMixSlider, crossLengthSlider;
    
    juce::ScopedPointer<juce::ResizableCornerComponent> resizer;
    juce::ComponentBoundsConstrainer resizeLimits;
    
    DynamicDelayAudioProcessor* getProcessor() const
    {
        return static_cast <DynamicDelayAudioProcessor*> (getAudioProcessor());
    }
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DynamicDelayAudioProcessorEditor)
};
