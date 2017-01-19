//--------//--------//--------//--------//--------//--------//--------//--------
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
//--------//--------//--------//--------//--------//--------//--------//--------

#include "Aidio/Maths.h"

namespace ado
{

//--------//--------//--------//--------//--------//--------//--------//--------

bool operator== (const RangeLog& a, const RangeLog& b)
{
    return a.getStart()    == b.getStart()
        && a.getEnd()      == b.getEnd()
        && a.getSkewLog()  == b.getSkewLog();
}

void checkRangeLog (float start, float end) // skewLog can be any value
{
    jassert (end > start);     // invariants here
}

//--------//--------//--------//--------//--------//--------//--------//--------

RangeLog::RangeLog (float rangeStart,
                    float rangeEnd,
                    float skewLogFactor)
    : start (rangeStart),
      end (rangeEnd),
      skewLog (skewLogFactor)
{
    checkRangeLog (start, end);
}

float RangeLog::convertTo0to1 (float v) const noexcept
{
    float proportion = (v - start) / (end - start);

    if (skewLog != 0.0f)
    {
        const float tenPowSkewLog = std::pow (10.0f, skewLog);
        proportion = (std::log10 ((proportion * (tenPowSkewLog - 1.0f)) + 1.0f))
                    / std::log10 (tenPowSkewLog);
    }

    return proportion;
}

float RangeLog::convertFrom0to1 (float proportion) const noexcept
{
    if (skewLog != 0.0f)
    {
        const float tenPowSkewLog = std::pow (10.0f, skewLog);
        proportion = (std::pow (tenPowSkewLog, proportion) - 1.0f)
                   / (tenPowSkewLog - 1.0f);
    }

    return start + ((end - start) * proportion);
}

void RangeLog::setStart (float newStart)
{
    checkRangeLog (newStart, end);
    start = newStart;
}

void RangeLog::setEnd (float newEnd)
{
    checkRangeLog (start, newEnd);
    end = newEnd;
}

void RangeLog::setSkewLog (float newSkewLog)
{
    checkRangeLog (start, end);
    skewLog = newSkewLog;
}

//--------//--------//--------//--------//--------//--------//--------//--------

} // namespace ado
