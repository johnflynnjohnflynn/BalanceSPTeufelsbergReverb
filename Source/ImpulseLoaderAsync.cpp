/*
==============================================================================

ImpulseLoaderAsync.cpp
Created: 27 Jan 2017 8:56:38am
Author:  John Flynn

==============================================================================
*/

#include "ImpulseLoaderAsync.h"

ImpulseLoaderAsync::ImpulseLoaderAsync (ado::Convolution& eng, ado::Buffer& impulse)
    : engine {eng},
      ir {impulse}
{
    startTimer (500);
}

void ImpulseLoaderAsync::changeImpulseAsync (int newImpulse)
{
    if (newImpulse != currentImpulse)
    {
        currentImpulse = newImpulse;
        nowChangingImpulseAsync = true;
    }
}

// private:

void ImpulseLoaderAsync::timerCallback()
{
    if (nowChangingImpulseAsync)
    {
        changeImpulse (currentImpulse);
        nowChangingImpulseAsync = false;
    }
}

void ImpulseLoaderAsync::changeImpulse (int newImpulse)
{
    jassert (1 <= newImpulse && newImpulse <= 6);   // only 6 WAV IRs to choose!

    switch (newImpulse)
    {
        case 1:                                               // here's the number \/
        jdo::bufferLoadFromAudioBinaryData<FlacAudioFormat> (BinaryData::balancemasteringteufelsbergIR014410024bit_flac,
                                                             BinaryData::balancemasteringteufelsbergIR014410024bit_flacSize,
                                                             ir,
                                                             44100);
        break;
        case 2:
        jdo::bufferLoadFromAudioBinaryData<FlacAudioFormat> (BinaryData::balancemasteringteufelsbergIR024410024bit_flac,
                                                             BinaryData::balancemasteringteufelsbergIR024410024bit_flacSize,
                                                             ir,
                                                             44100);
        break;
        case 3:
        jdo::bufferLoadFromAudioBinaryData<FlacAudioFormat> (BinaryData::balancemasteringteufelsbergIR034410024bit_flac,
                                                             BinaryData::balancemasteringteufelsbergIR034410024bit_flacSize,
                                                             ir,
                                                             44100);
        break;
        case 4:
        jdo::bufferLoadFromAudioBinaryData<FlacAudioFormat> (BinaryData::balancemasteringteufelsbergIR044410024bit_flac,
                                                             BinaryData::balancemasteringteufelsbergIR044410024bit_flacSize,
                                                             ir,
                                                             44100);
        break;
        case 5:
        jdo::bufferLoadFromAudioBinaryData<FlacAudioFormat> (BinaryData::balancemasteringteufelsbergIR054410024bit_flac,
                                                             BinaryData::balancemasteringteufelsbergIR054410024bit_flacSize,
                                                             ir,
                                                             44100);
        break;
        case 6:
        jdo::bufferLoadFromAudioBinaryData<FlacAudioFormat> (BinaryData::balancemasteringteufelsbergIR064410024bit_flac,
                                                             BinaryData::balancemasteringteufelsbergIR064410024bit_flacSize,
                                                             ir,
                                                             44100);
        break;

        default: jassertfalse;  // there are only 6 IRs 1-6 !!!
        break;
    }

    engine.set (ir);
}