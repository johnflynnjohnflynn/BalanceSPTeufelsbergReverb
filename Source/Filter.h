/*
  ==============================================================================

    Filter.h
    Created: 15 Sep 2016 1:10:29pm
    Author:  John Flynn

  ==============================================================================
*/

#ifndef FILTER_H_INCLUDED
#define FILTER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "OrfanidisCalc.h"

namespace filter
{

//==============================================================================
/** One band of Orfanidis-based parametric bell EQ.
*/
class ParametricBand
{
public:
    ParametricBand ();

    void reset (double sampleRate);
    void process (AudioSampleBuffer& buffer, int numChannels, float gain, float freq, float q);

private:
    OrfanidisCalc coeffs;

    std::vector<double> xn_1; // state for up to 8 chans
    std::vector<double> xn_2;
    std::vector<double> yn_1;
    std::vector<double> yn_2;

    float denormal {1.0e-8f}; // use to keep mantissa from dropping below 1.xxx
                              // see http://www.earlevel.com/main/2012/12/03/a-note-about-de-normalization/
                              // also see Falco's DspFilters MathSupplement.h re: 1e-8?
};

//==============================================================================
} // namespace filter

#endif  // FILTER_H_INCLUDED
