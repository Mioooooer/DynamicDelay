/*
  ==============================================================================

    DynamicDelayAudioProcessorEditor.cpp
    Created: 2 Feb 2023 3:18:09pm
    Author:  quanjx

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DynamicDelayAudioProcessorEditor::DynamicDelayAudioProcessorEditor(DynamicDelayAudioProcessor& p)
    :AudioProcessorEditor (&p),
     processor (p),
     delayLengthLabel("", "Delay (sec):"),
     feedbackLabel("", "Feedback:"),
     dryMixLabel("", "Dry:"),
     wetMixLabel("", "Wet:"),
     crossLengthLabel("", "cross (sec):")
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setSize (690, 471);
    
    delayLengthSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    delayLengthSlider.addListener(this);
    delayLengthSlider.setRange(0.01, 2.0, 0.01);
    addAndMakeVisible(&delayLengthSlider);
    
    feedbackSlider.setSliderStyle(juce::Slider::Rotary);
    feedbackSlider.addListener(this);
    feedbackSlider.setRange(0.0, 0.995, 0.005);
    addAndMakeVisible(&feedbackSlider);
    
    dryMixSlider.setSliderStyle(juce::Slider::Rotary);
    dryMixSlider.addListener(this);
    dryMixSlider.setRange(0.0, 1.0, 0.01);
    addAndMakeVisible(&dryMixSlider);

    wetMixSlider.setSliderStyle(juce::Slider::Rotary);
    wetMixSlider.addListener(this);
    wetMixSlider.setRange(0.0, 1.0, 0.01);
    addAndMakeVisible(&wetMixSlider);

    crossLengthSlider.setSliderStyle(juce::Slider::Rotary);
    crossLengthSlider.addListener(this);
    crossLengthSlider.setRange(0.0, 2.0, 0.01);
    addAndMakeVisible(&crossLengthSlider);
    
    delayLengthLabel.attachToComponent(&delayLengthSlider, false);
    delayLengthLabel.setFont(juce::Font (11.0f));

    feedbackLabel.attachToComponent(&feedbackSlider, false);
    feedbackLabel.setFont(juce::Font (11.0f));

    dryMixLabel.attachToComponent(&dryMixSlider, false);
    dryMixLabel.setFont(juce::Font (11.0f));

    wetMixLabel.attachToComponent(&wetMixSlider, false);
    wetMixLabel.setFont(juce::Font (11.0f));
    
    crossLengthLabel.attachToComponent(&crossLengthSlider, false);
    crossLengthLabel.setFont(juce::Font (11.0f));

    startTimer(50);

}

DynamicDelayAudioProcessorEditor::~DynamicDelayAudioProcessorEditor()
{
}

void DynamicDelayAudioProcessorEditor::timerCallback()
{
    DynamicDelayAudioProcessor* ourProcessor = getProcessor();
    
    delayLengthSlider.setValue(ourProcessor->delayLength, juce::dontSendNotification);
    feedbackSlider.setValue(ourProcessor->feedback, juce::dontSendNotification);
    dryMixSlider.setValue(ourProcessor->dryMix, juce::dontSendNotification);
    wetMixSlider.setValue(ourProcessor->wetMix, juce::dontSendNotification);
    crossLengthSlider.setValue(ourProcessor->crossLength, juce::dontSendNotification);
}

void DynamicDelayAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &delayLengthSlider) {
        getProcessor()->setParameterNotifyingHost(DynamicDelayAudioProcessor::delayLengthParam, (float) delayLengthSlider.getValue());
    } else if (slider == &feedbackSlider) {
        getProcessor()->setParameterNotifyingHost(DynamicDelayAudioProcessor::feedbackParam, (float) feedbackSlider.getValue());
    } else if (slider == &dryMixSlider) {
        getProcessor()->setParameterNotifyingHost(DynamicDelayAudioProcessor::dryMixParam, (float) dryMixSlider.getValue());
    } else if (slider == &wetMixSlider) {
        getProcessor()->setParameterNotifyingHost(DynamicDelayAudioProcessor::wetMixParam, (float) wetMixSlider.getValue());
    } else if (slider == &crossLengthSlider) {
        getProcessor()->setParameterNotifyingHost(DynamicDelayAudioProcessor::crossLengthParam, (float) crossLengthSlider.getValue());
    }
}

void DynamicDelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    // getFromFile();
    g.fillAll (juce::Colours::black);
    // g.drawImage (backgroundImage, 0, 0, getWidth(), getHeight(), 0, 0, backgroundImage.getWidth(), backgroundImage.getHeight(), false );


    g.setColour (juce::Colours::black);
    g.setFont (15.0f);
    g.drawFittedText ("This is Dynamic Delay", getLocalBounds(), juce::Justification::centred, 1);
}

void DynamicDelayAudioProcessorEditor::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    delayLengthSlider.setBounds(20, 20, 150, 40);
    feedbackSlider.setBounds(200, 20, 150, 40);
    dryMixSlider.setBounds(20, 80, 150, 40);
    wetMixSlider.setBounds(200, 80, 150, 40);
    crossLengthSlider.setBounds(20, 140, 150, 40);
}
