//--------//--------//--------//--------//--------//--------//--------//--------
/*
  ==============================================================================

   This file follows the same open source licensing as the JUCE library.
   Copyright (c) 2016 John Flynn

   Permission is granted to use this software under the terms of either:
   a) the GPL v2 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   Judio is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ==============================================================================
*/
//--------//--------//--------//--------//--------//--------//--------//--------

#ifndef HELPER_H_INCLUDED
#define HELPER_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"
#include "Aidio/Buffer.h"

namespace jdo
{
//--------//--------//--------//--------//--------//--------//--------//--------
/**
    Loads JUCE binary data in WAV format into AudioBuffer<float>

    @param  binaryData      Binary data. Needs WAV but DOES NOT CHECK!!!
    @param  binaryDataSize  Binary data size.
    @param  targetBuffer    buffer to write into. Will be cleared and resized!!!
    @param  fileSampleRate  Native sample rate of the file
                            Does not check if sample rate is correct!!!

    @see    juce::AudioBuffer<T>
*/
void bufferLoadFromWavBinaryData (const void* binaryData,
                                  size_t binaryDataSize,
                                  ado::Buffer& targetBuffer,
                                  int fileSampleRate);

//--------//--------//--------//--------//--------//--------//--------//--------
/**
    Fills entire AudioBuffer<float> with value 1.0f

    @param  buffer  Buffer to fill.

    @see    juce::AudioBuffer<T>
*/
void bufferFillAllOnes (juce::AudioBuffer<float>& buffer);

//--------//--------//--------//--------//--------//--------//--------//--------
/**
    @brief  Sums the elements of an AudioBuffer<float>

    @param  buffer  Buffer to sum.
    @return         Summed value.

    @see    juce::AudioBuffer<T>
*/
float bufferSumElements (const juce::AudioBuffer<float>& buffer);

//--------//--------//--------//--------//--------//--------//--------//--------
/** 
    Next power of two integer larger than x
*/
int nextPowerOf2 (int x);

} // namespace

#endif  // HELPER_H_INCLUDED
