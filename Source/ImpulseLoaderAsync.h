/*
  ==============================================================================

    ImpulseLoaderAsync.h
    Created: 27 Jan 2017 7:36:15am
    Author:  John Flynn

  ==============================================================================
*/

#ifndef IMPULSELOADERASYNC_H_INCLUDED
#define IMPULSELOADERASYNC_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Judio/Judio.h"

//==============================================================================
/** Loads impulse from WAV file asynchronously

    (May be called from audio thread i.e. won't block.)
    
    Audio thread just sets a flag: nowChangingImpulseAsync = true
    Outside audio thread, a timer checks periodically if this flag is set and
    calls the loading routine appropriately
    
    @see juce::Timer, ado::Convolution, ado::Buffer
*/
class ImpulseLoaderAsync  : private Timer
{
public:
    ImpulseLoaderAsync (ado::Convolution& eng, ado::Buffer& impulse);

    void changeImpulseAsync (int newImpulse);
    void changeImpulseNow (int newImpulse) { changeImpulse (newImpulse); } // will block
    
    const bool isNowChanging() { return nowChangingImpulseAsync; }

private:
    int currentImpulse {-1};    // force initial load
    bool nowChangingImpulseAsync {false};
    
    ado::Convolution& engine;   // keep handles to processor members
    ado::Buffer& ir;

    void timerCallback() override;
    void changeImpulse (int newImpulse);
};

#endif  // IMPULSELOADERASYNC_H_INCLUDED
