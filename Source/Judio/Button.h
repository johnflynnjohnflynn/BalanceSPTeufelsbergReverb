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

#ifndef BUTTON_H_INCLUDED_KSJDHF
#define BUTTON_H_INCLUDED_KSJDHF

#include "../../JuceLibraryCode/JuceHeader.h"

namespace jdo
{
//==============================================================================
/** Quick TextButton override to set button-side styles.
    
    Note: Visual look of button is set here AND in Look.h/.cpp !!!
    @see jdo::CustomLook
*/
class CustomButton  : public TextButton
{
public:
    CustomButton (const String& buttonName) //, const String& toolTip)          // add tooltip later
        : TextButton {buttonName} //, toolTip}
    {
        setColour (TextButton::buttonColourId,   Colour (0xff303030));
        setColour (TextButton::buttonOnColourId, Colour (0xff0000ff));  // change something sensible later
        setColour (TextButton::textColourOffId,  Colour (0xffd9d9d9));
        setColour (TextButton::textColourOnId,   Colour (0xffeeeeee));
    }
};

//==============================================================================

} // namespace

#endif  // BUTTON_H_INCLUDED_KSJDHF
