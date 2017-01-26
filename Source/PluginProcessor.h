/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Judio/Judio.h"

//==============================================================================
/**
*/
class Processor  : public AudioProcessor
{
public:
    //==============================================================================
    Processor();
    ~Processor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    jdo::StateAB stateAB;
    jdo::StatePresets statePresets;

private:
    //==============================================================================
    jdo::CustomLook look;

    // jdo::ParamStepBroadcast*  gainStepSizeParam; // Raw pointers here. Processor's
    jdo::ParamStep* bypassParam;                    // managedParameters OwnedArray
    jdo::ParamStep* reverbTypeParam;                // owns and manages. (See xtor.)
    jdo::ParamStep* mixParam;
    jdo::ParamStep* gainParam;

    ado::Buffer ir {1, 1};
    ado::Convolution engine {ir};

    int currentImpulse {0};
    void changeImpulse (int newImpulse);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Processor)
};

#endif  // PLUGINPROCESSOR_H_INCLUDED
