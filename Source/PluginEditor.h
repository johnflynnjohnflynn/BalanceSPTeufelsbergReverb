/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Judio/Judio.h"

//==============================================================================
/**
*/
class Editor  : public AudioProcessorEditor
{
public:
    explicit Editor (Processor&);
    ~Editor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    jdo::UtilityBarComponent utilityComponent;

    jdo::SliderStep gain1Slider;
    jdo::SliderStep freq1Slider;
    jdo::SliderStep q1Slider;
    jdo::SliderStep gain2Slider;
    jdo::SliderStep freq2Slider;
    jdo::SliderStep q2Slider;
    jdo::SliderStep gain3Slider;
    jdo::SliderStep freq3Slider;
    jdo::SliderStep q3Slider;
    jdo::SliderStep gain4Slider;
    jdo::SliderStep freq4Slider;
    jdo::SliderStep q4Slider;
    jdo::SliderStep gain5Slider;
    jdo::SliderStep freq5Slider;
    jdo::SliderStep q5Slider;
    jdo::SliderStep gain6Slider;
    jdo::SliderStep freq6Slider;
    jdo::SliderStep q6Slider;
    jdo::SliderStep gain7Slider;
    jdo::SliderStep freq7Slider;
    jdo::SliderStep q7Slider;

    Image backgroundImage;

    jdo::CustomLook look;

    Processor& processor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Editor)
};

#endif  // PLUGINEDITOR_H_INCLUDED
