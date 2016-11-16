
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#define gsl_CONFIG_CONTRACT_VIOLATION_THROWS 1

#include "Judio/Utility.h"

//--------//--------//--------//--------//--------//--------//--------//--------

TEST_CASE("bufferFillAllOnes()", "Utility") {
    AudioBuffer<float> buffer {2, 32};
    jdo::bufferFillAllOnes (buffer);
    REQUIRE (buffer.getSample(1, 31) == 1.0f);
}

TEST_CASE("bufferSumElements()", "Utility") {
    AudioBuffer<float> buffer {1, 500};
    jdo::bufferFillAllOnes (buffer);
    const float sum {jdo::bufferSumElements(buffer)};
    REQUIRE (sum == 500.0f);
}

TEST_CASE("loadBufferFromWavBinaryData()", "Utility") {
    AudioBuffer<float> buffer;
    jdo::bufferLoadFromWavBinaryData(BinaryData::Stereo64SamplesAllOnes_wav,
                                     BinaryData::Stereo64SamplesAllOnes_wavSize,
                                     buffer);
    const float sum {jdo::bufferSumElements(buffer)};
    REQUIRE (Approx(sum) == 128.0f);
}

TEST_CASE("nextPowerOf2()", "Utility") {
    int x = 55;
    REQUIRE(jdo::nextPowerOf2(x) == 64);
    x = -234;
    REQUIRE(jdo::nextPowerOf2(x) == 1);
    x = 16300;
    REQUIRE(jdo::nextPowerOf2(x) == 16384);
}
