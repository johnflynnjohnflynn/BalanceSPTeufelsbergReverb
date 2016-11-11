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

#ifndef CONVOLUTION_H_INCLUDED
#define CONVOLUTION_H_INCLUDED

#include <cassert>
#include "gsl.h"
#include "Utility.h"
#include "Buffer.h"
#include "WDL/convoengine.h"

namespace ado
{

//--------//--------//--------//--------//--------//--------//--------//--------
/**
*/
class Convolution
{
public:
    explicit Convolution (ado::Buffer& impulse)                         // WDL_CONVO_MAX_IMPULSE_NCH max channels set to 2
    {
        imp.Set (impulse.getReadArray(), impulse.numSamples(), impulse.numChannels());

        eng.SetImpulse (&imp,
                        -1,     // fft size                             // probably should be pow2 after processBufferSize?
                        0,      // starting sample
                        0,      // max impulse size
                        true);  // brute convolution
    }
    ~Convolution() {}

    void reset() { eng.Reset(); }

    void process (ado::Buffer& block)
    {
        eng.Add (block.getWriteArray(),                      // Send input to conv eng
                 block.numSamples(),
                 block.numChannels());

        const int avail = eng.Avail (block.numSamples());    // Confirm full buffer available
        assert (avail == block.numSamples());

        float** convolved = eng.Get();

        ado::rawBufferCopy (const_cast<const float**> (convolved),  // source
                            block.getWriteArray(),                  // dest
                            block.numChannels(),
                            block.numSamples());

        eng.Advance (block.numSamples());                    // Advance the eng
    }

    WDL_ImpulseBuffer imp;
    WDL_ConvolutionEngine eng;
};

} // namespace

#endif  // CONVOLUTION_H_INCLUDED
