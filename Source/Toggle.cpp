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

#include "../Toggle.h"

namespace jdo
{

Toggle::Toggle (AudioProcessorParameter& p)
    : ToggleButton (p.getName (256)),
      param (p)
{
    startTimerHz (30);
    updateToggle();
}

// private:
void Toggle::clicked()
{
    param.setValueNotifyingHost (static_cast<float> (getToggleState()));
}

void Toggle::timerCallback()
{
    updateToggle();
}

void Toggle::updateToggle()
{
    const bool newState = (param.getValue() >= 0.5f);

    if (newState != getToggleState() && ! isMouseButtonDown())
        setToggleState (newState, NotificationType::dontSendNotification);
}

//==============================================================================
} // namespace jdo
