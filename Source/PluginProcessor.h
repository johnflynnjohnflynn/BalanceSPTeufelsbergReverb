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
#include "OrfanidisCalc.h"
#include "Filter.h"

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

    jdo::ParamStepBroadcast*  gainStepSizeParam;   // Raw pointers here. Processor's
    jdo::ParamStepBroadcast*  freqStepSizeParam;   // managedParameters OwnedArray
                                                         // owns and manages. (See xtor.)
    jdo::ParamStepListenGain* gain1Param;
    jdo::ParamStepListenFreq* freq1Param;
    jdo::ParamStep*           q1Param;
    jdo::ParamStepListenGain* gain2Param;
    jdo::ParamStepListenFreq* freq2Param;
    jdo::ParamStep*           q2Param;
    jdo::ParamStepListenGain* gain3Param;
    jdo::ParamStepListenFreq* freq3Param;
    jdo::ParamStep*           q3Param;
    jdo::ParamStepListenGain* gain4Param;
    jdo::ParamStepListenFreq* freq4Param;
    jdo::ParamStep*           q4Param;
    jdo::ParamStepListenGain* gain5Param;
    jdo::ParamStepListenFreq* freq5Param;
    jdo::ParamStep*           q5Param;
    jdo::ParamStepListenGain* gain6Param;
    jdo::ParamStepListenFreq* freq6Param;
    jdo::ParamStep*           q6Param;
    jdo::ParamStepListenGain* gain7Param;
    jdo::ParamStepListenFreq* freq7Param;
    jdo::ParamStep*           q7Param;

    filter::ParametricBand band1;
    filter::ParametricBand band2;
    filter::ParametricBand band3;
    filter::ParametricBand band4;
    filter::ParametricBand band5;
    filter::ParametricBand band6;
    filter::ParametricBand band7;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Processor)
};

#endif  // PLUGINPROCESSOR_H_INCLUDED
