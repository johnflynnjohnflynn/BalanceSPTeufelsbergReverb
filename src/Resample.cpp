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

    const float* src = buffer.getReadArray()[0];                        // hmmmmmmmmmmmmm [0]
    int src_len = buffer.numSamples();

    int dest_len = src_len * (destRate / sourceRate);

    ado::Buffer destBuff {1, dest_len};
    float* dest = destBuff.getWriteArray()[0];

    if (dest_len == src_len)
    {
        for (int i = 0; i < dest_len; ++i) // Copy
            *dest++ = *src++;
        return destBuff;
    }

    const int mBlockLength {64};
    WDL_Resampler engine;

    engine.SetMode (false, 0, true);  // Sinc, default size
    engine.SetFeedMode (true);        // Input driven
    engine.SetRates (sourceRate, destRate);

    while (dest_len > 0)
    {
        float* p;
        int n = engine.ResamplePrepare(mBlockLength, 1, &p);
        int m = n;
        if (n > src_len)
            n = src_len;
        for (int i = 0; i < n; ++i)
            *p++ = *src++;
        if (n < m)
            memset(p, 0, (m - n) * sizeof(float));
        src_len -= n;

        float buf[mBlockLength];
        n = engine.ResampleOut(buf, m, m, 1);
        if (n > dest_len)
            n = dest_len;
        p = buf;
        for (int i = 0; i < n; ++i)
            *dest++ = *p++;
        dest_len -= n;
    }
    engine.Reset();

    return destBuff;
}

} // namespace
