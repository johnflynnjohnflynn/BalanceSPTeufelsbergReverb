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

#ifndef PARAMETERSLIDER_H_INCLUDED
#define PARAMETERSLIDER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Dependencies/Aidio/Aidio.h"

#include "Parameter.h"

namespace jdo
{

//==============================================================================
/** Modified version of ParameterSlider from Juce plugin demo that allows
    stepped sliders (as well as continuous).
    
    Uses numSteps member of jf::Parameter to determine steps. (Set to 0 for 
    continuous)
    
    @see ParameterSlider, Slider, jf::Parameter
*/
class SliderStep  : public Slider,
                    private Timer
{
public:
    explicit SliderStep (AudioProcessorParameter& p);

private:
    void valueChanged() override;

    void timerCallback() override;

    void startedDragging() override;
    void stoppedDragging() override;

    double getValueFromText (const String& text) override;
    String getTextFromValue (double value) override;

    void updateSliderPos();
    void updateInterval();

    AudioProcessorParameter& param;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderStep)
};

//==============================================================================
} // namespace jdo

#endif  // PARAMETERSLIDER_H_INCLUDED
