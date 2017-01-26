/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Editor::Editor (Processor& p)
    : AudioProcessorEditor (&p),
      stateComponent {p.stateAB, p.statePresets}, //*p.getParameters()[0]},
      //gainStepSizeSlider {*p.getParameters()[0]},                             // these now live in State
      //freqStepSizeSlider {*p.getParameters()[1]},
      bypassToggle         {*p.getParameters()[0]},
      reverbTypeSlider     {*p.getParameters()[1]},
      mixSlider            {*p.getParameters()[2]},                             // better way than indices?
      gainSlider           {*p.getParameters()[3]},
      processor (p)
{
    addAndMakeVisible (&stateComponent);

    addAndMakeVisible (&bypassToggle);

    reverbTypeSlider.setSliderStyle(Slider::SliderStyle::LinearBar);
    addAndMakeVisible (&reverbTypeSlider);

    addAndMakeVisible (&mixSlider);
    addAndMakeVisible (&gainSlider);

    setSize (550, 500); // remember to set before xtor finished
}

Editor::~Editor()
{
}

//==============================================================================
void Editor::paint (Graphics& g)
{
    g.drawImage (backgroundImage, 0, 0, 1000, 500, 0, 0, 2000, 1000);
}

void Editor::resized()
{
    stateComponent.setBounds(0, 0, getWidth(), getHeight());

    reverbTypeSlider.setBounds (24, 134, 166, jdo::CustomLook::buttonHeight);

    mixSlider .setBounds (38, 172, 135, 135);
    gainSlider.setBounds (38, 308, 135, 135);

    bypassToggle.setBounds (466, 9,
                            jdo::CustomLook::buttonWidth + 16,
                            jdo::CustomLook::buttonLargeHeight + 8);
}
