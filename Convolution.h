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

#ifndef CONVOLUTION_H_INCLUDED
#define CONVOLUTION_H_INCLUDED

#include "Aidio/Buffer.h"
#include "Aidio/Utility.h"
#include "Aidio/Dependencies/WDL/convoengine.h"


namespace ado
{

//==============================================================================
/**
*/
class Convolver
{
public:                                                             // how/where do we check imp chans==sig chans?
    Convolver(const Buffer& impulse, int convolvedLength) // length should be imp + sig - 1
        : imp {impulse},
          sigFft     {nextPowerOf2Order(convolvedLength), false}, // false==forward fft
          impFft     {nextPowerOf2Order(convolvedLength), false},
          inverseFft {nextPowerOf2Order(convolvedLength), true}
    {
        const int zeroPaddedfftLength = ado::nextPowerOf2 (convolvedLength);        // pad bufs
        sigFftBuf.clearAndResize (imp.getNumChannels(), zeroPaddedfftLength * 2);
        impFftBuf.clearAndResize (imp.getNumChannels(), zeroPaddedfftLength * 2);
        
        impFftBuf.copyFrom(impulse);                                                // pre-fft imp
        for (int c = 0; c < impFftBuf.getNumChannels(); ++c)
            impFft.performRealOnlyForwardTransform (impFftBuf.getWriteArray()[c]);
        //ado::coutBuffer(impFftBuf);
    }
    ~Convolver() {}

    void convolve (const ado::Buffer& signal, ado::Buffer& out)
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

private:
    const ado::Buffer& imp;

    ado::Buffer sigFftBuf {1,1}; // zero-padded scratchpad buffers
    ado::Buffer impFftBuf {1,1};
    juce::FFT sigFft;
    juce::FFT impFft;
    juce::FFT inverseFft;

    void complexMultiplyInterleavedBuffers()
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
};

//==============================================================================
/** Simple, zero latency convolution engine built on WDL.
    
    @example    ado::Buffer ir {1, 1};      // member variables
                ado::Convolution engine {ir};
                                            
                                            // put in constructor
                jdo::bufferLoadFromWavBinaryData (BinaryData::IR_wav,
                                                  BinaryData::IR_wavSize,
                                                  ir, 
                                                  44100);
                engine.set (ir);
                
                engine.reset (sampleRate);  // put in prepareToPlay()
                
                                            // put in processBlock()
                engine.process (buffer.getArrayOfWritePointers(),
                                buffer.getNumChannels(),
                                buffer.getNumSamples());
    
    Notes: 
    - Max 4 channels!!!
    - If impulse has same data in all channels, WDL treats signal as mono and
      won't work > 2 channels. Watch for this!

*/
class Convolution
{
public:
    explicit Convolution (const ado::Buffer& impulse);                              
    ~Convolution() {}

    Convolution (const Convolution&) = delete;     // disable copying & move
    Convolution& operator=(const Convolution&) = delete;
    Convolution (Convolution&&) = delete;
    Convolution& operator=(Convolution&&) = delete;

    void set (const ado::Buffer& impulse);

    void resampleIrOnRateChange (double sampleRate);

    void process (ado::Buffer& block);
    void process (float** block, int blockNumChannels, int blockNumSamples);

private:
    void convolve (float** block, int blockNumChannels, int blockNumSamples);

    double lastSampleRate;

    const ado::Buffer& irOriginal;
          ado::Buffer  irResampled {1, 1};

    WDL_ImpulseBuffer imp;
    WDL_ConvolutionEngine_Div eng;
};

} // namespace

#endif  // CONVOLUTION_H_INCLUDED
