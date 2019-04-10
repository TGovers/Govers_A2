/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Govers_a2AudioProcessorEditor::Govers_a2AudioProcessorEditor (Govers_a2AudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setOpaque(true);
    setSize (400, 300);
    
    
    mixSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    mixSlider.setRange(0, 100.0);
    mixSlider.setTextValueSuffix("%");
    mixSlider.setColour(Slider::ColourIds::rotarySliderFillColourId, Colours::pink);
    
    mixSlider.addListener(this);
    addAndMakeVisible(mixSlider);
    
    addAndMakeVisible(mixLabel);
    mixLabel.setText("Wet/Dry Mix", dontSendNotification);
    mixLabel.attachToComponent(&mixSlider, false);
    
    freqSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 80, 12);
    freqSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    freqSlider.setRange(20.0f, 1000.0f);
    freqSlider.setTextValueSuffix("Hz");
    
    freqSlider.addListener(this);
    addAndMakeVisible(freqSlider);
    freqSlider.setValue(60.0f);
    
    addAndMakeVisible(freqLabel);
    freqLabel.setText("Set Freq", dontSendNotification);
    freqLabel.attachToComponent(&freqSlider, false);
}

Govers_a2AudioProcessorEditor::~Govers_a2AudioProcessorEditor()
{
}

//==============================================================================
void Govers_a2AudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::darkorchid);

    g.setColour (Colours::aqua);
    g.setFont (15.0f);
    g.drawFittedText ("GOVERSA2 PLUGIN", getLocalBounds(), Justification::centred, 1);
    
    g.setColour(Colours::black);
    g.setFont(Font ("Arial", 20.0f, Font::italic));
    g.drawFittedText("Random Test Font! Hi There!", 10, 10, getWidth(), 50,
        Justification::left, 1);
}

void Govers_a2AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    // setting slider placement
    mixSlider.setBounds(10, getHeight()-100, 80, 80);
    
    freqSlider.setBounds(10, getHeight()-225, 100, 100);
}

void Govers_a2AudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == &mixSlider)
    {
        
        // get the value on the current slider, pass to audio processor
        processor.mixLevel.setTargetValue(mixSlider.getValue());
        
    }
    
    else if (slider ==&freqSlider)
    {
        
        processor.freqLevel.setTargetValue(freqSlider.getValue());
    }
}
