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

#ifndef BUFFER_H_INCLUDED
#define BUFFER_H_INCLUDED

#include <vector>
#include <algorithm>
#include <iostream>
#include "gsl.h"

namespace ado
{

//--------//--------//--------//--------//--------//--------//--------//--------
/** 
    Simple audio buffer

    Buffer b (2, 512);
    float** ptrPtrBuffer = b.getWriteArray();
*/
class Buffer
{
public:
    Buffer (int numChannels, int numSamples)
        : bufferData {gsl::narrow<size_t> (numChannels),
                      std::vector<float>  (numSamples)}
    {
        Expects (numChannels > 0);
        Expects (numSamples  > 0);

        for (auto& channel : bufferData)
            pointerAccess.push_back (channel.data());
    }
    ~Buffer() {}

    int numChannels() const { return bufferData.size(); }
    int numSamples()  const { return (numChannels() > 0) ? bufferData[0].size() : 0; }

    float** getWriteArray() { return pointerAccess.data(); }
    const float** getReadArray() { return const_cast<const float**> (getWriteArray()); } // Ugh, const_cast

    void clear()
    {
        for (auto& chan : bufferData)
            std::fill (chan.begin(), chan.end(), 0.0f);
    }

    void fillAllOnes()
    {
        for (auto& chan : bufferData)
            std::fill (chan.begin(), chan.end(), 1.0f);
    }

    void fillAscending()
    {
        for (auto& chan : bufferData)
        {
            float sum = 1.0f;
            for (auto& samp : chan)
            {
                samp = sum;
                sum += 1.0f;
            }
        }
    }

private:
    std::vector<std::vector<float>> bufferData;
    std::vector<float*> pointerAccess;
};

} // namespace

#endif  // BUFFER_H_INCLUDED
