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
      utilityComponent {p.stateAB, p.statePresets, p},
      //gainStepSizeSlider {*p.getParameters()[0]},                             // these now live in UtilityComponent
      //freqStepSizeSlider {*p.getParameters()[1]},
      gain1Slider        {*p.getParameters()[2]},                               // better way than indices?
      freq1Slider        {*p.getParameters()[3]}, //freqStepSizeSlider.slider_},
      q1Slider           {*p.getParameters()[4]},
      gain2Slider        {*p.getParameters()[5]},
      freq2Slider        {*p.getParameters()[6]},
      q2Slider           {*p.getParameters()[7]},
      gain3Slider        {*p.getParameters()[8]},
      freq3Slider        {*p.getParameters()[9]},
      q3Slider           {*p.getParameters()[10]},
      gain4Slider        {*p.getParameters()[11]},
      freq4Slider        {*p.getParameters()[12]},
      q4Slider           {*p.getParameters()[13]},
      gain5Slider        {*p.getParameters()[14]},
      freq5Slider        {*p.getParameters()[15]},
      q5Slider           {*p.getParameters()[16]},
      gain6Slider        {*p.getParameters()[17]},
      freq6Slider        {*p.getParameters()[18]},
      q6Slider           {*p.getParameters()[19]},
      gain7Slider        {*p.getParameters()[20]},
      freq7Slider        {*p.getParameters()[21]},
      q7Slider           {*p.getParameters()[22]},
      backgroundImage {ImageCache::getFromMemory (BinaryData::BalanceEQ_02fs8_png,
                                                  BinaryData::BalanceEQ_02fs8_pngSize)},
      processor (p)
{
    addAndMakeVisible (&utilityComponent);

    addAndMakeVisible (&gain1Slider);
    addAndMakeVisible (&freq1Slider);
    addAndMakeVisible (&q1Slider);
    addAndMakeVisible (&gain2Slider);
    addAndMakeVisible (&freq2Slider);
    addAndMakeVisible (&q2Slider);
    addAndMakeVisible (&gain3Slider);
    addAndMakeVisible (&freq3Slider);
    addAndMakeVisible (&q3Slider);
    addAndMakeVisible (&gain4Slider);
    addAndMakeVisible (&freq4Slider);
    addAndMakeVisible (&q4Slider);
    addAndMakeVisible (&gain5Slider);
    addAndMakeVisible (&freq5Slider);
    addAndMakeVisible (&q5Slider);
    addAndMakeVisible (&gain6Slider);
    addAndMakeVisible (&freq6Slider);
    addAndMakeVisible (&q6Slider);
    addAndMakeVisible (&gain7Slider);
    addAndMakeVisible (&freq7Slider);
    addAndMakeVisible (&q7Slider);

    LookAndFeel::setDefaultLookAndFeel (&look);

    setSize (900, 500); // remember to set before xtor finished
}

Editor::~Editor()
{
}

//==============================================================================
void Editor::paint (Graphics& g)
{
    //g.drawImage (backgroundImage, 0, 0, 1000, 500, 0, 0, 2000, 1000);
    g.fillAll(Colour (0xff202020));
    //g.fillAll (Colours::white);
}

void Editor::resized()
{
    Rectangle<int> r (getLocalBounds().reduced (2));

    utilityComponent.setBounds (r.removeFromTop (25));

    r.removeFromTop (25);  // space for UI labels and logos
    r.removeFromLeft (25);

    const int rows {3};
    const int sliderHeight {r.getHeight() / rows};

    const int cols {7};
    const int sliderWidth {r.getWidth() / cols};

    const int border {1};

    auto slice1 = r.removeFromTop(sliderHeight);
    gain1Slider.setBounds (slice1.removeFromLeft (sliderWidth).reduced (border));
    gain2Slider.setBounds (slice1.removeFromLeft (sliderWidth).reduced (border));
    gain3Slider.setBounds (slice1.removeFromLeft (sliderWidth).reduced (border));
    gain4Slider.setBounds (slice1.removeFromLeft (sliderWidth).reduced (border));
    gain5Slider.setBounds (slice1.removeFromLeft (sliderWidth).reduced (border));
    gain6Slider.setBounds (slice1.removeFromLeft (sliderWidth).reduced (border));
    //gain7Slider.setBounds (slice1.reduced (border));
    gain7Slider.setBounds(-50, 50, 600, 300);

    auto slice2 = r.removeFromTop(sliderHeight);
    freq1Slider.setBounds (slice2.removeFromLeft (sliderWidth).reduced (border));
    freq2Slider.setBounds (slice2.removeFromLeft (sliderWidth).reduced (border));
    freq3Slider.setBounds (slice2.removeFromLeft (sliderWidth).reduced (border));
    freq4Slider.setBounds (slice2.removeFromLeft (sliderWidth).reduced (border));
    freq5Slider.setBounds (slice2.removeFromLeft (sliderWidth).reduced (border));
    freq6Slider.setBounds (slice2.removeFromLeft (sliderWidth).reduced (border));
    freq7Slider.setBounds (slice2.reduced (border));
    
    auto slice3 = r.removeFromTop(sliderHeight);
    q1Slider.setBounds (slice3.removeFromLeft (sliderWidth).reduced (border));
    q2Slider.setBounds (slice3.removeFromLeft (sliderWidth).reduced (border));
    q3Slider.setBounds (slice3.removeFromLeft (sliderWidth).reduced (border));
    q4Slider.setBounds (slice3.removeFromLeft (sliderWidth).reduced (border));
    q5Slider.setBounds (slice3.removeFromLeft (sliderWidth).reduced (border));
    q6Slider.setBounds (slice3.removeFromLeft (sliderWidth).reduced (border));
    q7Slider.setBounds (slice3.reduced (border));
}
