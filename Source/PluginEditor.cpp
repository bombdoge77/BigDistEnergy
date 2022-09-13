/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define WIDTH 600
#define HEIGHT 300
#define AMT_TYPES 3

//==============================================================================
BigDistEnergyAudioProcessorEditor::BigDistEnergyAudioProcessorEditor (BigDistEnergyAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(WIDTH, HEIGHT);

    distSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    distSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    distSlider.setPopupDisplayEnabled(true, false, this);
    distSlider.setRange(0.0, 1.0, 0.01);
    distSlider.setValue(audioProcessor.distAmt);
    wetSlider.setTextValueSuffix(" DIST AMT");
    distSlider.addListener(this);
    addAndMakeVisible(&distSlider);

    wetSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    wetSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    wetSlider.setPopupDisplayEnabled(true, false, this);
    wetSlider.setRange(0.0, 1.0, 0.01);
    wetSlider.setValue(audioProcessor.wetAmt);
    wetSlider.setTextValueSuffix(" % WET");
    wetSlider.addListener(this);
    addAndMakeVisible(&wetSlider);

    gainInSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    gainInSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    gainInSlider.setPopupDisplayEnabled(true, false, this);
    gainInSlider.setRange(0.0, 10.0, 0.1);
    gainInSlider.setValue(audioProcessor.gain_in);
    gainInSlider.setTextValueSuffix(" INPUT GAIN");
    gainInSlider.addListener(this);
    addAndMakeVisible(&gainInSlider);

    colorSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    colorSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    colorSlider.setPopupDisplayEnabled(true, false, this);
    colorSlider.setRange(0.0, 20.0, 1);
    colorSlider.setValue(audioProcessor.color);
    colorSlider.setTextValueSuffix(" COLOR");
    colorSlider.addListener(this);
    addAndMakeVisible(&colorSlider);

    juce::String types[AMT_TYPES] = { "square", "sin fold", "soft clip" };
    for (int i = 0; i < AMT_TYPES; i++) {
        typeMenu.addItem(types[i], i + 1);
    }
    typeMenu.setSelectedId(1);
    typeMenu.addListener(this);
    addAndMakeVisible(&typeMenu);
}

BigDistEnergyAudioProcessorEditor::~BigDistEnergyAudioProcessorEditor()
{
}

void BigDistEnergyAudioProcessorEditor::sliderValueChanged(juce::Slider* slider) {
    audioProcessor.distAmt = distSlider.getValue();
    audioProcessor.wetAmt = wetSlider.getValue();
    audioProcessor.gain_in = gainInSlider.getValue();
    audioProcessor.color = colorSlider.getValue();
}

void BigDistEnergyAudioProcessorEditor::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) {
    audioProcessor.type = typeMenu.getSelectedId();
}

//==============================================================================
void BigDistEnergyAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Hello World", getLocalBounds(), juce::Justification::centred, 1);
}

void BigDistEnergyAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    int maxDim;

    if (WIDTH >= HEIGHT) {
        maxDim = WIDTH;
    }
    else {
        maxDim = HEIGHT;
    }

    float knobAmt = 4;
    float knobScale = 1.f/(knobAmt + 1); //decimal percentage of largest dimension
    int knobSize = maxDim*knobScale;
    int margin = knobSize / 2;
    int space = (WIDTH - margin * 2) / knobAmt;

    //N * WIDTH / knobAmt

    distSlider.setBounds(margin + 1 * space, HEIGHT / 2 - knobSize /2, knobSize, knobSize);
    wetSlider.setBounds(margin + 3 * space, HEIGHT / 2 - knobSize / 2, knobSize, knobSize);
    gainInSlider.setBounds(margin + 0 * space, HEIGHT / 2 - knobSize / 2, knobSize, knobSize);
    colorSlider.setBounds(margin + 2 * space, HEIGHT / 2 - knobSize / 2, knobSize, knobSize);

    int dropDownWidth = WIDTH / 4;
    int dropDownHeight = 30;
    typeMenu.setBounds(WIDTH / 2 - dropDownWidth / 2, 0.1 * knobSize, dropDownWidth, dropDownHeight);
}
