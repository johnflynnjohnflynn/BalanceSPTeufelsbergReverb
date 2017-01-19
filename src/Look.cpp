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
    width > height ?                    // Centre knob in largest square that fits in slider rectangle
        (x = (width - height) / 2)
       : y = (height - width) / 2;

    const int smallestSide {jmin (width, height)};
    const float scaleFactor = static_cast<float> (smallestSide) / knob.getWidth();

    g.drawImageTransformed (knob, AffineTransform{}.scaled (scaleFactor)
                                                   .translated (x, y));

        // Calculate rotation
    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    const float rotationCentre = knob.getWidth() / 2;
    AffineTransform rotationTransform;
    rotationTransform = rotationTransform.rotation (angle, rotationCentre, rotationCentre);

    g.drawImageTransformed (knobMarker, rotationTransform.scaled (scaleFactor)
                                                         .translated (x, y));
}

Slider::SliderLayout CustomLook::getSliderLayout (Slider& slider)
{
    Slider::SliderLayout layout;

    layout.sliderBounds = slider.getLocalBounds();

    const int h = slider.getHeight();
    const int w = slider.getWidth();

    const int smallestSide {jmin (w, h)};

    layout.textBoxBounds = Rectangle<int> {w / 2 + 11,                      // x pos
                                          (h / 2 - smallestSide / 2) + 4,   // y pos
                                           40,                              // box width
                                           12};                             // box height

    return layout;
}

//==============================================================================
static void drawButtonShape (Graphics& g, const Path& outline, Colour baseColour, float height)
{
    //const float mainBrightness = baseColour.getBrightness();
    //const float mainAlpha = baseColour.getFloatAlpha();

    g.setGradientFill (ColourGradient (baseColour.brighter (0.025f), 0.0f, 0.0f,
                                       baseColour.darker (0.025f), 0.0f, height, false));
    g.fillPath (outline);

        // Don't draw outline!
    //g.setColour (Colours::white.withAlpha (0.4f * mainAlpha * mainBrightness * mainBrightness));
    //g.strokePath (outline, PathStrokeType (1.0f), AffineTransform::translation (0.0f, 1.0f)
    //                                                    .scaled (1.0f, (height - 1.6f) / height));
    //g.setColour (Colours::black.withAlpha (0.4f * mainAlpha));
    //g.strokePath (outline, PathStrokeType (1.0f));
}

void CustomLook::drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour,
                                           bool isMouseOverButton, bool isButtonDown)
{
    Colour baseColour (backgroundColour.withMultipliedSaturation (button.hasKeyboardFocus (true) ? 1.1f : 1.0f)
                                       .withMultipliedAlpha (button.isEnabled() ? 0.8f : 0.7f));

    if (isButtonDown || isMouseOverButton)
        baseColour = baseColour.contrasting (isButtonDown ? 0.2f : 0.1f);

    const bool flatOnLeft   = button.isConnectedOnLeft();
    const bool flatOnRight  = button.isConnectedOnRight();
    const bool flatOnTop    = button.isConnectedOnTop();
    const bool flatOnBottom = button.isConnectedOnBottom();

    const float width  = button.getWidth() - 1.0f;
    const float height = button.getHeight() - 1.0f;

    if (width > 0 && height > 0)
    {
        const float cornerSize = 4.0f;

        Path outline;
        outline.addRoundedRectangle (0.5f, 0.5f, width, height, cornerSize, cornerSize,
                                     ! (flatOnLeft  || flatOnTop),
                                     ! (flatOnRight || flatOnTop),
                                     ! (flatOnLeft  || flatOnBottom),
                                     ! (flatOnRight || flatOnBottom));

        drawButtonShape (g, outline, baseColour, height);
    }
}

} // namespace
