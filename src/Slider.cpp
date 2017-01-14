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

#include "Judio/Slider.h"

namespace jdo
{

SliderStep::SliderStep (AudioProcessorParameter& p)
    : Slider (p.getName (256)), param (p)
{
    setTextValueSuffix (p.getLabel());

    setSliderStyle (SliderStyle::RotaryHorizontalVerticalDrag);
    setTextBoxStyle (Slider::TextEntryBoxPosition::TextBoxBelow, false, 75, 16);

    setColour (Slider::textBoxTextColourId,       Colour (0xff373737));                     // Possible move to custom Look???
    setColour (Slider::textBoxBackgroundColourId, Colour (0xff808080));
    setColour (Slider::textBoxHighlightColourId,  Colour (0xffffffff));
    setColour (Slider::textBoxOutlineColourId,    Colour (0x00000000));

    if (1 <= p.getNumSteps() && p.getNumSteps() <= 100) // <=100? defaults to 37850784 steps!
    {
        const double normStepSize = 1.0 / (p.getNumSteps());
        setRange (0.0, 1.0, normStepSize); // stepped
    }
    else
        setRange (0.0, 1.0, 0.0); // continuous

    startTimerHz (30);
    updateSliderPos();
}

// private:
void SliderStep::valueChanged()
{
    param.setValueNotifyingHost ((float) Slider::getValue());
}

void SliderStep::timerCallback()
{
    updateInterval();
    updateSliderPos();
    updateText();
}

void SliderStep::startedDragging()     { param.beginChangeGesture(); }
void SliderStep::stoppedDragging()     { param.endChangeGesture();   }

double SliderStep::getValueFromText (const String& text)   { return param.getValueForText (text); }
String SliderStep::getTextFromValue (double value)         { return param.getText ((float) value, 1024); }

void SliderStep::updateSliderPos()
{
    const float newValue = param.getValue();

    if (newValue != (float) Slider::getValue() && ! isMouseButtonDown())
        Slider::setValue (newValue);
}
void SliderStep::updateInterval()
{
    if (jdo::ParamStepListenFreq* pslf = dynamic_cast<jdo::ParamStepListenFreq*> (&param))
    {
        const int numStepsParam = pslf->getNumSteps();
        const float newSliderInterval0to1 = 1.0f / numStepsParam;

        Slider::setRange (0, 1, newSliderInterval0to1);
    }
}

//==============================================================================
} // namespace jdo
