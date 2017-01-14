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
                                         int x,
                                         int y,
                                         int width,
                                         int height,
                                         float sliderPos,//sliderPos0to1,
                                         float rotaryStartAngle,
                                         float rotaryEndAngle,
                                         Slider& slider)
{
    const float radius = jmin (width / 2, height / 2) - 2.0f;
    const float centreX = x + width * 0.5f;
    const float centreY = y + height * 0.5f;
    const float rx = centreX - radius;
    const float ry = centreY - radius;
    const float rw = radius * 2.0f;
    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    const bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();

    if (radius > 12.0f)
    {
        /*if (slider.isEnabled())
            g.setColour (slider.findColour (Slider::rotarySliderFillColourId).withAlpha (isMouseOver ? 1.0f : 0.7f));
        else*/
            g.setColour (Colour (0xa0a0a0a0));

        const float thickness = 0.5f;

        {
            Path filledArc;
            filledArc.addPieSegment (rx, ry, rw, rw, rotaryStartAngle, angle, thickness);
            g.fillPath (filledArc);
        }

        Path circle;
        circle.addEllipse (rx, ry, rw, rw);
        g.fillPath (circle);

        {/*
            const float innerRadius = radius * 0.2f;
            Path p;
            p.addTriangle (-innerRadius, 0.0f,
                           0.0f, -radius * thickness * 1.1f,
                           innerRadius, 0.0f);

            p.addEllipse (-innerRadius, -innerRadius, innerRadius * 2.0f, innerRadius * 2.0f);

            g.fillPath (p, AffineTransform::rotation (angle).translated (centreX, centreY));
        */}

        if (slider.isEnabled())
            g.setColour (slider.findColour (Slider::rotarySliderOutlineColourId));
        else
            g.setColour (Colour (0x80808080));

        /*Path outlineArc;
        outlineArc.addPieSegment (rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, thickness);
        outlineArc.closeSubPath();

        g.strokePath (outlineArc, PathStrokeType (slider.isEnabled() ? (isMouseOver ? 2.0f : 1.2f) : 0.3f));*/
    }
    else
    {
        if (slider.isEnabled())
            g.setColour (slider.findColour (Slider::rotarySliderFillColourId).withAlpha (isMouseOver ? 1.0f : 0.7f));
        else
            g.setColour (Colour (0x80808080));

        Path p;
        p.addEllipse (-0.4f * rw, -0.4f * rw, rw * 0.8f, rw * 0.8f);
        PathStrokeType (rw * 0.1f).createStrokedPath (p, p);

        p.addLineSegment (Line<float> (0.0f, 0.0f, 0.0f, -radius), rw * 0.2f);

        g.fillPath (p, AffineTransform::rotation (angle).translated (centreX, centreY));
    }
/*
    // const bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();

    jassert (knobMarker.getHeight() > knobMarker.getWidth()); // vertical strip

    const int knobSourceSize  {knobMarker.getWidth()};

    const int numFrames      {knobMarker.getHeight() / knobMarker.getWidth()};
    const int frameToDisplay {static_cast<int> (sliderPos0to1 * (numFrames - 1))};

    const int centreX {x + (width / 2)};            // x is numbox offset
    const int centreY {y + (height / 2)};
    const int knobDrawSize {jmin (width, height)};
    const int destX {centreX - (knobDrawSize / 2)};
    const int destY {centreY - (knobDrawSize / 2)};

    g.drawImage (knobMarker,
                 destX,                             // dest x
                 destY,                             // dest y
                 knobDrawSize,                      // dest w
                 knobDrawSize,                      // dest h
                 0,                                 // source x
                 frameToDisplay * knobSourceSize,   // source y (drawing offset)
                 knobSourceSize,                    // source w
                 knobSourceSize);                   // source h

    g.drawImage (knob,
                 destX,                             // dest x
                 destY,                             // dest y
                 knobDrawSize,                      // dest w
                 knobDrawSize,                      // dest h
                 0,                                 // source x
                 0,                                 // source y
                 knobSourceSize,                    // source w
                 knobSourceSize);                   // source h
                */
}

} // namespace
