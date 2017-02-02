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
#include "ImpulseLoaderAsync.h"

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
    /** Returns the current list of parameters for writing. */
    OwnedArray<AudioProcessorParameter>& getParametersForWriting() noexcept;

    //==============================================================================
    jdo::StateAB stateAB;
    jdo::StatePresets statePresets;

    enum ParamNames                             // Avoid confusing indices when
    {                                           // instantiating each jdo::SliderStep
        bypassName,                             // in the plugin editor's constructor
        reverbTypeName,                         // NOTE: MUST be same order as below
        mixName,
        gainName
    };

private:
    //==============================================================================
    jdo::CustomLook look;

    jdo::ParamStep* bypassParam;                // Raw pointers here. Processor's
    jdo::ParamStep* reverbTypeParam;            // managedParameters OwnedArray
    jdo::ParamStep* mixParam;                   // owns and manages. (See xtor.)
    jdo::ParamStep* gainParam;

    ado::Buffer ir;
    ado::Convolution engine;

    ImpulseLoaderAsync impulseLoaderAsync;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Processor)
};

#endif  // PLUGINPROCESSOR_H_INCLUDED
