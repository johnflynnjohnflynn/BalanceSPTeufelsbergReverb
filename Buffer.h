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

#ifndef BUFFER_H_INCLUDED
#define BUFFER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Aidio/Dependencies/gsl.h"

namespace ado
{

//==============================================================================
/** Simple audio buffer based on juce::AudioBuffer<T>

    @example Buffer b {2, 512, 44100};
             float** rawBuff = b.getWriteArray();
             
    @see juce::AudioBuffer<T>
*/
class Buffer
{
public:
    Buffer (int numChannels, int numSamples, int samplingRate = 44100);
    ~Buffer() {}

    void clearAndResize (int numChannels, int numSamples);
    void clearAndResize (int numChannels, int numSamples, int samplingRate);

    int getNumChannels() const { return bufferData.getNumChannels(); }
    int getNumSamples()  const { return bufferData.getNumSamples();  }
    int getSampleRate()  const { return sampleRate; }

    const float** getReadArray() const { return bufferData.getArrayOfReadPointers();  }
          float** getWriteArray()      { return bufferData.getArrayOfWritePointers(); }

    void clear() { bufferData.clear(); }
    void fillAllOnes();                                                     
    void fillAscending();

    Buffer& operator*= (float scale);

private:
    int sampleRate;
    juce::AudioBuffer<float> bufferData;
};

//==============================================================================
/** Print all elements in buffer using cout
*/
void coutBuffer (ado::Buffer& buffer);

//==============================================================================
/** Sum all elements in all channels of buffer
*/
float bufferSumElements (const Buffer& buffer);

} // namespace

#endif  // BUFFER_H_INCLUDED
