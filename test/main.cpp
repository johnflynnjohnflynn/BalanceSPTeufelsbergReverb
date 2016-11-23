
#include "../JuceLibraryCode/JuceHeader.h"
#include "Aidio/Aidio.h"

#include "Judio/Helper.h"

int main (int argc, const char* argv[])
{
    juce::UnitTestRunner runner;
    runner.runAllTests();

    return 0;
}

//--------//--------//--------//--------//--------//--------//--------//--------

AIDIO_DECLARE_UNIT_TEST_WITH_STATIC_INSTANCE(Helper)

Helper::Helper() : UnitTest ("Helper") {}

void Helper::runTest()
{
    beginTest ("bufferFillAllOnes()");

    {
        AudioBuffer<float> buffer {2, 32};
        jdo::bufferFillAllOnes (buffer);
        expectEquals (buffer.getSample(1, 31), 1.0f);
    }

    beginTest ("bufferSumElements()");

    {
        AudioBuffer<float> buffer {1, 500};
        jdo::bufferFillAllOnes (buffer);
        const float sum {jdo::bufferSumElements(buffer)};
        expectEquals (sum, 500.0f);
    }

    beginTest ("loadBufferFromWavBinaryData()");

    {
        ado::Buffer buffer {1,1};
        jdo::bufferLoadFromWavBinaryData(BinaryData::Stereo64SamplesAllOnes_wav,
                                         BinaryData::Stereo64SamplesAllOnes_wavSize,
                                         buffer,
                                         44100);
        float sum {ado::rawBufferSum (buffer.getReadArray(), buffer.getNumChannels(), buffer.getNumSamples())};
        expectWithinAbsoluteError (sum, 128.0f, 0.001f);
        jdo::bufferLoadFromWavBinaryData(BinaryData::Stereo64SamplesAllOnes_wav,
                                         BinaryData::Stereo64SamplesAllOnes_wavSize,
                                         buffer,
                                         44100);
        sum = ado::rawBufferSum (buffer.getReadArray(), buffer.getNumChannels(), buffer.getNumSamples());
        expectWithinAbsoluteError (sum, 128.0f, 0.001f);
    }

    beginTest ("loadBufferFromWavBinaryData() check sample rate");

    {
        ado::Buffer buffer {1,1};
        jdo::bufferLoadFromWavBinaryData(BinaryData::Stereo64SamplesAllOnes_wav,
                                         BinaryData::Stereo64SamplesAllOnes_wavSize,
                                         buffer,
                                         48000);
        expectEquals (buffer.getSampleRate(), 48000);
    }

    
    beginTest ("loadBufferFromWavBinaryData() 4 channels");

    {
        ado::Buffer buffer {1,1};
        jdo::bufferLoadFromWavBinaryData(BinaryData::_4Channel_wav,     // 4 channel WAV file.
                                         BinaryData::_4Channel_wavSize, // ch1: 1 impulse
                                         buffer,                        // ch2: 2 impulses
                                         44100);                        // ch3: 3 impulses
        //ado::coutBuffer(buffer);                                      // ch4: 4 impulses
        expectEquals (buffer.getNumChannels(), 4);
        expectWithinAbsoluteError (ado::bufferSumElements (buffer), 1 + 2 + 3 + 4.f, 0.001f);
    }

    beginTest ("nextPowerOf2()");

    {
        int x = 55;
        expectEquals(jdo::nextPowerOf2(x), 64);
        x = -234;
        expectEquals(jdo::nextPowerOf2(x), 1);
        x = 16300;
        expectEquals(jdo::nextPowerOf2(x), 16384);
    }
}
