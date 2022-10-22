/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef juce::AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;

//==============================================================================
/**
*/
class BigDistEnergyAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                            private juce::Slider::Listener,
                                            private juce::ComboBox::Listener
{
public:
    BigDistEnergyAudioProcessorEditor (BigDistEnergyAudioProcessor&, juce::AudioProcessorValueTreeState& vts);
    ~BigDistEnergyAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    juce::AudioProcessorValueTreeState& state;

    BigDistEnergyAudioProcessor& audioProcessor;
    void sliderValueChanged(juce::Slider* slider) override;
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
    juce::Slider distSlider;
    juce::Slider wetSlider;
    juce::Slider gainInSlider;
    juce::Slider colorSlider;
    juce::ComboBox typeMenu;

    std::unique_ptr<SliderAttachment> distSliderAttachment;
    std::unique_ptr<SliderAttachment> wetSliderAttachment;
    std::unique_ptr<SliderAttachment> gainInSliderAttachment;
    std::unique_ptr<SliderAttachment> colorSliderAttachment;
    std::unique_ptr<ComboBoxAttachment> typeMenuAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BigDistEnergyAudioProcessorEditor)
};
