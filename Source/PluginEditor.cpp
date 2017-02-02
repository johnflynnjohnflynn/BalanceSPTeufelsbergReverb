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
    : AudioProcessorEditor {&p},
      stateComponent {p.stateAB, p.statePresets}, //*p.getParameters()[0]},     // move bypass to State?
      bypassToggle     {*p.getParameters()[Processor::ParamNames::bypassName]},
      reverbTypeSlider {*p.getParameters()[Processor::ParamNames::reverbTypeName]},
      mixSlider        {*p.getParameters()[Processor::ParamNames::mixName]},
      gainSlider       {*p.getParameters()[Processor::ParamNames::gainName]},
      backgroundImage {ImageCache::getFromMemory (BinaryData::layout04NoKnobsfs8_png,
                                                  BinaryData::layout04NoKnobsfs8_pngSize)},
      versionNumberLabel {"LabelID", "v" + String {ProjectInfo::versionString}},
      processor {p}
{
    addAndMakeVisible (&stateComponent);

    addAndMakeVisible (&bypassToggle);

    reverbTypeSlider.setSliderStyle (Slider::SliderStyle::LinearBar);
    reverbTypeSlider.setVelocityBasedMode (false);
    addAndMakeVisible (&reverbTypeSlider);

    addAndMakeVisible (&mixSlider);
    addAndMakeVisible (&gainSlider);

    versionNumberLabel.setColour (Label::textColourId, Colour {0xff575757});
    addAndMakeVisible (&versionNumberLabel);

    setSize (550, 500); // remember to set before xtor finished
}

Editor::~Editor()
{
}

//==============================================================================
void Editor::paint (Graphics& g)
{
    g.drawImage (backgroundImage, 0, 0, 1000, 500,  // xPos, yPos, xSize, ySize
                                  0, 0, 2000, 1000);

    versionNumberLabel.setBounds(213, 34, 100, 10);
}

void Editor::resized()
{
    stateComponent.setBounds (0, 0, getWidth(), getHeight());

    reverbTypeSlider.setBounds (24, 134, 166, jdo::CustomLook::buttonHeight);

    mixSlider .setBounds (38, 172, 135, 135);
    gainSlider.setBounds (38, 308, 135, 135);

    bypassToggle.setBounds (466, 9,
                            jdo::CustomLook::buttonWidth + 16,
                            jdo::CustomLook::buttonLargeHeight + 8);
}
