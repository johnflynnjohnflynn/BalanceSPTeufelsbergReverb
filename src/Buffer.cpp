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

#include <algorithm>
#include "Buffer.h"

namespace ado
{

//--------//--------//--------//--------//--------//--------//--------//--------

void Buffer::clear()
{
    for (auto& chan : bufferData)
        std::fill (chan.begin(), chan.end(), 0.0f);
}

void Buffer::fillAllOnes()
{
    for (auto& chan : bufferData)
        std::fill (chan.begin(), chan.end(), 1.0f);
}

void Buffer::fillAscending()
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

//--------//--------//--------//--------//--------//--------//--------//--------
    
} // namespace
