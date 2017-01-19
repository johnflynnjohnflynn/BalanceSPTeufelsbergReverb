//--------//--------//--------//--------//--------//--------//--------//--------
/*
  ==============================================================================

   This file follows the same open source licensing as the JUCE library.
   Copyright (c) 2016 John Flynn

   Permission is granted to use this software under the terms of either:
   a) the GPL v2 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   Judio is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ==============================================================================
*/
//--------//--------//--------//--------//--------//--------//--------//--------

#ifndef LOOK_H_INCLUDED
#define LOOK_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"

namespace jdo
{

//==============================================================================
/** Custom LookAndFeel class for uniform styling across plugins
    @see juce::LookAndFeel
    
    Note: Visual look of button is set here AND in Button.h !!!
    @see jdo::CustomButton
*/
class CustomLook  : public LookAndFeel_V3
{
public:
    void drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos0to1,
                           float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override;

    Slider::SliderLayout getSliderLayout (Slider& slider) override;

    //==============================================================================
    void drawButtonBackground (Graphics&, Button&, const Colour& backgroundColour,
                           bool isMouseOverButton, bool isButtonDown) override;

private:
    Image knob       {ImageCache::getFromMemory (BinaryData::layout04knob01onfs8_png,
                                                 BinaryData::layout04knob01onfs8_pngSize)};
    Image knobMarker {ImageCache::getFromMemory (BinaryData::layout04knob01dotonfs8_png,
                                                 BinaryData::layout04knob01dotonfs8_pngSize)};

};

//==============================================================================
} // namespace jdo

#endif  // LOOK_H_INCLUDED
