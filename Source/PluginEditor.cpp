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
BigDistEnergyAudioProcessorEditor::BigDistEnergyAudioProcessorEditor (BigDistEnergyAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), state(vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(WIDTH, HEIGHT);

    distSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    distSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    distSlider.setPopupDisplayEnabled(true, false, this);
    distSlider.setRange(0.0, 1.0, 0.01);
    distSlider.setValue(*(audioProcessor.distAmt));
    wetSlider.setTextValueSuffix(" DIST AMT");
    distSlider.addListener(this);
    addAndMakeVisible(&distSlider);
    distSliderAttachment.reset(new SliderAttachment(state, "distAmt", distSlider));

    wetSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    wetSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    wetSlider.setPopupDisplayEnabled(true, false, this);
    wetSlider.setRange(0.0, 1.0, 0.01);
    wetSlider.setValue(*(audioProcessor.wetAmt));
    wetSlider.setTextValueSuffix(" % WET");
    wetSlider.addListener(this);
    addAndMakeVisible(&wetSlider);
    wetSliderAttachment.reset(new SliderAttachment(state, "wetAmt", wetSlider));

    gainInSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    gainInSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    gainInSlider.setPopupDisplayEnabled(true, false, this);
    gainInSlider.setRange(0.0, 10.0, 0.1);
    gainInSlider.setValue(*(audioProcessor.gainIn));
    gainInSlider.setTextValueSuffix(" INPUT GAIN");
    gainInSlider.addListener(this);
    addAndMakeVisible(&gainInSlider);
    gainInSliderAttachment.reset(new SliderAttachment(state, "gainIn", gainInSlider));

    colorSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    colorSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    colorSlider.setPopupDisplayEnabled(true, false, this);
    colorSlider.setRange(0.0, 20.0, 1);
    colorSlider.setValue(*(audioProcessor.color));
    colorSlider.setTextValueSuffix(" COLOR");
    colorSlider.addListener(this);
    addAndMakeVisible(&colorSlider);
    colorSliderAttachment.reset(new SliderAttachment(state, "color", colorSlider));

    juce::String types[AMT_TYPES] = { "square", "sin fold", "soft clip" };
    for (int i = 0; i < AMT_TYPES; i++) {
        typeMenu.addItem(types[i], i + 1);
    }
    typeMenu.setSelectedId(*(audioProcessor.type));
    typeMenu.addListener(this);
    addAndMakeVisible(&typeMenu);
    typeMenuAttachment.reset(new ComboBoxAttachment(state, "type", typeMenu));
}

BigDistEnergyAudioProcessorEditor::~BigDistEnergyAudioProcessorEditor()
{
}

void BigDistEnergyAudioProcessorEditor::sliderValueChanged(juce::Slider* slider) {
    *(audioProcessor.distAmt) = distSlider.getValue();
    *(audioProcessor.wetAmt) = wetSlider.getValue();
    *(audioProcessor.gainIn) = gainInSlider.getValue();
    *(audioProcessor.color) = colorSlider.getValue();
}

void BigDistEnergyAudioProcessorEditor::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) {
    *(audioProcessor.type) = typeMenu.getSelectedId();
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
    juce::FlexBox fbMain;
    fbMain.flexDirection = juce::FlexBox::Direction::column;
    fbMain.flexWrap = juce::FlexBox::Wrap::noWrap;
    fbMain.alignContent = juce::FlexBox::AlignContent::center;
    fbMain.justifyContent = juce::FlexBox::JustifyContent::flexStart;

    juce::FlexBox fbKnob;
    fbKnob.flexDirection = juce::FlexBox::Direction::row;
    fbKnob.flexWrap = juce::FlexBox::Wrap::noWrap;
    fbKnob.alignContent = juce::FlexBox::AlignContent::center;
    fbKnob.justifyContent = juce::FlexBox::JustifyContent::center;

    int amtItems = 4;
    int knobSize = getWidth() / amtItems;

    juce::Array<juce::FlexItem> knobs;
    knobs.add(juce::FlexItem(knobSize, knobSize, distSlider));
    knobs.add(juce::FlexItem(knobSize, knobSize, wetSlider));
    knobs.add(juce::FlexItem(knobSize, knobSize, gainInSlider));
    knobs.add(juce::FlexItem(knobSize, knobSize, colorSlider));
    fbKnob.items = knobs;

    juce::Array<juce::FlexItem> mainContent;

    juce::FlexItem dropDown = juce::FlexItem(getWidth(), knobSize / 4, typeMenu);
    dropDown.margin = juce::FlexItem::Margin(0, 0, knobSize / 4, 0);

    mainContent.add(dropDown);
    mainContent.add(juce::FlexItem(getWidth(), knobSize, fbKnob));
    fbMain.items = mainContent;

    fbMain.performLayout(getLocalBounds());
}
