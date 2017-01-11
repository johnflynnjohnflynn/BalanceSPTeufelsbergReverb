/*
  ==============================================================================

    TestResample.cpp
    Created: 11 Jan 2017 2:44:06pm
    Author:  John Flynn

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Aidio/Aidio.h"

//--------//--------//--------//--------//--------//--------//--------//--------

#if AIDIO_UNIT_TESTS

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

#endif // AIDIO_UNIT_TESTS
