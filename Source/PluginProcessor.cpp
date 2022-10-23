/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define PI 3.14

//==============================================================================
BigDistEnergyAudioProcessor::BigDistEnergyAudioProcessor()
    : parameters(*this, nullptr, juce::Identifier("BDE_STATE"), {
            std::make_unique<juce::AudioParameterFloat>("distAmt", "Distortion Amount", 0.0f, 1.0f, 0.5f),
            std::make_unique<juce::AudioParameterFloat>("wetAmt", "Wet Amount", 0.0f, 1.0f, 0.5f),
            std::make_unique<juce::AudioParameterFloat>("gainIn", "Input Gain", 0.0f, 10.0f, 1.0f),
            std::make_unique<juce::AudioParameterFloat>("color", "Color", 0.0f, 20.0f, 1.0f),
            std::make_unique<juce::AudioParameterInt>("type", "Distortion Type", 1, 3, 1)
        }),
    #ifndef JucePlugin_PreferredChannelConfigurations
    AudioProcessor(BusesProperties()
    #if ! JucePlugin_IsMidiEffect
    #if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
    #endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
    #endif
    )
    #endif
{
    distAmt = parameters.getRawParameterValue("distAmt");
    wetAmt = parameters.getRawParameterValue("wetAmt");
    gainIn = parameters.getRawParameterValue("gainIn");
    color = parameters.getRawParameterValue("color");
    type = parameters.getRawParameterValue("type");
}

BigDistEnergyAudioProcessor::~BigDistEnergyAudioProcessor()
{
}

//==============================================================================
const juce::String BigDistEnergyAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BigDistEnergyAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BigDistEnergyAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BigDistEnergyAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BigDistEnergyAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BigDistEnergyAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BigDistEnergyAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BigDistEnergyAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BigDistEnergyAudioProcessor::getProgramName (int index)
{
    return {};
}

void BigDistEnergyAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void BigDistEnergyAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void BigDistEnergyAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BigDistEnergyAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void BigDistEnergyAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
            // TYPES: 1. square, 2. sin fold, 3. soft clip

            float dry = channelData[sample];

            // Input gain is applied
            channelData[sample] = channelData[sample] * *gainIn;

            // TODO: Two distortion modules in parallel
            // TODO: Compressor that controls distortion, look up VCA
            // TODO: Use input signal (volume) to modulate distortion, look up envelope followers

            float ACTIVE_THRESHOLD = 0.01; //dont know what value to pick exactly

            switch ((int) *type) {
            case 1:
                // Square waveshaper
                if (abs(channelData[sample]) >= ACTIVE_THRESHOLD) channelData[sample] = (channelData[sample] >= (*distAmt - 0.5) / 4 ? 1.f : -1.f);
                break;
            case 2:
                // Sine foldover, to expand watch 10:27 in fifty shades of distortion
                if (abs(channelData[sample]) >= 1 - *distAmt) {
                    for (int i = 1; i <= *color; i++) {
                        channelData[sample] += sin(i * channelData[sample]);
                    }
                }
                break;
            case 3:
                // Soft clip
                channelData[sample] = 2 / PI * atan(*distAmt * 100 * channelData[sample]);
                break;
            }

            


            // WET/DRY mix
            channelData[sample] = channelData[sample] * *wetAmt + dry * (1 - *wetAmt);
        }
    }
}

//==============================================================================
bool BigDistEnergyAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BigDistEnergyAudioProcessor::createEditor()
{
    return new BigDistEnergyAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void BigDistEnergyAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void BigDistEnergyAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BigDistEnergyAudioProcessor();
}
