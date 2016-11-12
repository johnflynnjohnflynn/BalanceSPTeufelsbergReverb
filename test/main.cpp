
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#define gsl_CONFIG_CONTRACT_VIOLATION_THROWS 1

#include "WDL/convoengine.h"
#include "Aidio/Aidio.h"

//--------//--------//--------//--------//--------//--------//--------//--------

TEST_CASE ("Delta through 9 point moving average", "Convolution") {
    ado::Buffer h {1, 9};
    h.fillAllOnes(); // moving average

    ado::Convolution engine {h};
    engine.reset();

    int blockSize = 8;
    ado::Buffer block {1, blockSize};
    block.getWriteArray()[0][0] = 1.0f; // kronecker

    engine.process (block);     // run one block and sum
    //ado::coutBuffer (block);

    float sum = ado::rawBufferSum (block.getReadArray(), 1, blockSize);

    block.clear();              // run next block (clear last output) and sum
    engine.process (block);
    //ado::coutBuffer (block);

    sum += ado::rawBufferSum (block.getReadArray(), 1, blockSize);

    REQUIRE (sum == 9.0f);
}

TEST_CASE ("Delta through 256 point moving average", "Convolution") {
    ado::Buffer h {1, 256};
    h.fillAllOnes(); // moving average

    ado::Convolution engine {h};
    engine.reset();

    int blockSize = 200;
    ado::Buffer block {1, blockSize};
    block.getWriteArray()[0][0] = 1.0f; // kronecker

    engine.process (block);     // run one block and sum
    //ado::coutBuffer (block);

    float sum = ado::rawBufferSum (block.getReadArray(), 1, blockSize);

    block.clear();              // run next block (clear last output) and sum
    engine.process (block);
    //ado::coutBuffer (block);

    sum += ado::rawBufferSum (block.getReadArray(), 1, blockSize);

    REQUIRE (sum == 256.0f);
}

TEST_CASE (">2 channels does not work!!!", "Convolution") {                     // NB: >2 channels will not work!
    const int channels {3};

    ado::Buffer h {channels, 2};
    h.fillAllOnes(); // 2 point moving average

    ado::Convolution engine {h};
    engine.reset();

    const int blockSize = 16;
    ado::Buffer block {channels, blockSize};
    block.getWriteArray()[0][0] = 1.0f; // kronecker
    block.getWriteArray()[1][0] = 1.0f; // kronecker
    block.getWriteArray()[2][0] = 1.0f; // kronecker

    engine.process (block);     // run one block and sum
    //ado::coutBuffer (block);

    float sum = ado::rawBufferSum (block.getReadArray(), channels, blockSize);

    block.clear();              // run next block (clear last output) and sum
    engine.process (block);
    //ado::coutBuffer (block);

    sum += ado::rawBufferSum (block.getReadArray(), channels, blockSize);

    REQUIRE (sum != 6.0f); // uncomment coutBuffer lines to inspect
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

TEST_CASE ("fillAscending()", "Buffer") {
    ado::Buffer b {64, 4};
    CHECK_NOTHROW(b.fillAscending());
    float elementSum = 0.0f;
    for (int i = 0; i < b.numSamples(); ++i)
        elementSum += b.getWriteArray()[0][i];
    REQUIRE (elementSum == 1 + 2 + 3 + 4);
}

TEST_CASE ("Clear()", "Buffer") {
    ado::Buffer b {4, 1024};
    CHECK_NOTHROW(b.fillAllOnes());
    CHECK_NOTHROW(b.clear());
    float elementSum = 0.0f;
    for (int i = 0; i < b.numSamples(); ++i)
        elementSum += b.getWriteArray()[0][i];
    REQUIRE (elementSum == 0.0f);
}

//--------//--------//--------//--------//--------//--------//--------//--------

TEST_CASE("rawBufferEquals()", "Utility") {
    ado::Buffer a {2, 2048};
    ado::Buffer b {2, 2048};
    b.fillAllOnes();
    REQUIRE_FALSE (ado::rawBufferEquals (a.getReadArray(), b.getReadArray(), 2, 2048));
    a.fillAllOnes();
    REQUIRE       (ado::rawBufferEquals (a.getReadArray(), b.getReadArray(), 2, 2048));

}

TEST_CASE("rawBufferCopy()", "Utility") {
    ado::Buffer source {2, 2048};
    ado::Buffer dest   {2, 2048};
    source.fillAllOnes();
    REQUIRE_FALSE (ado::rawBufferEquals (source.getReadArray(), dest.getReadArray(), 2, 2048));
    ado::rawBufferCopy (source.getReadArray(), dest.getWriteArray(), 2, 2048);
    REQUIRE       (ado::rawBufferEquals (source.getReadArray(), dest.getReadArray(), 2, 2048));
}

TEST_CASE("rawBufferSum()", "Utility") {
    ado::Buffer buffer {2, 2048};
    buffer.fillAllOnes();
    REQUIRE (ado::rawBufferSum (buffer.getReadArray(), 1, 2048) == 2048.0f);
}

TEST_CASE("nextPowerOf2()", "Utility") {
    int x = 55;
    REQUIRE(ado::nextPowerOf2(x) == 64);
    x = -234;
    REQUIRE(ado::nextPowerOf2(x) == 1);
    x = 16300;
    REQUIRE(ado::nextPowerOf2(x) == 16384);
}
