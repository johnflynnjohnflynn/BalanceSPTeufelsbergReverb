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

#include "WDL/resample.h"
#include "Resample.h"

namespace ado
{

ado::Buffer resampleBuffer (ado::Buffer buffer, double sourceRate, double destRate)
{
    WDL_Resampler engine;

    engine.SetMode (false, 0, true); // sinc, default size (interp, filtercnt, sinc)
    engine.SetRates (sourceRate, destRate);

    int sourceLength = buffer.numSamples();
    int destLength = sourceLength * (destRate / sourceRate);

    ado::Buffer destBuff {buffer.numChannels(), destLength};

    if (destLength == sourceLength)            // copy and exit
    {
        destBuff = buffer;
        return destBuff;
    }

    for (int chan = 0; chan < buffer.numChannels(); ++chan)
    {
        float* source = buffer.getWriteArray()[chan];
        float* dest = destBuff.getWriteArray()[chan];

        float* p;                                       // needs to work this way
        engine.ResamplePrepare (destLength, 1, &p);
        for (int i = 0; i < sourceLength; ++i)
            *p++ = *source++;

        engine.ResampleOut (dest, sourceLength, destLength, 1);
        
        engine.Reset(); // don't carry garbage over to next channel pass
    }

    return destBuff;
}

} // namespace
