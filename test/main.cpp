
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

AIDIO_DECLARE_UNIT_TEST_WITH_STATIC_INSTANCE(Resample)

Resample::Resample() : UnitTest ("Resample") {}

void Resample::runTest()
{

beginTest ("bufferFillAllOnes()"); {
    AudioBuffer<float> buffer {2, 32};
    jdo::bufferFillAllOnes (buffer);
    expectEquals (buffer.getSample(1, 31), 1.0f);
}

beginTest ("bufferSumElements()"); {
    AudioBuffer<float> buffer {1, 500};
    jdo::bufferFillAllOnes (buffer);
    const float sum {jdo::bufferSumElements(buffer)};
    expectEquals (sum, 500.0f);
}

beginTest ("loadBufferFromWavBinaryData()"); {
    AudioBuffer<float> buffer;
    jdo::bufferLoadFromWavBinaryData(BinaryData::Stereo64SamplesAllOnes_wav,
                                     BinaryData::Stereo64SamplesAllOnes_wavSize,
                                     buffer);
    const float sum {jdo::bufferSumElements(buffer)};
    expectEquals (Approx(sum), 128.0f);
}

beginTest ("nextPowerOf2()"); {
    int x = 55;
    expectEquals(jdo::nextPowerOf2(x), 64);
    x = -234;
    expectEquals(jdo::nextPowerOf2(x), 1);
    x = 16300;
    expectEquals(jdo::nextPowerOf2(x), 16384);
}

}
