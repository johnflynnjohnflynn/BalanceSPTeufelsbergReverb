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

#include "Judio/UtilityBar.h"

namespace jdo
{

//==============================================================================
UtilityBarComponent::UtilityBarComponent (jdo::StateAB& sab, jdo::StatePresets& sp, AudioProcessor& p)
    : stateComponent {sab, sp},
      gainStepSizeSlider {*p.getParameters()[0]},                           // better way than indices?
      freqStepSizeSlider {*p.getParameters()[1]}
{
    addAndMakeVisible (stateComponent);

    gainStepSizeSlider.setSliderStyle (Slider::SliderStyle::LinearBar); // override default rotary
    freqStepSizeSlider.setSliderStyle (Slider::SliderStyle::LinearBar);

    gainStepSizeSlider.setColour (Slider::backgroundColourId, Colour (0xff808080));     // move to custom Look???
    gainStepSizeSlider.setColour (Slider::thumbColourId, Colour (0xff606060));
    freqStepSizeSlider.setColour (Slider::backgroundColourId, Colour (0xff808080));
    freqStepSizeSlider.setColour (Slider::thumbColourId, Colour (0xff606060));

    addAndMakeVisible (gainStepSizeSlider);
    addAndMakeVisible (freqStepSizeSlider);
}

void UtilityBarComponent::resized()
{
    auto r (getLocalBounds());

    const int componentWidth {r.getWidth() / 8};

    stateComponent.setBounds (r.removeFromLeft (componentWidth * 6));

    gainStepSizeSlider.setBounds (r.removeFromLeft (componentWidth).reduced (2));
    freqStepSizeSlider.setBounds (r.reduced (2));
}

//==============================================================================
} // namespace jdo
