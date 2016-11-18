
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "Aidio/Aidio.h"

//--------//--------//--------//--------//--------//--------//--------//--------
// Resample.h

TEST_CASE ("Resample 441 samples to 882", "Resample") {
    ado::Buffer source {1, 441};
    source.fillAscending();
    ado::Buffer dest = ado::resampleBuffer (source, 441, 882);
    //ado::coutBuffer(source);
    //ado::coutBuffer(dest);
    REQUIRE (source.getReadArray()[0][440]
     == Approx(dest.getReadArray()[0][880]));
}

TEST_CASE ("Resample 128 samples to 64", "Resample") {
    ado::Buffer source {1, 128};
    source.fillAscending();
    ado::Buffer dest = ado::resampleBuffer (source, 128, 64);
    //ado::coutBuffer(source);
    //ado::coutBuffer(dest);
    REQUIRE (134.632 == Approx(dest.getReadArray()[0][63]));
}

TEST_CASE ("Should not resample at all", "Resample") {
    ado::Buffer source {1, 128};
    source.fillAscending();
    ado::Buffer dest = ado::resampleBuffer (source, 128, 128);
    REQUIRE (source.getReadArray()[0][127]
            == dest.getReadArray()[0][127]);
}

TEST_CASE ("2 channels, should not resample", "Resample") {
    ado::Buffer source {2, 300};
    source.fillAscending();
    ado::Buffer dest = ado::resampleBuffer (source, 441, 441);
    //ado::coutBuffer(source);
    //ado::coutBuffer(dest);
    REQUIRE (source.getReadArray()[1][299]
            == dest.getReadArray()[1][299]);
}

TEST_CASE ("2 channels, 441->882", "Resample") {
    ado::Buffer source {2, 300};
    source.fillAscending();
    ado::Buffer dest = ado::resampleBuffer (source, 441, 882);
    //ado::coutBuffer(source);
    //ado::coutBuffer(dest);
    REQUIRE (source.getReadArray()[1][299]
     == Approx(dest.getReadArray()[1][598]));
}

TEST_CASE ("16 channels, 441->480", "Resample") {
    ado::Buffer source {16, 30};
    source.fillAscending();
    ado::Buffer dest = ado::resampleBuffer (source, 441, 480);
    REQUIRE (33.5712 == Approx(dest.getReadArray()[15][31]));   // check many channels
    REQUIRE (33.5712 == Approx(dest.getReadArray()[14][31]));   // i.e. don't carry garbage
    REQUIRE (33.5712 == Approx(dest.getReadArray()[5][31]));    // from previous channel pass
}

//--------//--------//--------//--------//--------//--------//--------//--------
// Convolution.h

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

TEST_CASE ("Ascending sequence with delta, 2 channels", "Convolution") {
    const int channels {2};
    ado::Buffer h {channels, 8};
    h.fillAscending(); // h[n] = {1,2,3,4,5,6,7,8}

    ado::Convolution engine {h};
    engine.reset();

    const int blockSize = 9;
    ado::Buffer block {channels, blockSize};
    block.getWriteArray()[0][0] = 1.0f; // kronecker
    block.getWriteArray()[1][0] = 1.0f; // kronecker

    engine.process (block);     // run one block and sum
    //ado::coutBuffer (block);

    float sum = ado::rawBufferSum (block.getReadArray(), channels, blockSize);

    block.clear();              // run next block (clear last output) and sum
    engine.process (block);
    //ado::coutBuffer (block);

    sum += ado::rawBufferSum (block.getReadArray(), channels, blockSize);

    REQUIRE (sum == 36.0f * channels); // octave: sum(conv(h, x))
}

TEST_CASE ("Ascending sequence with ascending sequence", "Convolution") {
    const int channels {2};
    ado::Buffer h {channels, 8};
    h.fillAscending(); // h[n] = {1,2,3,4,5,6,7,8}

    ado::Convolution engine {h};
    engine.reset();

    const int blockSize = 8;
    ado::Buffer block {channels, blockSize};
    block.fillAscending(); // x[n] = {1,2,3,4,5,6,7,8}

    engine.process (block);     // run one block and sum
    //ado::coutBuffer (block);

    float sum = ado::rawBufferSum (block.getReadArray(), channels, blockSize);

    block.clear();              // run next block (clear last output) and sum
    engine.process (block);
    //ado::coutBuffer (block);

    sum += ado::rawBufferSum (block.getReadArray(), channels, blockSize);

    REQUIRE (sum == 1296.0f * channels); // octave: sum(conv(h, x))
}

TEST_CASE ("Long ascending sequence with ascending sequence", "Convolution") {
    const int channels {2};
    const int ascendingSequenceLength {1000};
    ado::Buffer h {channels, ascendingSequenceLength};
    h.fillAscending(); // octave h = 1:1000

    ado::Convolution engine {h};
    engine.reset();

    const int blockSize = ascendingSequenceLength;
    ado::Buffer block {channels, blockSize};
    block.fillAscending();

    engine.process (block);     // run one block and sum
    //ado::coutBuffer (block);

    float sum = ado::rawBufferSum (block.getReadArray(), channels, blockSize);

    block.clear();              // run next block (clear last output) and sum
    engine.process (block);
    //ado::coutBuffer (block);

    sum += ado::rawBufferSum (block.getReadArray(), channels, blockSize);

    REQUIRE (sum == Approx(2.5050e+11 * channels)); // octave: sum(conv(h, h))
}

TEST_CASE ("set()", "Convolution") {
    ado::Buffer h {1, 2};
    h.fillAllOnes();

    ado::Convolution engine {h};
    engine.reset();

    ado::Buffer block {1, 4};
    block.getWriteArray()[0][0] = 1.0f; // kronecker

    engine.process (block);     // run one block and sum

    float sum = ado::rawBufferSum (block.getReadArray(), 1, 4);

    REQUIRE (sum == 2.0f);

    block.clear();
    engine.process(block);

    ado::Buffer hh {1, 2};
    hh.fillAscending();      // hh[n] = {1,2}

    engine.set(hh); // set new impulse

    block.clear();
    block.getWriteArray()[0][0] = 1.0f; // kronecker

    engine.process (block);     // run one block and sum

    sum = ado::rawBufferSum (block.getReadArray(), 1, 4);

    REQUIRE (sum == 3.0f);
}

//--------//--------//--------//--------//--------//--------//--------//--------
// Buffer.h

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
// Utility.h

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
