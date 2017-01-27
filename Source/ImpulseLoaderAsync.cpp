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
        jdo::bufferLoadFromWavBinaryData (BinaryData::balancemasteringteufelsbergIR014410024bit_wav,
                                          BinaryData::balancemasteringteufelsbergIR014410024bit_wavSize,
                                          ir,
                                          44100);
        break;
        case 2:
        jdo::bufferLoadFromWavBinaryData (BinaryData::balancemasteringteufelsbergIR024410024bit_wav,
                                          BinaryData::balancemasteringteufelsbergIR024410024bit_wavSize,
                                          ir,
                                          44100);
        break;
        case 3:
        jdo::bufferLoadFromWavBinaryData (BinaryData::balancemasteringteufelsbergIR034410024bit_wav,
                                          BinaryData::balancemasteringteufelsbergIR034410024bit_wavSize,
                                          ir,
                                          44100);
        break;
        case 4:
        jdo::bufferLoadFromWavBinaryData (BinaryData::balancemasteringteufelsbergIR044410024bit_wav,
                                          BinaryData::balancemasteringteufelsbergIR044410024bit_wavSize,
                                          ir,
                                          44100);
        break;
        case 5:
        jdo::bufferLoadFromWavBinaryData (BinaryData::balancemasteringteufelsbergIR054410024bit_wav,
                                          BinaryData::balancemasteringteufelsbergIR054410024bit_wavSize,
                                          ir,
                                          44100);
        break;
        case 6:
        jdo::bufferLoadFromWavBinaryData (BinaryData::balancemasteringteufelsbergIR064410024bit_wav,
                                          BinaryData::balancemasteringteufelsbergIR064410024bit_wavSize,
                                          ir,
                                          44100);
        break;

        default: jassertfalse;  // there are only 6 IRs 1-6 !!!
        break;
    }

    ir *= 0.1f;                 // reduce WAV gain

    engine.set (ir);
}