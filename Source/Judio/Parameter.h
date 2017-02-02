//--------//--------//--------//--------//--------//--------//--------//--------
/*
  ==============================================================================

   This file follows the same open source licensing as the JUCE library.
   Copyright (c) 2016 John Flynn

   Permission is granted to use this software under the terms of either:
   a) the GPL v2 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   Judio is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ==============================================================================
*/
//--------//--------//--------//--------//--------//--------//--------//--------

#ifndef PARAMETER_H_INCLUDED
#define PARAMETER_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"
#include "Aidio/Aidio.h"

namespace jdo
{

//==============================================================================
/** Modification of AudioParameterFloat using a jf::RangeLog.
    
    A subclass of AudioProcessorParameter that provides an easy way to create a
    parameter which maps onto a given RangeLog.
    
    Keeps a numSteps member but doesn't implement stepping (handled on slider
    side only).

    @see AudioParameterFloat
*/
class ParamStep  : public AudioProcessorParameterWithID
{
public:
    ParamStep (String parameterID,  // no spaces
               String name,         // spaces allowed
               String labelSuffix,
               float minValue,
               float maxValue,
               float defaultValue,
               int numSteps = 0,
               float skewLog = 0.0f);

    ~ParamStep() {}

    float get() const noexcept                  { return value; }
    operator float() const noexcept             { return value; } // allows dereference access

    int getNumSteps() const noexcept override   { return numSteps; }
    void setNumSteps (int newNumSteps);

    const ado::RangeLog& getRange() const        { return range; }
    void setRange (float start, float end, float skewLog);

protected:
    float value;

private:
    ado::RangeLog range;
    float defaultValue;
    int numSteps;

    void setValue (float newValue) override;
    float getValue() const override;
    float getDefaultValue() const override;
    String getText (float, int) const override;
    float getValueForText (const String&) const override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParamStep)
};

//==============================================================================
/** Subclass of ParamStep that allows listeners to be registered. Broadcasts to
    listeners whenever parameter changes.
    
    Use in conjuntion with:
    @see ParamStepListenGain
*/
class ParamStepBroadcast  : public ParamStep,
                            public ChangeBroadcaster
{
public:
    ParamStepBroadcast (String parameterID,  // no spaces
                        String name,         // spaces allowed
                        String labelSuffix,
                        float minValue,
                        float maxValue,
                        float defaultValue,
                        int numSteps = 0,
                        float skewLog = 0.0f);

private:
    void setValue (float newValue) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParamStepBroadcast)
};

//==============================================================================
/** Subclass of ParamStep that acts as a listener to a given broadcast param.

    Used for gain controls with a broadcasting step size parameter. A change in
    step size alters this parameter's range accordingly.
    
    Centre always zero. Number of steps constant.
    
    @see ParamStepBroadcast
*/
class ParamStepListenGain  : public ParamStep,
                             public ChangeListener
{
public:
    ParamStepListenGain (String parameterID,  // no spaces
                         String name,         // spaces allowed
                         String labelSuffix,
                         float minValue,
                         float maxValue,
                         float defaultValue,
                         int numSteps,
                         float skewLog,
                         ParamStepBroadcast& stepParam);

private:
    ParamStepBroadcast& stepSizeParam;
    
    void changeListenerCallback (ChangeBroadcaster* source) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParamStepListenGain)
};

//==============================================================================
/** Subclass of ParamStep that acts as a listener to a given broadcast param.

    Used for frequency controls to allow variable number of steps. Range 
    start/end stays constant.
    
    @see ParamStepBroadcast
*/
class ParamStepListenFreq  : public ParamStep,
                             public ChangeListener
{
public:
    ParamStepListenFreq (String parameterID,  // no spaces
                         String name,         // spaces allowed
                         String labelSuffix,
                         float minValue,
                         float maxValue,
                         float defaultValue,
                         int numSteps,
                         float skewLog,
                         ParamStepBroadcast& stepsParam);

private:
    ParamStepBroadcast& numStepsParam;
    
    void changeListenerCallback (ChangeBroadcaster* source) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParamStepListenFreq)
};

//==============================================================================
} // namespace jdo

#endif  // PARAMETER_H_INCLUDED
