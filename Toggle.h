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

#ifndef TOGGLE_H_INCLUDED
#define TOGGLE_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Dependencies/Aidio/Aidio.h"

#include "Parameter.h"

namespace jdo
{

//==============================================================================
/** Modified version of ParameterSlider from Juce plugin demo that attaches to a
    ToggleButton.
    
    @see ParameterSlider, Slider, jdo::Parameter, jdo::SliderStep
*/
class Toggle  : public ToggleButton,
                private Timer
{
public:
    explicit Toggle (AudioProcessorParameter& p);

private:
    void clicked() override;

    void timerCallback() override;

    void updateToggle();

    AudioProcessorParameter& param;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Toggle)
};

//==============================================================================
} // namespace jdo

#endif  // TOGGLE_H_INCLUDED
