/*
  ==============================================================================

    ParameterTest.cpp
    Created: 8 Jul 2016 9:00:36pm
    Author:  John Flynn

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Dependencies/Aidio/Aidio.h"

#include "../Parameter.h"

//==============================================================================

AIDIO_DECLARE_UNIT_TEST_WITH_STATIC_INSTANCE(ParameterTest)

ParameterTest::ParameterTest() : UnitTest ("ParameterTest") {}

//==============================================================================
class ParamStepTestsProc  : public AudioProcessor   // Dummy processor for use in tests below
{
public:
    ParamStepTestsProc()
        : stepSizeParam     {new jdo::ParamStepBroadcast  {"sid", "sname", "slab",  0.01f,    3, 0.75,  0, 0}},
          gainParam         {new jdo::ParamStepListenGain {"gid", "gname", "glab",   -10,    10,    0, 20, 0, *stepSizeParam}},
          numFreqStepsParam {new jdo::ParamStepBroadcast  {"sid", "sname", "slab",     1,     7,    5,  6, 0}},
          freqParam         {new jdo::ParamStepListenFreq {"gid", "gname", "glab",    20, 20000,  200,  0, 3, *numFreqStepsParam}}
    {
        addParameter (stepSizeParam);
        addParameter (gainParam);
        addParameter (numFreqStepsParam);
        addParameter (freqParam);
    }

    ~ParamStepTestsProc() {}

    void prepareToPlay (double, int) override {}
    void releaseResources() override {}
/*
   #ifndef JucePlugin_PreferredChannelConfigurations
    bool setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet) override {}
   #endif
*/
    void processBlock (AudioSampleBuffer&, MidiBuffer&) override {}

    AudioProcessorEditor* createEditor() override { return nullptr; }
    bool hasEditor() const override { return true; }

    const String getName() const override { return ""; }

    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return true; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 0; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const String getProgramName (int) override { return ""; }
    void changeProgramName (int, const String&) override {}

    void getStateInformation (MemoryBlock&) override {}
    void setStateInformation (const void*, int) override {}

private:
    jdo::ParamStepBroadcast* stepSizeParam {nullptr};            // 0
    jdo::ParamStepListenGain* gainParam {nullptr};               // 1
    jdo::ParamStepBroadcast* numFreqStepsParam {nullptr};        // 2
    jdo::ParamStepListenFreq* freqParam {nullptr};               // 3

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParamStepTestsProc)
};

//==============================================================================
void ParameterTest::runTest()
{
    Random rnd = getRandom();

    beginTest ("min < max");
    expectDoesNotThrow ((jdo::ParamStep {"dummyID", "The Name", "dB", 0, 10, 0, 0, 0}));
    expectThrows       ((jdo::ParamStep {"dummyID", "The Name", "dB", 0, -1, 0, 0, 0}));

    beginTest ("min <= default");
    expectThrows       ((jdo::ParamStep {"dummyID", "The Name", "dB", 0, 10, -5, 0, 0}));
    expectDoesNotThrow ((jdo::ParamStep {"dummyID", "The Name", "dB", 0, 10,  0, 0, 0}));

    beginTest ("default <= max");
    expectThrows       ((jdo::ParamStep {"dummyID", "The Name", "dB", 0, 10, 20, 0, 0}));
    expectDoesNotThrow ((jdo::ParamStep {"dummyID", "The Name", "dB", 0, 10, 10, 0, 0}));

    beginTest ("0 <= numSteps");
    expectThrows       ((jdo::ParamStep {"dummyID", "The Name", "dB", 0, 10,  0, -1, 0}));
    expectDoesNotThrow ((jdo::ParamStep {"dummyID", "The Name", "dB", 0, 10,  0, 0, 0}));

    beginTest ("set any skew");
    for (int i = 0; i < 1000; ++i)
        expectDoesNotThrow ((jdo::ParamStep {"dummyID", "The Name", "dB", 0, 10,  0, 0, rnd.nextFloat()}));

    jdo::ParamStep freq  {"id", "nm", "lb", 20, 20000, 632.456f,   0, 3};
    jdo::ParamStep steps {"id", "nm", "lb",  0,   127,        0, 128, 0};

    beginTest ("get() default");
    expect (freq.get() == 632.456f);

    beginTest ("getNumSteps()");
    expect (freq .getNumSteps() == 0);
    expect (steps.getNumSteps() == 128);

    beginTest ("setNumSteps()");
    expectDoesNotThrow (steps.setNumSteps(10));
    expect (steps.getNumSteps() == 10);
    expectThrows (steps.setNumSteps(-5));

    beginTest ("getRange()");
    ado::RangeLog range {20, 20000, 3};
    expect (freq.getRange() == range);

    beginTest ("setRange(f,f,f)");
    expectDoesNotThrow (freq.setRange (0, 1, 0));
    ado::RangeLog range010 {0, 1, 0};
    expect (freq.getRange() == range010);

    beginTest ("Create ParamStepBroadcast");
    expectDoesNotThrow ((jdo::ParamStepBroadcast {"id", "name", "label", -10, 10, 0, 20, 0}));

    beginTest ("Create jdo::ParamStepBroadcast and jdo::ParamStepListenGain in dummy processor");
//    expectDoesNotThrow (ParamStepTestsProc());                                            // leaks and doesn't update on stepsize change!!!
//    ParamStepTestsProc paramStepTestsProc;

/*
    beginTest ("Change stepSize parameter");
    expect (paramStepTestsProc.getParameters()[0]->getValue() != 1.0f);
    expectDoesNotThrow (paramStepTestsProc.getParameters()[0]->setValueNotifyingHost (1.0f)); // +3dB steps
    expect (paramStepTestsProc.getParameters()[0]->getValue() == 1.0f);

    beginTest ("Check listening gain parameter's range changed");
    ado::RangeLog expectedRange {-30, 30, 0}; // now +3dB steps, so 10 steps times 3 is +/-30
    jdo::ParamStepListenGain& gainParam = dynamic_cast<jdo::ParamStepListenGain&> (*paramStepTestsProc.getParameters()[1]);
    expect (gainParam.getRange() == expectedRange);

    beginTest ("Create jdo::ParamStepBroadcast and jdo::ParamStepListenFreq in dummy processor");
    expectDoesNotThrow (ParamStepTestsProc());

    beginTest ("Change numFreqSteps parameter");
    expect (paramStepTestsProc.getParameters()[2]->getValue() != 1.0f);
    expectDoesNotThrow (paramStepTestsProc.getParameters()[2]->setValueNotifyingHost (1.0f));
    expect (paramStepTestsProc.getParameters()[2]->getValue() == 1.0f);

    beginTest ("Check listening freq parameter numSteps changed");
    jdo::ParamStepListenFreq& freqParam = dynamic_cast<jdo::ParamStepListenFreq&> (*paramStepTestsProc.getParameters()[3]);
    expect (freqParam.getNumSteps() == 192);
*/}
