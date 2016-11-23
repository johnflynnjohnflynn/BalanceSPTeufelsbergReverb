
#include "../JuceLibraryCode/JuceHeader.h"
#include "Aidio/Aidio.h"

int main(int argc, const char* argv[])
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
    beginTest ("Resample 128 samples to 64");

    {
        ado::Buffer source2 {1, 128, 128};
        source2.fillAscending();
        ado::Buffer dest2 = ado::resampleBuffer (source2, 64);
        expectWithinAbsoluteError (134.632f, dest2.getReadArray()[0][63], 0.001f);
    }

    beginTest ("Resample 441 samples to 882");

    {
        ado::Buffer source {1, 441, 44100};
        source.fillAscending();
        ado::Buffer dest = ado::resampleBuffer (source, 88200);
        //ado::coutBuffer(source);
        //ado::coutBuffer(dest);
        expectWithinAbsoluteError (source.getReadArray()[0][440],
                                     dest.getReadArray()[0][880], 0.001f);
    }

    beginTest ("Should not resample at all");

    {
        ado::Buffer source3 {1, 128};
        source3.fillAscending();
        ado::Buffer dest3 = ado::resampleBuffer (source3, 44100);
        expectEquals (source3.getReadArray()[0][127],
                        dest3.getReadArray()[0][127]);
    }

    beginTest ("2 channels, should not resample");

    {
        ado::Buffer source4 {2, 300, 44100};
        source4.fillAscending();
        ado::Buffer dest4 = ado::resampleBuffer (source4, 44100);
        expectEquals (source4.getReadArray()[1][299],
                        dest4.getReadArray()[1][299]);
    }

    beginTest ("1 channel, 441->882 then 441->176400");

    {
        ado::Buffer source5 {1, 300};
        source5.fillAscending();
        ado::Buffer dest5 = ado::resampleBuffer (source5, 88200);
        expectWithinAbsoluteError (source5.getReadArray()[0][299],
                                     dest5.getReadArray()[0][598], 0.001f);
        dest5 = ado::resampleBuffer (source5, 176400);
        //ado::coutBuffer(dest5);                                     // and again
        expectWithinAbsoluteError (source5.getReadArray()[0][299],
                                     dest5.getReadArray()[0][1196], 0.001f);
    }

    beginTest ("16 channels, 441->480");

    {
        ado::Buffer source6 {16, 30};
        source6.fillAscending();
        ado::Buffer dest6 = ado::resampleBuffer (source6, 48000);
        expectWithinAbsoluteError (33.5712f, dest6.getReadArray()[15][31], 0.001f);   // check many channels
        expectWithinAbsoluteError (33.5712f, dest6.getReadArray()[14][31], 0.001f);   // i.e. don't carry garbage
        expectWithinAbsoluteError (33.5712f, dest6.getReadArray()[5][31], 0.001f);    // from previous channel pass
    }
}

//--------//--------//--------//--------//--------//--------//--------//--------

AIDIO_DECLARE_UNIT_TEST_WITH_STATIC_INSTANCE(Convolution)

Convolution::Convolution() : UnitTest ("Convolution") {}

void Convolution::runTest()
{
    beginTest ("Different sample rates");

    {
        ado::Buffer h {1, 16};      // 16x 1.0f values in...
        h.fillAllOnes();

        ado::Convolution engine {h};

        int blockSize = 40;
        ado::Buffer block {1, blockSize};
        block.getWriteArray()[0][0] = 1.0f; // kronecker

        engine.process (block);     // run one block
        //ado::coutBuffer (block);

        engine.resampleIrOnRateChange (88200);       // change rate to double

        block.clear();              // clear last output & reset kronecker
        block.getWriteArray()[0][0] = 1.0f;

        engine.process (block);     // run one block and sum
        //ado::coutBuffer (block);

        float sum = ado::rawBufferSum (block.getReadArray(), block.getNumChannels(), block.getNumSamples());

        expectWithinAbsoluteError (sum, 16.f, 0.5f); // we should get 32x (approx) 0.5f values.
    }

    beginTest ("4 channel convolution");

    {
        ado::Buffer h {4, 1};
        h.getWriteArray()[0][0] = 1;    // NOTE! these MUST be different
        h.getWriteArray()[1][0] = 2;    // Otherwise WDL will treat as a mono
        h.getWriteArray()[2][0] = 3;    // impulse and just convolve ch1&2 (not 3&4!!!)
        h.getWriteArray()[3][0] = 4;

        ado::Convolution engine {h};

        int blockSize = 4;
        ado::Buffer block {4, blockSize};
        block.getWriteArray()[0][0] = 1.0f; // kronecker
        block.getWriteArray()[1][0] = 2.0f; // kronecker * 2
        block.getWriteArray()[2][0] = 3.0f; // kronecker * 3
        block.getWriteArray()[3][0] = 4.0f; // kronecker * 4

        engine.process (block);     // run one block and sum
        //ado::coutBuffer (block);

        float sum = ado::rawBufferSum (block.getReadArray(), 4, blockSize);

        expectEquals (sum, 1 + 4 + 9 + 16.0f);
    }

    beginTest ("Delta through 9 point moving average");

    {
        ado::Buffer h {1, 9};
        h.fillAllOnes(); // moving average

        ado::Convolution engine {h};

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

        expectEquals (sum, 9.0f);
    }

    beginTest ("Delta through 256 point moving average");

    {
        ado::Buffer h {1, 256};
        h.fillAllOnes(); // moving average

        ado::Convolution engine {h};

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

        expectEquals (sum, 256.0f);
    }


    beginTest ("Ascending sequence with delta, 2 channels");

    {
        const int channels {2};
        ado::Buffer h {channels, 8};
        h.fillAscending(); // h[n] = {1,2,3,4,5,6,7,8}

        ado::Convolution engine {h};

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

        expectEquals (sum, 36.0f * channels); // octave: sum(conv(h, x))
    }

    beginTest ("Ascending sequence with ascending sequence");

    {
        const int channels {2};
        ado::Buffer h {channels, 8};
        h.fillAscending(); // h[n] = {1,2,3,4,5,6,7,8}

        ado::Convolution engine {h};

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

        expectEquals (sum, 1296.0f * channels); // octave: sum(conv(h, x))
    }

    beginTest ("Long ascending sequence with ascending sequence");

    {
        const int channels {2};
        const int ascendingSequenceLength {1000};
        ado::Buffer h {channels, ascendingSequenceLength};
        h.fillAscending(); // octave h = 1:1000

        ado::Convolution engine {h};

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

        expectWithinAbsoluteError (sum, 5.010006e+11f, 0.0001f); // octave: sum(conv(h, h))
    }

    beginTest ("set()");

    {
        ado::Buffer h {1, 2};
        h.fillAllOnes();

        ado::Convolution engine {h};

        ado::Buffer block {1, 4};
        block.getWriteArray()[0][0] = 1.0f; // kronecker

        engine.process (block);     // run one block and sum

        float sum = ado::rawBufferSum (block.getReadArray(), 1, 4);

        expectEquals (sum, 2.0f);

        block.clear();
        engine.process(block);

        ado::Buffer hh {1, 2};
        hh.fillAscending();      // hh[n] = {1,2}

        engine.set(hh); // set new impulse

        block.clear();
        block.getWriteArray()[0][0] = 1.0f; // kronecker

        engine.process (block);     // run one block and sum

        sum = ado::rawBufferSum (block.getReadArray(), 1, 4);

        expectEquals (sum, 3.0f);
    }
}

//--------//--------//--------//--------//--------//--------//--------//--------

AIDIO_DECLARE_UNIT_TEST_WITH_STATIC_INSTANCE(Buffer)

Buffer::Buffer() : UnitTest ("Buffer") {}

void Buffer::runTest()
{
    beginTest ("Constructs");
    expectDoesNotThrow (ado::Buffer (2, 512));

    beginTest ("Must have channels");
    expectThrows (ado::Buffer (0, 512));

    beginTest ("Must have samples");
    expectThrows (ado::Buffer (2, 0));

    beginTest ("Must have +ve samples");
    expectThrows (ado::Buffer (2, -1));

    beginTest ("No ridiculous numChannels");
    expectThrows (ado::Buffer (size_t(2147483647)+12039812, 0));

    beginTest ("clearAndResize()");

    {
        ado::Buffer source7 {2, 12};
        source7.fillAscending();
        expectEquals (source7.getReadArray()[1][10], 11.f);
        source7.clearAndResize(4, 30);
        expectEquals (source7.getReadArray()[3][29], 0.f);
        source7.fillAscending();
        expectEquals (source7.getReadArray()[3][29], 30.f);
    }

    beginTest ("clearAndResize() with sampling rate");

    {
        ado::Buffer source7 {2, 12};
        expectEquals (source7.getSampleRate(), 44100);
        source7.fillAscending();
        expectEquals (source7.getReadArray()[1][10], 11.f);
        source7.clearAndResize(4, 30, 88200);
        expectEquals (source7.getReadArray()[3][29], 0.f);
        expectEquals (source7.getSampleRate(), 88200);
        source7.fillAscending();
        expectEquals (source7.getReadArray()[3][29], 30.f);
    }

    beginTest ("clearAndResize() must clear pointer array first");

    {
        ado::Buffer source7 {2, 12};
        source7.fillAscending();
        expectEquals (source7.getReadArray()[1][11], 12.f);
        source7.clearAndResize(4, 30);
        source7.fillAscending();
        expectEquals (source7.getReadArray()[2][29], 30.f);
        source7.clearAndResize(1, 9);
        expectEquals (source7.getReadArray()[0][7], 0.f);
    }

    beginTest ("numChannels()");

    {
        ado::Buffer b {4, 1024};
        expectEquals (b.getNumChannels(), 4);
    }

    beginTest ("numSamples()");

    {
        ado::Buffer b {4, 1024};
        expectEquals (b.getNumSamples(), 1024);
    }

    beginTest ("getWriteArray()");

    {
        ado::Buffer b {4, 1024};
        float** bb {b.getWriteArray()};
        int chan = 2;
        int samp = 385;
        expectEquals (bb[chan][samp], 0.0f);
        bb[chan][samp] = 0.239f;
        expectEquals (bb[chan][samp], 0.239f);
    }

    beginTest ("fillAllOnes()");

    {
        ado::Buffer b {4, 1024};
        expectDoesNotThrow (b.fillAllOnes());
        float elementSum = 0.0f;
        for (int i = 0; i < b.getNumSamples(); ++i)
            elementSum += b.getWriteArray()[0][i];
        expectEquals (elementSum, static_cast<float> (b.getNumSamples()));
    }

    beginTest ("fillAscending()");

    {
        ado::Buffer b {64, 4};
        expectDoesNotThrow (b.fillAscending());
        float elementSum = 0.0f;
        for (int i = 0; i < b.getNumSamples(); ++i)
            elementSum += b.getWriteArray()[0][i];
        expectEquals (elementSum, 1.f + 2.f + 3.f + 4.f);
    }

    beginTest ("Clear()");

    {
        ado::Buffer b {4, 1024};
        expectDoesNotThrow (b.fillAllOnes());
        expectDoesNotThrow (b.clear());
        float elementSum = 0.0f;
        for (int i = 0; i < b.getNumSamples(); ++i)
            elementSum += b.getWriteArray()[0][i];
        expectEquals (elementSum, 0.0f);
    }

    beginTest ("getSamplingRate()");

    {
        ado::Buffer b {4, 1024, 96000};
        const int rate = b.getSampleRate();
        expectEquals (rate, 96000);
    }

    beginTest ("bufferSumElements()");

    {
        ado::Buffer b {1, 5};
        b.fillAscending();
        const float sum {ado::bufferSumElements(b)};
        expectEquals (sum, 1.f + 2 + 3 + 4 + 5);
    }

    beginTest ("operator *= ()");

    {
        ado::Buffer b {1, 128};
        b.fillAllOnes();
        float sum {ado::bufferSumElements(b)};
        expectEquals (sum, 128.0f);
        b *= 0.5f;
        sum = ado::bufferSumElements(b);
        expectEquals (sum, 64.0f);
    }
}

//--------//--------//--------//--------//--------//--------//--------//--------

AIDIO_DECLARE_UNIT_TEST_WITH_STATIC_INSTANCE(Utility)

Utility::Utility() : UnitTest ("Utility") {}

void Utility::runTest()
{
    beginTest("rawBufferEquals()");

    {
        ado::Buffer a {2, 2048};
        ado::Buffer b {2, 2048};
        b.fillAllOnes();
        expect (! ado::rawBufferEquals (a.getReadArray(), b.getReadArray(), 2, 2048));
        a.fillAllOnes();
        expect   (ado::rawBufferEquals (a.getReadArray(), b.getReadArray(), 2, 2048));

    }

    beginTest("rawBufferCopy()");

    {
        ado::Buffer source {2, 2048};
        ado::Buffer dest   {2, 2048};
        source.fillAllOnes();
        expect (! ado::rawBufferEquals (source.getReadArray(), dest.getReadArray(), 2, 2048));
        ado::rawBufferCopy (source.getReadArray(), dest.getWriteArray(), 2, 2048);
        expect   (ado::rawBufferEquals (source.getReadArray(), dest.getReadArray(), 2, 2048));
    }

    beginTest("rawBufferSum()");

    {
        ado::Buffer buffer {2, 2048};
        buffer.fillAllOnes();
        expectEquals (ado::rawBufferSum (buffer.getReadArray(), 1, 2048), 2048.0f);
    }

    beginTest("nextPowerOf2()");

    {
        int x = 55;
        expectEquals(ado::nextPowerOf2(x), 64);
        x = -234;
        expectEquals(ado::nextPowerOf2(x), 1);
        x = 16300;
        expectEquals(ado::nextPowerOf2(x), 16384);
    }
}