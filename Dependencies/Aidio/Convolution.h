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

#include <vector>
#include <cmath>

#include "Aidio/Buffer.h"
#include "Aidio/Utility.h"
#include "Aidio/Dependencies/WDL/convoengine.h"


namespace ado
{

//==============================================================================
/** Simple, zero latency convolution engine.

--- during reset
    - pre-fft ir

--- for each block
    - fft in-block (once)
    - copy to in circbuffer

--- for each slice
    - complex multiply
    - ifft
    - sum into out circbuffer

--- for each block
    - output out-block
    - clear outputted blocks in circbuffer

How many sigFftBufs do we need ?!?!?!
Surely not 8192 * BlocksPerIr == ~120 * IR size !!!

* /
class Convolver
{
public:
    Convolver (const ado::Buffer& impulse);     // slices & pre-ffts impulse to impFfts
    ~Convolver() {}

    bool reset (double samplerate);             // resamples then slices/pre-ffts impulse again

    bool convolve (ado::Buffer& blockInOut);    // sends input block to threaded convolution.
                                                // gets current block's summed, convolved output
//private:
    const ado::Buffer& ir;
    ado::Buffer irResampled;

    ado::Buffer inCircBuf;                  // size IR.         large. heap. pre-allocated. ado::Buffer?
    std::vector<ado::Buffer> impFftBufs;    // size 2 * IR.     large. heap. pre-allocated.
    std::vector<ado::Buffer> sigFftBufs;    // 8192*blocksPerIr large. heap. pre-allocated. size 2 * largestSlice * blocksPerIr ??? ???
    ado::Buffer outCircBuf;                 // size IR.         large. heap. pre-allocated. ado::Buffer?

    //==============================================================================
    /** once during reset * /
    bool resampleImpulseIfRateHasChanged (double newSampleRate);
    bool sliceImpulseToImpFftBufs();
    bool preFftImpFftBufs();

    //==============================================================================
    /** for each block * /
    bool fftInputBlockToSigFftBufs();       using pointer = float*; // for now...
    pointer copyInputBlockToInCircBuf();

    //==============================================================================
    /** for each IR slice (threaded with priorities) * /
    bool complexMultiplyFftsToSigFftBuf (pointer inCircBufPosition, 
                                         const ado::Buffer& impFftBuf,
                                         ado::Buffer& sigFftBufAndMultipliedOutput);
    bool ifftToOutCircBuf (ado::Buffer& sigFftBufAndIfft, 
                           pointer outCircBufPosition);

    //==============================================================================
    /** for each block * /
    bool copyOutCircBufToOutputBlock (pointer outCircBufPosition);
    bool clearAlreadyOutputInOutCircBuf();
};
*/
//==============================================================================
/** Simple, zero latency convolution engine.

class Convolver
{
public:
    Convolver (const Buffer& impulse, int convolvedLength); // length should be imp + sig - 1
    ~Convolver() {}

    void convolve (const ado::Buffer& signal, ado::Buffer& out);
    void sliceConvolve (const ado::Buffer& signal, ado::Buffer& out)
    {
    }

private:
    const ado::Buffer& imp;
    std::vector<ado::Buffer> impSlices;

    ado::Buffer sigFftBuf {1,1}; // zero-padded scratchpad buffers
    ado::Buffer impFftBuf {1,1};

    juce::FFT fft128  {static_cast<int> (log2(128)),  false}; // {2^??? aka order, isIfft}
    juce::FFT fft512  {static_cast<int> (log2(528)),  false};
    juce::FFT fft2048 {static_cast<int> (log2(2048)), false};

    juce::FFT sigFft;
    juce::FFT impFft;
    juce::FFT inverseFft;

    void complexMultiplyInterleavedBuffers();

    //==============================================================================
    /** Slice the impulse into the following sizes (and stop as soon as we're over
        the impulse size).
        8 *   64 * 2
        8 *  256 * 2
        8 * 1024 * 2
        ...etc.
//
    void sliceImpulse()
    {
        int currentSliceSize {64 * 2}; // * 2 for real+imag             // this is really horrible, unfortunately
        for (int s = 0; s < imp.getNumSamples() * 2; )
        {
            for (int e = 0; e < 8 && s < imp.getNumSamples() * 2; ++e)
            {
                DBG(currentSliceSize);
                ado::Buffer slice {imp.getNumChannels(), currentSliceSize};
                impSlices.push_back (slice);
                s += currentSliceSize;
                DBG(">" << s);
            }
            currentSliceSize *= 4; // 8*64*2, 8*256*2, 8*1024*2, ...
        }
    }

    void fftImpulseSlices()
    {
        for (auto& slice : impSlices)
        {
            const int size = slice.getNumSamples();
            juce::FFT fft {static_cast<int> (log2(size)), false};
            for (int c = 0; c < slice.getNumChannels(); ++c)
                fft.performFrequencyOnlyForwardTransform (slice.getWriteArray()[c]);
            ado::coutBuffer(slice);
        }
    }
};
*/
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
