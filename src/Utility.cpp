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

#include "Judio/Utility.h"

namespace jdo
{

//--------//--------//--------//--------//--------//--------//--------//--------
/** 
    Next power of two integer larger than x
*/
int nextPowerOf2 (int x)
{
    int nextPO2 {1};
    while (nextPO2 < x)
        nextPO2 *= 2;

    return nextPO2;
}

} // namespace
