//==============================================================================
/*
    The MIT License (MIT)

    Copyright (c) 2016 John Flynn

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
    deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/
//==============================================================================

#ifndef RANGE_H_INCLUDED
#define RANGE_H_INCLUDED

#include "Dependencies/JuceLibraryCode/JuceHeader.h"
#include "Test.h"                               // redefine jassert for tests!

namespace ado
{

//==============================================================================
/** Modifies JUCE's own NormalisableRange to allow more complex logarithmic
    skew factor. (Not templated, just float.)

    @see juce::NormalisableRange

    Uses a more computationally expensive calculation than JUCE's skew
    but is more accurate for things like decade based frequency scales.

    A factor of 0.0 will be a straight line x=y, (skips calculation)
    
    A factor of 1.0 will have 1 log decade per scale, 2.0 will give 2 etc. 
    A factor of -1.0 will be the inverse of +1.0.

    For a 3 decade Hz frequency scale, start and end should be 20 and 20000 
    respectively and skewLog should be set to 3.0.
*/
class RangeLog
{
public:
    RangeLog (float rangeStart,
              float rangeEnd,
              float skewLogFactor);

    float convertTo0to1 (float v) const noexcept;

    float convertFrom0to1 (float proportion) const noexcept;

    float getStart() const noexcept { return start; }
    void setStart (float newStart);

    float getEnd() const noexcept { return end; }
    void setEnd (float newEnd);

    float getSkewLog() const noexcept { return skewLog; }
    void setSkewLog (float newSkewLog);

private:
    float start;
    float end;
    float skewLog;
};

//==============================================================================
bool operator== (const RangeLog& a, const RangeLog& b);

//==============================================================================
} // namespace ado

#endif  // MATHS_H_INCLUDED
