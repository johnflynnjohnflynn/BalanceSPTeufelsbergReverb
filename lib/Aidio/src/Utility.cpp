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
#include "Aidio/Utility.h"
#include "Aidio/Buffer.h"

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
    Sum elements for raw audio buffers. NOT CHECKED!
*/
float rawBufferSum (const float** buffer, int channelsToSum, int samples)
{
    float elementSum = 0.0f;
    for (int chan = 0; chan < channelsToSum; ++chan)
        for (int samp = 0; samp < samples; ++samp)
            elementSum += buffer[chan][samp]; // not checked!!!

    return elementSum;
}

//--------//--------//--------//--------//--------//--------//--------//--------
/** 
    Sum channels 1 & 3 to 1, channels 2 & 4 to 2. NOT CHECKED!
*/
void rawBufferDownmix4To2 (float** buffer, int samples)
{
    for (int samp = 0; samp < samples; ++samp)
    {
        buffer[0][samp] = buffer[0][samp] + buffer[2][samp];
        buffer[1][samp] = buffer[1][samp] + buffer[3][samp];
    }
}

//--------//--------//--------//--------//--------//--------//--------//--------
/** 
    Outputs 2 channel from a mono raw buffer pointer. NOT CHECKED!
*/
ado::RawBufferView rawBufferCopyChannelPointers1to2 (float** buffer)
{
    ado::RawBufferView bv;

    bv.channels[0] = buffer[0]; // 1 -> 11
    bv.channels[1] = buffer[0];

    return bv;
}

//--------//--------//--------//--------//--------//--------//--------//--------
/** 
    Outputs 4 channel from a 2 channel raw buffer pointer 12->1122. NOT CHECKED!
*/
ado::RawBufferView rawBufferCopyChannelPointers2to4 (float** buffer)
{
    ado::RawBufferView bv;

    bv.channels[0] = buffer[0]; // 12 -> 1122
    bv.channels[1] = buffer[0];
    bv.channels[2] = buffer[1];
    bv.channels[3] = buffer[1];

    return bv;
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
