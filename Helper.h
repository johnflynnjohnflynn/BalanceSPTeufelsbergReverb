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

#include "../JuceLibraryCode/JuceHeader.h"
#include "Aidio/Buffer.h"
#include "Aidio/Utility.h"

namespace jdo
{
//--------//--------//--------//--------//--------//--------//--------//--------
/**
    Loads JUCE binary data in an audio file format into AudioBuffer<float>

    @param  AudioFormatType e.g. WavAudioFormat or FlacAudioFormat
    @param  binaryData      Binary data. Needs WAV but DOES NOT CHECK!!!
    @param  binaryDataSize  Binary data size.
    @param  targetBuffer    buffer to write into. Will be cleared and resized!!!
    @param  fileSampleRate  Native sample rate of the file
                            Does not check if sample rate is correct!!!

    @see    juce::AudioBuffer<T>, juce::AudioFileFormat
*/
template <typename AudioFormatType> // e.g. WavAudioFormat, FlacAudioFormat
void bufferLoadFromAudioBinaryData (const void* binaryData,
                                    size_t binaryDataSize,
                                    ado::Buffer& targetBuffer,
                                    int fileSampleRate)
{
    ScopedPointer<AudioFormatType> format {new AudioFormatType};
    ScopedPointer<MemoryInputStream> mis {new MemoryInputStream {binaryData, binaryDataSize, false}};
    ScopedPointer<AudioFormatReader> audioReader {format->createReaderFor (mis.release(), true)}; // now owns mis

    const int chans = gsl::narrow<int> (audioReader->numChannels);
    const int samps = gsl::narrow<int> (audioReader->lengthInSamples);

    juce::AudioBuffer<float> temp {chans, samps};
    audioReader->read (&temp, 0, samps, 0, true, true);

    targetBuffer.clearAndResize (chans, samps, fileSampleRate);
    ado::rawBufferCopy (temp.getArrayOfReadPointers(), targetBuffer.getWriteArray(), chans, samps);
}

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
