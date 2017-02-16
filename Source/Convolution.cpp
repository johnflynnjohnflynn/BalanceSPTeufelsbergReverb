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

#include <cassert>
#include "../Dependencies/gsl.h"
#include "../Convolution.h"
#include "../Utility.h"
#include "../Buffer.h"
#include "../Resampling.h"

namespace ado
{

//==============================================================================
                                                           // how/where do we check imp chans==sig chans?
Convolver::Convolver(const Buffer& impulse, int convolvedLength) // length should be imp + sig - 1
    : imp {impulse},
      sigFft     {nextPowerOf2Order(convolvedLength), false}, // false==forward fft
      impFft     {nextPowerOf2Order(convolvedLength), false},
      inverseFft {nextPowerOf2Order(convolvedLength), true}   // true==ifft
{
    const int zeroPaddedfftLength = ado::nextPowerOf2 (convolvedLength);        // zero pad bufs
    sigFftBuf.clearAndResize (imp.getNumChannels(), zeroPaddedfftLength * 2);   // (* 2 for imaginary too)
    impFftBuf.clearAndResize (imp.getNumChannels(), zeroPaddedfftLength * 2);
    
    impFftBuf.copyFrom(impulse);                                                // pre-fft imp
    for (int c = 0; c < impFftBuf.getNumChannels(); ++c)
        impFft.performRealOnlyForwardTransform (impFftBuf.getWriteArray()[c]);
    //ado::coutBuffer(impFftBuf);
}

void Convolver::convolve (const ado::Buffer& signal, ado::Buffer& out)
{
    sigFftBuf.copyFrom(signal);                                                 // fft sig
    for (int c = 0; c < sigFftBuf.getNumChannels(); ++c)
        sigFft.performRealOnlyForwardTransform (sigFftBuf.getWriteArray()[c]);
    //ado::coutBuffer(sigFftBuf);
                                                                                            // MAKE HELPER!!!
    complexMultiplyInterleavedBuffers();                                        // freq domain multiply
    //ado::coutBuffer(sigFftBuf);

    for (int c = 0; c < sigFftBuf.getNumChannels(); ++c)                        // inverse fft
        inverseFft.performRealOnlyInverseTransform (sigFftBuf.getWriteArray()[c]);
    //ado::coutBuffer(sigFftBuf);

    for (int c = 0; c < out.getNumChannels(); ++c)                              // copy to out
        for (int s = 0; s < out.getNumSamples(); ++s)
            out.getWriteArray()[c][s] = sigFftBuf.getReadArray()[c][s];
    //ado::coutBuffer(out);
}

//private:

void Convolver::complexMultiplyInterleavedBuffers()
{
    for (int c = 0; c < sigFftBuf.getNumChannels(); ++c)            // freq domain complex multiply
        for (int s = 0; s < sigFftBuf.getNumSamples(); s += 2)
        {
            const float x1 = sigFftBuf.getReadArray()[c][s];
            const float y1 = sigFftBuf.getReadArray()[c][s+1];
            const float x2 = impFftBuf.getReadArray()[c][s];
            const float y2 = impFftBuf.getReadArray()[c][s+1];
            const float outreal = x1*x2 - y1*y2;    // see JOS MDFT p.12
            const float outimag = x1*y2 + y1*x2;
            sigFftBuf.getWriteArray()[c][s]   = outreal;
            sigFftBuf.getWriteArray()[c][s+1] = outimag;
        }
}


//==============================================================================

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
        irResampled = ado::resampleBuffer (irOriginal, static_cast<int> (sampleRate));

        const float scale = static_cast<float> (irOriginal.getSampleRate() / sampleRate); // more samples convolved = louder!
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

//==============================================================================
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
