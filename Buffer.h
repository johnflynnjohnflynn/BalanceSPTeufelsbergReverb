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

#include <cassert>

#include "../JuceLibraryCode/JuceHeader.h"
#include "Dependencies/gsl.h"

namespace ado
{

//==============================================================================
/** class BufferView... see p948 Stroustrup PPP

    @example

    void process (BufferView& buff)
    {
        for (auto chan : buff)      // for (float* chan : buff)
            for (auto& samp : chan) // for (float& samp : chan)
            {
                samp = sineWave();
                samp *= 0.25f;
            }

        buff (0, 148) = 0.0f; // mute 148th sample in first channel
    }

    //...
    process (BufferView {buffer, channels, samples});
    //...
*/
class ChannelView
{
public:
    ChannelView (float* buffer, int numSamples)
        : fp {buffer}, samps {numSamples}
    {
        Expects (8 <= samps);
    }

          float& operator() (int s)       { jassert (0 <= s && s < samps); return fp[s]; }
    const float& operator() (int s) const { jassert (0 <= s && s < samps); return fp[s]; }

    int getNumSamples() const { return samps; }

    //==============================================================================

    class SampleIterator
    {
    public:
        SampleIterator (float* sample)
            : curr {sample}
        {}

        float* operator++()                     { ++curr; return curr; }
        float* operator++(int) { float* c = curr; ++curr; return c; }
        float& operator*()                              { return *curr; }
        bool operator!= (const SampleIterator& other) { return curr != other.curr; }

    private:
        float* curr;
    };

          SampleIterator begin()       { return SampleIterator {&fp[0]    }; }
          SampleIterator end()         { return SampleIterator {&fp[samps]}; }
    const SampleIterator begin() const { return SampleIterator {&fp[0]    }; }
    const SampleIterator end()   const { return SampleIterator {&fp[samps]}; }

private:
    float* fp;
    int samps;
};

//==============================================================================

class BufferView
{
public:
    BufferView (float** buffer, int numChannels, int numSamples)
        : fpp {buffer}, chans {numChannels}, samps {numSamples}
    {
        Expects (1 <= chans && chans <= 6);     // don't mix up channels and samples
        Expects (8 <= samps);
    }

    float& operator() (int c, int s)
    {
        jassert (0 <= c && c < chans);
        jassert (0 <= s && s < samps);
        return fpp[c][s];
    }
    float operator() (int c, int s) const
    {
        jassert (0 <= c && c < chans);
        jassert (0 <= s && s < samps);
        return fpp[c][s];
    }

    ChannelView channel (int c)
    {
        jassert (0 <= c && c < chans);
        return ChannelView {fpp[c], samps};
    }

    int getNumChannels() const { return chans; }
    int getNumSamples()  const { return samps; }

    //==============================================================================

    class ChannelIterator
    {
    public:
        ChannelIterator (float** channel, int numSamples)
            : curr {channel}, samps {numSamples}
        {}

        float** operator++()                      { ++curr; return curr; }
        float** operator++(int) { float** c = curr; ++curr; return c; }
        ChannelView operator*() { return ChannelView {*curr, samps}; }
        bool operator!= (const ChannelIterator& other) { return curr != other.curr; }

    private:
        float** curr;
        int samps;
    };

          ChannelIterator begin()       { return ChannelIterator {&fpp[0],     samps}; }
          ChannelIterator end()         { return ChannelIterator {&fpp[chans], samps}; }
    const ChannelIterator begin() const { return ChannelIterator {&fpp[0],     samps}; }
    const ChannelIterator end()   const { return ChannelIterator {&fpp[chans], samps}; }

    //==============================================================================

    void fillAllOnes ()
    {
        for (auto chan : *this)
            for (auto& samp : chan)
                samp = 1.0f;
    }

    void fillAscending()
    {
        for (auto chan : *this)
        {
            float sum = 1.0f;

            for (auto& samp : chan)
            {
                samp = sum;
                sum += 1.0f;
            }
        }
    }
    float** getFpp() { return fpp; }

private:
    float** fpp;
    int chans;
    int samps;
};

//==============================================================================
/**
*/
BufferView makeBufferView (juce::AudioBuffer<float>& juceBuffer);

void coutBuffer (const BufferView& buffer);

//==============================================================================
/** Simple audio buffer based on juce::AudioBuffer<T>

    @example Buffer b {2, 512, 44100};
             float** rawBuff = b.getWriteArray();
             
    @see juce::AudioBuffer<T>
*/
class Buffer
{
public:
    Buffer (int numChannels = 1, int numSamples = 1, int samplingRate = 44100);
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

    void copyFrom (const Buffer& source)
    {
        jassert (source.getNumChannels() <= getNumChannels());
        jassert (source.getNumSamples() <= getNumSamples());

        clear();

        for (int c = 0; c < source.getNumChannels(); ++c)
            for (int s = 0; s < source.getNumSamples(); ++s)
                getWriteArray()[c][s] = source.getReadArray()[c][s];
    }

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
