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

#include "Convolution.h"
#include <cassert>
#include "gsl.h"
#include "Utility.h"
#include "Buffer.h"
#include "Resampling.h"

namespace ado
{

//--------//--------//--------//--------//--------//--------//--------//--------

Convolution::Convolution (const ado::Buffer& impulse)
    : lastSampleRate {static_cast<double> (impulse.getSampleRate())},
      irOriginal {impulse}
{
    set (irOriginal);
}

void Convolution::set (const ado::Buffer& impulse)
{
    imp.Set (impulse.getReadArray(), impulse.getNumSamples(), impulse.getNumChannels());
    eng.SetImpulse (&imp);
}

void Convolution::resampleIrOnRateChange (double sampleRate)
{
    eng.Reset();

    if (sampleRate != lastSampleRate)
    {
        irResampled = ado::resampleBuffer (irOriginal, sampleRate);

        const double scale = irOriginal.getSampleRate() / sampleRate; // more samples convolved = louder!
        irResampled *= scale;

        set (irResampled);

        lastSampleRate = sampleRate;
    }
}

void Convolution::process (ado::Buffer& block)
{
    convolve (block.getWriteArray(), block.getNumChannels(), block.getNumSamples());
}

void Convolution::process (float** block, int blockNumChannels, int blockNumSamples)
{
    convolve (block, blockNumChannels, blockNumSamples);
}

//private:
void Convolution::convolve (float** block, int blockNumChannels, int blockNumSamples)
{
    eng.Add (block,                                 // Send input to conv eng
             blockNumSamples,
             blockNumChannels);

    const int avail = eng.Avail (blockNumSamples);  // Confirm full buffer available
    assert (avail == blockNumSamples);

    float** convolved = eng.Get();
    auto constConvolved = const_cast<const float**> (convolved);

    ado::rawBufferCopy (constConvolved,
                        block,
                        blockNumChannels,
                        blockNumSamples);

    eng.Advance (blockNumSamples);                  // Advance the eng
}

} // namespace
