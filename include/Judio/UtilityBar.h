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

#ifndef UTILITYBAR_H_INCLUDED
#define UTILITYBAR_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"
#include "Aidio/Aidio.h"
#include "Judio/State.h"
#include "Judio/Slider.h"

namespace jdo
{

//==============================================================================
/** A utility bar for settings and preset handling. Lives at the top of the UI.

    Combines StateComponent for preset and AB handling with 
    stepSize/freqDivision sliders.
*/
class UtilityBarComponent  : public Component
{
public:
    UtilityBarComponent (jdo::StateAB& sab,
                         jdo::StatePresets& sp,
                         AudioProcessor& p);
    
    void paint (Graphics&) override {}
    void resized() override;

private:
    jdo::StateComponent stateComponent;

    jdo::SliderStep gainStepSizeSlider;
    jdo::SliderStep freqStepSizeSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UtilityBarComponent);
};

//==============================================================================
} // namespace jdo

#endif  // UTILITYBAR_H_INCLUDED
