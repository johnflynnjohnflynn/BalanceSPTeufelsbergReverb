
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#define gsl_CONFIG_CONTRACT_VIOLATION_THROWS 1

#include "WDL/convoengine.h"
#include "Aidio/Buffer.h"
#include "Aidio/Utility.h"

TEST_CASE ("conv1", "sdkl") {

}

TEST_CASE ("sdf234sf", "sdf") {
    ado::Buffer hh {1, 8};
    hh.fillAllOnes();

    WDL_ImpulseBuffer h;
    h.Set (const_cast<const float**> (hh.getWriteArray()), // ugly!
                                      hh.numSamples(),
                                      hh.numChannels());

    WDL_ConvolutionEngine engine;

    engine.SetImpulse (&h, 4, 0, 8, false);

    engine.Reset();

    
    ado::Buffer y {1, 32};
    y.getWriteArray()[0][0] = 1.0f;

    engine.Add (y.getWriteArray(),              // Send input to conv engine
                y.numSamples(),
                y.numChannels());

    const int avail = engine.Avail (y.numSamples());   // Confirm full buffer available
    //std::cout << avail << "\n";

    float** convolved = engine.Get();

    for (int channel = 0; channel < y.numChannels(); ++channel)
        for (int sample = 0; sample < y.numSamples(); ++sample)
            y.getWriteArray()[channel][sample] = convolved[channel][sample]; // does NOT range check [channel][sample]!!!
/*
    for (int i = 0; i < y.numSamples(); ++i)
        std::cout << y.getWriteArray()[0][i] << "\n";
*/
    engine.Advance (y.numSamples());                   // Advance the engine
}

//--------//--------//--------//--------//--------//--------//--------//--------

TEST_CASE ("Constructs", "Buffer") {
    CHECK_NOTHROW (ado::Buffer (2, 512));
}

TEST_CASE ("Must have channels", "Buffer") {
    CHECK_THROWS (ado::Buffer (0, 512));
}

TEST_CASE ("Must have samples", "Buffer") {
    CHECK_THROWS (ado::Buffer (2, 0));
}

TEST_CASE ("Must have +ve samples", "Buffer") {
    CHECK_THROWS (ado::Buffer (2, -1));
}

TEST_CASE ("No ridiculous numChannels", "Buffer") {
    CHECK_THROWS (ado::Buffer (size_t(2147483647)+12039812, 0));
}

TEST_CASE ("numChannels()", "Buffer") {
    ado::Buffer b {4, 1024};
    REQUIRE (b.numChannels() == 4);
}

TEST_CASE ("numSamples()", "Buffer") {
    ado::Buffer b {4, 1024};
    REQUIRE (b.numSamples() == 1024);
}

TEST_CASE ("getWriteArray()", "Buffer") {
    ado::Buffer b {4, 1024};
    float** bb {b.getWriteArray()};
    int chan = 2;
    int samp = 385;
    REQUIRE (bb[chan][samp] == 0.0f);
    bb[chan][samp] = 0.239f;
    REQUIRE (bb[chan][samp] == 0.239f);
}

TEST_CASE ("fillAllOnes()", "Buffer") {
    ado::Buffer b {4, 1024};
    CHECK_NOTHROW(b.fillAllOnes());
    float elementSum = 0.0f;
    for (int i = 0; i < b.numSamples(); ++i)
        elementSum += b.getWriteArray()[0][i];
    REQUIRE (elementSum == b.numSamples());
}

//--------//--------//--------//--------//--------//--------//--------//--------

TEST_CASE("rawBufferEquals()", "Utility") {
    ado::Buffer a {2, 2048};
    ado::Buffer b {2, 2048};
    b.fillAllOnes();
    REQUIRE_FALSE (ado::rawBufferEquals (a.getWriteArray(), b.getWriteArray(), 2, 2048));
    a.fillAllOnes();
    REQUIRE       (ado::rawBufferEquals (a.getWriteArray(), b.getWriteArray(), 2, 2048));

}

TEST_CASE("rawBufferCopy()", "Utility") {
    ado::Buffer source {2, 2048};
    ado::Buffer dest   {2, 2048};
    source.fillAllOnes();
    REQUIRE_FALSE (ado::rawBufferEquals (source.getWriteArray(), dest.getWriteArray(), 2, 2048));
    ado::rawBufferCopy (source.getWriteArray(), dest.getWriteArray(), 2, 2048);
    REQUIRE       (ado::rawBufferEquals (source.getWriteArray(), dest.getWriteArray(), 2, 2048));
}
