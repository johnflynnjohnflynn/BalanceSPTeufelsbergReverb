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

#include "../Parameter.h"

namespace jdo
{

//==============================================================================
void checkParamStep (float val, float min, float max, int steps)
{
    jassert ((min < max)                      // invariants here
          && (min <= val) && (val <= max)
          && (0 <= steps));
}

//==============================================================================
ParamStep::ParamStep (String pid, String nm, String suffix, float minValue, float maxValue, float def, int steps, float ske)
   : AudioProcessorParameterWithID (pid, nm, suffix),
     value (def),
     range (minValue, maxValue, ske),
     defaultValue (def),
     numSteps (steps)
{
    checkParamStep (def, minValue, maxValue, steps);
}

void ParamStep::setNumSteps (int newNumSteps)
{
    checkParamStep (value, range.getStart(), range.getEnd(), newNumSteps);
    numSteps = newNumSteps;
}

void ParamStep::setRange(float start, float end, float skewLog)
{
    range.setStart (start);     // range does invariant checks
    range.setEnd (end);
    range.setSkewLog (skewLog);
}

// private:
float ParamStep::getValue() const                           { return range.convertTo0to1 (value); }
void ParamStep::setValue (float newValue)                   { value = range.convertFrom0to1 (newValue); }
float ParamStep::getDefaultValue() const                    { return range.convertTo0to1 (defaultValue); }
float ParamStep::getValueForText (const String& text) const { return range.convertTo0to1 (text.getFloatValue()); }

String ParamStep::getText (float v, int length) const
{
    String asText (range.convertFrom0to1 (v)); // see JUCE to constrain decimal places
    return length > 0 ? asText.substring (0, length) : asText;
}

//==============================================================================
ParamStepBroadcast::ParamStepBroadcast (String parameterID,  // no spaces
                                        String name,         // spaces allowed
                                        String labelSuffix,
                                        float minValue,
                                        float maxValue,
                                        float defaultValue,
                                        int numSteps,
                                        float skewLog)
    : ParamStep {parameterID, name, labelSuffix, minValue, maxValue, defaultValue, numSteps, skewLog}
{
}

// private:
void ParamStepBroadcast::setValue (float newValue)
{
    jassert (0.0f <= newValue && newValue <= 1.0f);
    value = getRange().convertFrom0to1 (newValue);

   #if AIDIO_UNIT_TESTS
    sendSynchronousChangeMessage(); // Unit test demands blocking synchronous call
   #endif

    sendChangeMessage(); // Async fine otherwise.
}

//==============================================================================
ParamStepListenGain::ParamStepListenGain (String parameterID,  // no spaces
                                          String name,         // spaces allowed
                                          String labelSuffix,
                                          float minValue,
                                          float maxValue,
                                          float defaultValue,
                                          int numSteps,
                                          float skewLog,
                                          ParamStepBroadcast& stepParam)
    : ParamStep {parameterID, name, labelSuffix, minValue, maxValue, defaultValue, numSteps, skewLog},
      stepSizeParam {stepParam}
{
    stepSizeParam.addChangeListener (this);
}

// private:
void ParamStepListenGain::changeListenerCallback (ChangeBroadcaster* source)
{
    if (source == &stepSizeParam)
    {
            // calc temp values
        const float newStepSize = stepSizeParam.get();

        const int steps = getNumSteps();
        const float oldMin = getRange().getStart();
        const float oldMax = getRange().getEnd();
        const float centre = oldMin + (oldMax - oldMin) / 2.0f;
        
        const float newMin = centre - (steps * newStepSize) / 2.0f;
        const float newMax = centre + (steps * newStepSize) / 2.0f;

            // hold parameter position before, to avoid clamping
        const float val0to1 = getRange().convertTo0to1 (get());

            // set range for parameter (warning this can clamp the value!)
        setRange (newMin, newMax, getRange().getSkewLog());

            // set value for parameter (setValueNotifyingHost expects 0to1)
        setValueNotifyingHost (val0to1);
    }
}

//==============================================================================
ParamStepListenFreq::ParamStepListenFreq (String parameterID,  // no spaces
                                          String name,         // spaces allowed
                                          String labelSuffix,
                                          float minValue,
                                          float maxValue,
                                          float defaultValue,
                                          int numSteps,
                                          float skewLog,
                                          ParamStepBroadcast& stepsParam)
    : ParamStep {parameterID, name, labelSuffix, minValue, maxValue, defaultValue, numSteps, skewLog},
      numStepsParam {stepsParam}
{
    numStepsParam.addChangeListener (this);
}

// private:
void ParamStepListenFreq::changeListenerCallback (ChangeBroadcaster* source)
{
    if (source == &numStepsParam)
    {
            // calc temp values
        const float numStepsPower = numStepsParam.get();
        const float newNumSteps = 1.5f * std::pow (2.0f, numStepsPower); // for [1:5] yields [3:48]
        setNumSteps (static_cast<int> (newNumSteps));

            // set value for parameter (setValueNotifyingHost expects 0to1)
        const float val0to1 = getRange().convertTo0to1 (get());
        setValueNotifyingHost (val0to1);
    }
}

//==============================================================================
} // namespace jdo
