/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class Govers_a2AudioProcessorEditor  : public AudioProcessorEditor, Slider::Listener
{
public:
    Govers_a2AudioProcessorEditor (Govers_a2AudioProcessor&);
    ~Govers_a2AudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:

 
    Slider mixSlider;
    Label mixLabel;
    
    Slider freqSlider;
    Label freqLabel;
    
    void sliderValueChanged(Slider * slider) override;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Govers_a2AudioProcessor& processor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Govers_a2AudioProcessorEditor)
};
