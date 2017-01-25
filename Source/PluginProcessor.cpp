/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Processor::Processor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)  // Change JohnFlynn/TestParameters02/presets.xml !!!
                      #endif                                                    // Remove stepSize param???
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),                                                       // now reliant on this define!!!
#endif
      stateAB {*this},
      statePresets {*this, "JohnFlynn/TestParameters02/presets.xml"}, // ID     Name      Min      Max     Def nSteps   skew broadcastParam
      // gainStepSizeParam {new jdo::ParamStepBroadcast  {"gainStepID", "Gain Step Size",  0.05f,     3.0f,   0.5f              }},
      reverbTypeParam   {new jdo::ParamStep           {"RevTypeID",  "Reverb Type",     1.0f,      5.0f,   1.0f,    4        }},
      mixParam          {new jdo::ParamStep           {"mixID",      "Mix",              0.0f,   100.0f,  50.0f,   64        }},
      gainParam         {new jdo::ParamStep           {"gainID",     "Gain",           -36.0f,     0.0f,   0.0f,   72        }}
{
    LookAndFeel::setDefaultLookAndFeel (&look);
    
        // addParameter()s to the processor's OwnedArray<AudioProcessorParameter>
        // managedParameters (which takes ownership and deletes appropriately)
    // addParameter (gainStepSizeParam);
    addParameter (reverbTypeParam);
    addParameter (mixParam);
    addParameter (gainParam);
}

Processor::~Processor()
{
}

//==============================================================================
const String Processor::getName() const
{
    return JucePlugin_Name;
}

bool Processor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Processor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double Processor::getTailLengthSeconds() const
{
    return 0.0;
}

int Processor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Processor::getCurrentProgram()
{
    return 0;
}

void Processor::setCurrentProgram (int /*index*/)
{
}

const String Processor::getProgramName (int /*index*/)
{
    return String();
}

void Processor::changeProgramName (int /*index*/, const String& /*newName*/)
{
}

//==============================================================================
void Processor::prepareToPlay (double sampleRate, int /*samplesPerBlock*/)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void Processor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Processor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Processor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& /*midiMessages*/)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...

    /*std::cout << getParameters()[1]->getName(64) << " " << (String) getParameters()[1]->getValue() << "\n";
    std::cout << "*freqStep  " << (String) *freqStepSizeParam << "\n";
    std::cout << getParameters()[3]->getName(64) << " " << (String) getParameters()[3]->getValue() << "\n";
    std::cout << "*freqparam " << (String) *freq1Param << "\n";
    std::cout << "*qparam " << (String) *q1Param << "\n";
    std::cout << "\n";*/
}

//==============================================================================
bool Processor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Processor::createEditor()
{
    return new Editor (*this);
}

//==============================================================================
void Processor::getStateInformation (MemoryBlock& /*destData*/)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Processor::setStateInformation (const void* /*data*/, int /*sizeInBytes*/)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Processor();
}
