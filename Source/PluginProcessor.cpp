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
     : AudioProcessor (BusesProperties()                // Note position of ':' relies on this define!!!
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
    : stateAB {getParametersForWriting()},
      statePresets {getParametersForWriting(), "BalanceAudioTools/SPTeufelsbergReverb/presets.xml"},
                                         // ID            Name       Suffix          Min      Max     Def nSteps   skew broadcastParam
      bypassParam     {new jdo::ParamStep {"bypassID",   "Bypass",       "",    0.0f,     1.0f,   0.0f,    1        }},
      reverbTypeParam {new jdo::ParamStep {"revTypeID",  "Reverb Type",  "",    1.0f,     6.0f,   1.0f,    5        }},
      mixParam        {new jdo::ParamStep {"mixID",      "Mix",         "%",    0.0f,   100.0f,  50.0f,   64        }},
      gainParam       {new jdo::ParamStep {"gainID",     "Gain",       "dB",  -18.0f,    18.0f,   0.0f,   72        }},
      ir {1, 1},
      engine {ir},
      impulseLoaderAsync {engine, ir}
{
        // Set look here not in editor.
        // Needs to be set before editor's member variables are initialised     // better way?
    LookAndFeel::setDefaultLookAndFeel (&look);
    
        // addParameter()s to the processor's OwnedArray<AudioProcessorParameter>
        // managedParameters (which takes ownership and deletes appropriately)
    addParameter (bypassParam);
    addParameter (reverbTypeParam);
    addParameter (mixParam);
    addParameter (gainParam);

    impulseLoaderAsync.changeImpulseNow (1);
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
void Processor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    engine.resampleIrOnRateChange (sampleRate);
    dryBuffer.setSize (2, samplesPerBlock * 2); // extra safety size
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

void Processor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& /*midiMessages*/) noexcept
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    const int bufferNumChannels = buffer.getNumChannels();
    const int bufferNumSamples  = buffer.getNumSamples();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, bufferNumSamples);

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...

    bool isMonoToStereo = getTotalNumInputChannels()  == 1
                       && getTotalNumOutputChannels() == 2;
    if (isMonoToStereo)
        buffer.copyFrom(1, 0, buffer,               // dest chan, offset, buff
                        0, 0, bufferNumSamples);    //  src chan, offset, size

    const int newImpulse = static_cast<int> (*reverbTypeParam);
    impulseLoaderAsync.changeImpulseAsync (newImpulse);

    const bool impulseIsChanging {impulseLoaderAsync.isNowChanging()};  // lower gain on IR change
    if (impulseIsChanging)
        buffer.applyGain (0.25f);

    if (*bypassParam < 0.5f && ! impulseIsChanging) // bypass while changing impulse also
    {
        const float gainLin = Decibels::decibelsToGain<float> (*gainParam);
        const float mix = *mixParam / 100.0f; // range 0-1

        dryBuffer.makeCopyOf (buffer, true);                            // copy dry buffer

        engine.process (buffer.getArrayOfWritePointers(),               // convolve buffer
                        bufferNumChannels,
                        bufferNumSamples);
        buffer.applyGain (mix);

        for (int chan = 0; chan < bufferNumChannels; ++chan)            // mix back dry
            buffer.addFrom (chan, 0, dryBuffer,
                            chan, 0, dryBuffer.getNumSamples(),
                            1.0f - mix);

        buffer.applyGain (gainLin);                                     // apply gain from param
    }
    // else bypass
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
void Processor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    // Create an outer XML element..
    XmlElement xml ("MYPLUGINSETTINGS");

    // Store the values of all our parameters, using their param ID as the XML attribute
    for (int i = 0; i < getNumParameters(); ++i)
        if (AudioProcessorParameterWithID* p = dynamic_cast<AudioProcessorParameterWithID*> (getParameters().getUnchecked(i)))
            xml.setAttribute (p->paramID, p->getValue());

    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary (xml, destData);
}

void Processor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    // This getXmlFromBinary() helper function retrieves our XML from the binary blob..
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState != nullptr)
    {
        // make sure that it's actually our type of XML object..
        if (xmlState->hasTagName ("MYPLUGINSETTINGS"))
        {
            // Now reload our parameters..
            for (int i = 0; i < getNumParameters(); ++i)
                if (AudioProcessorParameterWithID* p = dynamic_cast<AudioProcessorParameterWithID*> (getParameters().getUnchecked(i)))
                    p->setValue ((float) xmlState->getDoubleAttribute (p->paramID, p->getValue()));
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Processor();
}

//==============================================================================
OwnedArray<AudioProcessorParameter>& Processor::getParametersForWriting() noexcept
{
        // hideous cast, but why no non-const getParameters() in AudioProcessor ?
    return const_cast<OwnedArray<AudioProcessorParameter>&> (getParameters());
}
