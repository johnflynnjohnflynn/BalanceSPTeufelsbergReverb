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

#include "Buffer.h"
#include "WDL/convoengine.h"

namespace ado
{

//--------//--------//--------//--------//--------//--------//--------//--------
/**
    Simple, zero latency convolution engine built on WDL.
    
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
