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
    Simple, zero latency convolution engine.
    
    Note: Max 2 channels!!!
*/
class Convolution
{
public:
    explicit Convolution (ado::Buffer& impulse)                 // WDL_CONVO_MAX_IMPULSE_NCH max channels set to 2
    {
        set (impulse);
    }
    ~Convolution() {}

    void set (ado::Buffer& impulse)
    {
        imp.Set (impulse.getReadArray(), impulse.numSamples(), impulse.numChannels());
        eng.SetImpulse (&imp);
    }

    void reset() { eng.Reset(); }

    void process (ado::Buffer& block)
    {
        convolve (block.getWriteArray(), block.numChannels(), block.numSamples());
    }

    void process (float** block, int blockNumChannels, int blockNumSamples)
    {
        convolve (block, blockNumChannels, blockNumSamples);
    }

private:
    void convolve (float** block, int blockNumChannels, int blockNumSamples)
    {
        eng.Add (block,                                 // Send input to conv eng
                 blockNumSamples,
                 blockNumChannels);

        const int avail = eng.Avail (blockNumSamples);  // Confirm full buffer available
        assert (avail == blockNumSamples);

        float** convolved = eng.Get();
        auto constConvolved = const_cast<const float**> (convolved);

        ado::rawBufferCopy (constConvolved,             // source
                            block,                      // dest
                            blockNumChannels,
                            blockNumSamples);

        eng.Advance (blockNumSamples);                  // Advance the eng
    }

    WDL_ImpulseBuffer imp;
    WDL_ConvolutionEngine_Div eng;
};

} // namespace

#endif  // CONVOLUTION_H_INCLUDED
