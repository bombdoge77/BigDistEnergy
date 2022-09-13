/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class BigDistEnergyAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                            private juce::Slider::Listener,
                                            private juce::ComboBox::Listener
{
public:
    BigDistEnergyAudioProcessorEditor (BigDistEnergyAudioProcessor&);
    ~BigDistEnergyAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    BigDistEnergyAudioProcessor& audioProcessor;
    void sliderValueChanged(juce::Slider* slider) override;
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
    juce::Slider distSlider;
    juce::Slider wetSlider;
    juce::Slider gainInSlider;
    juce::Slider colorSlider;
    juce::ComboBox typeMenu;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BigDistEnergyAudioProcessorEditor)
};
