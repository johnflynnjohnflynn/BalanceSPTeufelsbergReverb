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

#include "Judio/Look.h"

namespace jdo
{

//--------//--------//--------//--------//--------//--------//--------//--------

void CustomLook::drawRotarySlider (Graphics& g,
                                         int x,                 // offset position
                                         int y,
                                         int width,
                                         int height,
                                         float sliderPos,       // 0to1,
                                         float rotaryStartAngle,
                                         float rotaryEndAngle,
                                         Slider& slider)
{
    //g.fillAll(Colour (0xff404040));                                                                 // REMOVE THIS!

    width > height ? (x = (width - height) / 2) : y = (height - width) / 2; // set the slider centred in the
                                                                            // largest square that will
                                                                            // fit in the rectangle

    const float scaleFactor = static_cast<float> (width) / knob.getWidth();

    g.drawImageTransformed (knob, AffineTransform{}.scaled (scaleFactor)
                                                   .translated (x, y));

    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    const float rotationCentre = knob.getWidth() / 2;
    AffineTransform rotationTransform;
    rotationTransform = rotationTransform.rotation (angle, rotationCentre, rotationCentre);

    g.drawImageTransformed (knobMarker, rotationTransform.scaled (scaleFactor)
                                                         .translated (x, y));
}

//==============================================================================
Slider::SliderLayout CustomLook::getSliderLayout (Slider& slider)
{
    Slider::SliderLayout layout;

    layout.sliderBounds = slider.getLocalBounds();

    const int h = slider.getHeight();
    const int w = slider.getWidth();

    layout.textBoxBounds = Rectangle<int> {w / 2 + 5,  // x pos
                                           static_cast<int> (h / 2 - h / 3.5), // y pos
                                           50,  // box width
                                           12}; // box height

    return layout;
}

} // namespace
