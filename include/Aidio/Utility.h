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

#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

namespace ado
{

//--------//--------//--------//--------//--------//--------//--------//--------
/** 
    Test element equality for raw audio buffers. NOT CHECKED!
*/
bool rawBufferEquals (const float** a, const float** b, int channels, int samples)
{
    for (int chan = 0; chan < channels; ++chan)
        for (int samp = 0; samp < samples; ++samp)
            if (a[chan][samp] != b[chan][samp])
                return false;
    return true;
}

//--------//--------//--------//--------//--------//--------//--------//--------
/** 
    Element copy for raw audio buffers. NOT CHECKED!
*/
void rawBufferCopy (const float** source, float** dest, int channels, int samples)
{
    for (int chan = 0; chan < channels; ++chan)
        for (int samp = 0; samp < samples; ++samp)
            dest[chan][samp] = source[chan][samp];
}

//--------//--------//--------//--------//--------//--------//--------//--------
/** 
    Sum elements (in first channel only) for raw audio buffers. NOT CHECKED!
*/
float sumElements (const float** buffer, int samples)
{
    float elementSum = 0.0f;
    for (int samp = 0; samp < samples; ++samp)
        elementSum += buffer[0][samp]; // not checked!!!

    return elementSum;
}

//--------//--------//--------//--------//--------//--------//--------//--------
/** 
    Next power of two integer larger than x
*/
int nextPowerOf2 (int x)
{
    int nextPO2 {1};
    while (nextPO2 < x)
        nextPO2 *= 2;

    return nextPO2;
}

} // namespace

#endif  // UTILITY_H_INCLUDED
