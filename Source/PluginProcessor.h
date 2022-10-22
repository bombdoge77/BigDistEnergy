/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class BigDistEnergyAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    BigDistEnergyAudioProcessor();
    ~BigDistEnergyAudioProcessor() override;

    std::atomic<float>* distAmt = nullptr;
    std::atomic<float>* wetAmt = nullptr;
    std::atomic<float>* gainIn = nullptr;
    std::atomic<float>* color = nullptr;
    std::atomic<float>* type = nullptr;
    /*float distAmt = 0.5;
    float wetAmt = 0.5;
    float gain_in = 1;
    float color = 1;
    int type = 1;*/

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
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

private:
    juce::AudioProcessorValueTreeState parameters;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BigDistEnergyAudioProcessor)
};
