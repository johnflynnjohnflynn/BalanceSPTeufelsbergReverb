/*
  ==============================================================================

    TestUtility.cpp
    Created: 11 Jan 2017 2:49:11pm
    Author:  John Flynn

  ==============================================================================
*/
/*
#include "../../Dependencies/JuceLibraryCode/JuceHeader.h"
#include "../../Aidio.h"

//==============================================================================

#if AIDIO_UNIT_TESTS

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

    beginTest("rawBufferDownmix4To2()");

    {
        ado::Buffer buffer {4, 6};
        buffer.getWriteArray()[0][3] = 1.f;
        buffer.getWriteArray()[1][3] = 2.f;
        buffer.getWriteArray()[2][3] = 3.f;
        buffer.getWriteArray()[3][3] = 4.f;
        ado::rawBufferDownmix4To2 (buffer.getWriteArray(), buffer.getNumSamples());
        expectEquals (buffer.getReadArray()[0][3], 1 + 3.f);
        expectEquals (buffer.getReadArray()[1][3], 2 + 4.f);
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

#endif // AIDIO_UNIT_TESTS*/
