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
#include "gsl.h"

namespace ado
{

//--------//--------//--------//--------//--------//--------//--------//--------
/** 
    Non-owning, audio buffer handle

    @param data         raw buffer (e.g. in float** form)
    @param numChannels
    @param numSamples
*/
template <typename FloatType>
struct BufferHandle
{
    BufferHandle (FloatType** array, int chans, int samps)
        : data {array},
          numChannels {chans},
          numSamples {samps}
    {}
    ~BufferHandle();

    FloatType** data;
    int numChannels;
    int numSamples;
};

//--------//--------//--------//--------//--------//--------//--------//--------
/** 
    Simple audio buffer

    @param   numChannels
    @param   numSamples

    @example Buffer b (2, 512);
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
    int numSamples()  const { return bufferData[0].size(); }

    float** getWriteArray() { return pointerAccess.data(); }
    const float** getReadArray() const { return const_cast<const float**> (pointerAccess.data()); } // const!? Really?!

    void clear();
    void fillAllOnes();
    void fillAscending();

private:
    std::vector<std::vector<float>> bufferData;
    std::vector<float*> pointerAccess;
};

} // namespace

#endif  // BUFFER_H_INCLUDED
