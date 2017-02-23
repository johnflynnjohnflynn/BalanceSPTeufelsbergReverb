/*
  ==============================================================================

    TestConvolution.cpp
    Created: 11 Jan 2017 2:48:12pm
    Author:  John Flynn

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Aidio/Aidio.h"

//==============================================================================

#if AIDIO_UNIT_TESTS

AIDIO_DECLARE_UNIT_TEST_WITH_STATIC_INSTANCE(Convolution)

Convolution::Convolution() : UnitTest ("Convolution") {}

void Convolution::runTest()
{
    /*beginTest ("Convolve");

    {
        juce::AudioBuffer<float> buf {1, 16};                       // subtle pointer weirdness going on!
        ado::BufferView buffer {ado::makeBufferView (buf)};
        buffer.fillAllOnes();

        juce::AudioBuffer<float> imp {1, 8};
        ado::BufferView ir {ado::makeBufferView (imp)};
        ir.fillAscending();

        //ado::Convolver;

        //ado::coutBuffer (buffer);
        //buffer.fillAllOnes();
        //ado::coutBuffer (buffer);

        //std::terminate();
    }*/
/*
    beginTest("Convolver");

    {
        ado::Buffer sig {1, 4};
        ado::Buffer imp {1, 2};       // out size must be sig + imp - 1 !!!
        ado::Buffer out {1, sig.getNumSamples() + imp.getNumSamples() - 1};
        sig.fillAllOnes();
        imp.fillAscending();

        ado::Convolver eng {imp, out.getNumSamples()};
        eng.convolve (sig, out);
        //ado::coutBuffer(out);

        auto o = out.getReadArray();
        expectWithinAbsoluteError<float> (o[0][0], 1, 0.001);
        expectWithinAbsoluteError<float> (o[0][1], 3, 0.001);
        expectWithinAbsoluteError<float> (o[0][2], 3, 0.001);
        expectWithinAbsoluteError<float> (o[0][3], 3, 0.001);
        expectWithinAbsoluteError<float> (o[0][4], 2, 0.001);
    }

    beginTest("Convolver 2 channels");

    {
        ado::Buffer sig {2, 4};
        ado::Buffer imp {2, 2};       // out size must be sig + imp - 1 !!!
        ado::Buffer out {2, sig.getNumSamples() + imp.getNumSamples() - 1};
        sig.fillAllOnes();
        imp.fillAscending();

        ado::Convolver eng {imp, out.getNumSamples()};
        eng.convolve (sig, out);
        //ado::coutBuffer(out);

        auto o = out.getReadArray();
        expectWithinAbsoluteError<float> (o[1][0], 1, 0.001);
        expectWithinAbsoluteError<float> (o[1][1], 3, 0.001);
        expectWithinAbsoluteError<float> (o[1][2], 3, 0.001);
        expectWithinAbsoluteError<float> (o[1][3], 3, 0.001);
        expectWithinAbsoluteError<float> (o[1][4], 2, 0.001);
    }

    beginTest("Convolver performance");                                         // not a test!

    {
        ado::Buffer sig {1, 64};
        ado::Buffer imp {1, 8192};       // out size must be sig + imp - 1 !!!
        ado::Buffer out {1, sig.getNumSamples() + imp.getNumSamples() - 1};
        sig.fillAllOnes();
        imp.fillAscending();

        ado::Convolver eng {imp, out.getNumSamples()};

        juce::PerformanceCounter pc ("1SecConv2Sec", 5, File {});
        pc.start();

        eng.convolve (sig, out); // will ouput correctly 8192 + 64 - 1 samples

        pc.stop();

        std::cout << "\nAre you in DEBUG MODE?!?!?!\n\n";
    }

    beginTest("Old Convolution performance");                                   // not a test!

    {
        ado::Buffer sig {1, 8192};
        ado::Buffer imp {1, 64};       // out size must be sig + imp - 1 !!!
        ado::Buffer out {1, sig.getNumSamples() + imp.getNumSamples() - 1};
        sig.fillAllOnes();
        imp.fillAscending();

        ado::Convolution eng {imp};

        juce::PerformanceCounter pc ("1SecConv2Sec", 5, File {});
        pc.start();

        eng.process (sig); // will output 63 too few samples, but how and ever,

        pc.stop();

        std::cout << "\nAre you in DEBUG MODE?!?!?!\n\n";
    }

    beginTest("Convolver sliced");                                         // not a test!

    {
        ado::Buffer sig {2, 64};
        ado::Buffer imp {2, 8000};
        ado::Buffer out {2, sig.getNumSamples() + imp.getNumSamples() - 1}; // L+M-1
        sig.fillAllOnes();
        imp.fillAscending();

        ado::Convolver eng {imp, out.getNumSamples()};

        eng.sliceConvolve (sig, out);

        DBG("boo");
    }
*/
    //==============================================================================

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

#endif // AIDIO_UNIT_TESTS
