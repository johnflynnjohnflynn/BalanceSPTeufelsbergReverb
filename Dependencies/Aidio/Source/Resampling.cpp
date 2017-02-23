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

#include "../Dependencies/WDL/resample.h"
#include "../Resampling.h"

namespace ado
{

ado::Buffer resampleBuffer (const ado::Buffer& buffer, int destRate)
{
    int sourceRate = buffer.getSampleRate();
    int sourceLength = buffer.getNumSamples();
    const double factor = static_cast<double> (destRate) / sourceRate; // divide in double
    int destLength = static_cast<int> (sourceLength * factor);         // then round down

    ado::Buffer destBuff {buffer.getNumChannels(), destLength, destRate};

    if (destLength == sourceLength)             // copy and exit
    {
        destBuff = buffer;
        return destBuff;
    }
    
    WDL_Resampler engine;                       // else resample...
    engine.SetMode (false, 0, true); // sinc, default size (interp, filtercnt, sinc)
    engine.SetRates (sourceRate, destRate);

    for (int chan = 0; chan < buffer.getNumChannels(); ++chan)
    {
        ado::Buffer p {1, sourceLength};                    // needs to work this way
        engine.ResamplePrepare (destLength, 1, p.getWriteArray());
        for (int samp = 0; samp < sourceLength; ++samp)
            p.getWriteArray()[0][samp] = buffer.getReadArray()[chan][samp];

        float* dest = destBuff.getWriteArray()[chan];

        engine.ResampleOut (dest, sourceLength, destLength, 1);
        
        engine.Reset(); // don't carry garbage over to next channel pass
    }

    return destBuff;
}

} // namespace
