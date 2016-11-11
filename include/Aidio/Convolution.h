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

#include "gsl.h"
#include "Audio/Buffer.h"
#include "Audio/Utility.h"
#include "WDL/convoengine.h"

namespace ado
{

//--------//--------//--------//--------//--------//--------//--------//--------
/**
*/
class Convolution
{/*
public:
    Convolution (const ado::Buffer& impulse)                                   // WDL_CONVO_MAX_IMPULSE_NCH max channels set to 2
    {
        imp.Set (const_cast<const float**> (impulse.getWriteArray()),   // sigh, const_cast
                                            impulse.numSamples(),
                                            impulse.numChannels());
        eng.SetImpulse (&imp,
                        4,      // fft size
                        0,      // starting sample
                        8       // max impulse size
                        false); // brute convolution
    }
    ~Convolution() {}

    void reset() { engine.Reset(); }

    void process (ado::Buffer& data)
    {
        engine.Add (data.getWriteArray(),              // Send input to conv engine
                    data.numSamples(),
                    data.numChannels());

        const int avail = engine.Avail (data.numSamples());   // Confirm full buffer available
        std::cout << avail << "\n"; // debug

        float** convolved = engine.Get();

        ado::rawBufferCopy (convolved,
                            data.getWriteArray(),
                            data.numChannels(),
                            data.numSamples()); // does NOT range check!!!

        engine.Advance (data.numSamples());                   // Advance the engine
    }

    WDL_ImpulseBuffer imp;
    WDL_ConvolutionEngine eng;*/
};

} // namespace

#endif  // CONVOLUTION_H_INCLUDED
