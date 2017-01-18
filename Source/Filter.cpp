/*
  ==============================================================================

    Filter.cpp
    Created: 15 Sep 2016 1:10:29pm
    Author:  John Flynn

  ==============================================================================
*/

#include "Filter.h"

namespace filter
{

//==============================================================================
ParametricBand::ParametricBand ()
{
    xn_1.resize (8, 0); // state for up to 8 chans
    xn_2.resize (8, 0);
    yn_1.resize (8, 0);
    yn_2.resize (8, 0);
}

void ParametricBand::reset (double sampleRate)
{
    xn_1.assign(8, 0); // clear unit delays
    xn_2.assign(8, 0);
    yn_1.assign(8, 0);
    yn_2.assign(8, 0);

    coeffs.setRate (sampleRate);
}

void ParametricBand::process (AudioSampleBuffer& buffer, int numChannels, float gain, float freq, float q)
{
    coeffs.calculate(gain, freq, q);

    const double b0 = coeffs.b0();
    const double b1 = coeffs.b1();
    const double b2 = coeffs.b2();
    const double a1 = coeffs.a1();
    const double a2 = coeffs.a2();

    float** yn = buffer.getArrayOfWritePointers();

    for (int s = 0; s < buffer.getNumSamples(); ++s)
    {
        for (int c = 0; c < numChannels; ++c)
        {
            yn[c][s] += denormal;

            const double xn = static_cast<double> (yn[c][s]); // perform calcs at double

                                          // direct form 1
            yn[c][s] = static_cast<float> (b0*xn + b1*xn_1[c] + b2*xn_2[c]
                                                 - a1*yn_1[c] - a2*yn_2[c]);

            xn_2[c] = xn_1[c];  // advance delay networks
            xn_1[c] = xn;
            yn_2[c] = yn_1[c];
            yn_1[c] = yn[c][s];
        }
    }

    denormal *= -1; // avoid removal by DC filter
}

//==============================================================================
} // namespace filter