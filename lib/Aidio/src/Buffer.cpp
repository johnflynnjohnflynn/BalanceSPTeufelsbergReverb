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

#include <iostream>
#include "Aidio/Buffer.h"

namespace ado
{

//--------//--------//--------//--------//--------//--------//--------//--------

void Buffer::clearAndResize (int numChannels, int numSamples)
{
    Expects (numChannels  > 0);
    Expects (numSamples   > 0);

    bufferData.setSize (numChannels, numSamples);
    clear(); // juce buffer does not clear on creation
}

void Buffer::clearAndResize (int numChannels, int numSamples, int samplingRate)
{
    Expects (samplingRate > 0);
    
    sampleRate = samplingRate;
    clearAndResize (numChannels, numSamples);
}

void Buffer::fillAllOnes()
{
    for (int chan = 0; chan < getNumChannels(); ++chan)
        for (int samp = 0; samp < getNumSamples(); ++samp)
            bufferData.setSample(chan, samp, 1.0f);
}

void Buffer::fillAscending()
{
    for (int chan = 0; chan < getNumChannels(); ++chan)
    {
        float sum = 1.0f;

        for (int samp = 0; samp < getNumSamples(); ++samp)
        {
            bufferData.setSample(chan, samp, sum);
            sum += 1.0f;
        }
    }
}

Buffer& Buffer::operator*= (float scale)
{
    for (int chan = 0; chan < getNumChannels(); ++chan)
        for (int samp = 0; samp < getNumSamples(); ++samp)
            getWriteArray()[chan][samp] *= scale;

    return *this;
}

//--------//--------//--------//--------//--------//--------//--------//--------

void coutBuffer (ado::Buffer& buffer)
{
    for (int chan = 0; chan < buffer.getNumChannels(); ++chan)
        for (int samp = 0; samp < buffer.getNumSamples(); ++samp)
            std::cout << buffer.getReadArray()[chan][samp]
                      << " [" << chan << "][" << samp << "]"<< "\n";
    std::cout << "\n";
}

float bufferSumElements (const Buffer& buffer)
{
    float sum {0};
    for (int chan = 0; chan < buffer.getNumChannels(); ++chan)
        for (int samp = 0; samp < buffer.getNumSamples(); ++samp)
            sum += buffer.getReadArray()[chan][samp];

    return sum;
}
    
} // namespace
