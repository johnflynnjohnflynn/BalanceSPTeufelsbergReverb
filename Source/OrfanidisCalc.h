/*
  ==============================================================================

    OrfanidisCalc.h
    Created: 22 Jun 2016 3:54:00pm
    Author:  John Flynn

  ==============================================================================
*/

#ifndef ORFANIDISCALC_H_INCLUDED
#define ORFANIDISCALC_H_INCLUDED

#include <cmath>


//==============================================================================
/**
    Example usage...

    OrfanidisCalc c;
    c.calculateCoefficients (1, 2, 1.75, 0.25*pi, 0.4*pi);
    cout << c.b0() << c.b1() << c.b2() << c.a0() << c.a1() << c.a2() << endl;
    
    ---

    From the matlab/octave code...

    % peq.m - Parametric EQ with matching gain at Nyquist frequency
    % Sophocles J. Orfanidis, J. Audio Eng. Soc., vol.45, p.444, June 1997.
    %
    % Usage:  [b, a, G1] = peq(G0, G, GB, w0, Dw)
    %
    % G0 = reference gain at DC
    % G  = boost/cut gain
    % GB = bandwidth gain
    %
    % w0 = center frequency in rads/sample
    % Dw = bandwidth in rads/sample
    %
    % b  = [b0, b1, b2] = numerator coefficients
    % a  = [1,  a1, a2] = denominator coefficients
    % G1 = Nyquist-frequency gain

    function [b, a, G1] = peq(G0, G, GB, w0, Dw)

    F   = abs(G^2  - GB^2);
    G00 = abs(G^2  - G0^2);
    F00 = abs(GB^2 - G0^2);

    num = G0^2 * (w0^2 - pi^2)^2 + G^2 * F00 * pi^2 * Dw^2 / F;
    den = (w0^2 - pi^2)^2 + F00 * pi^2 * Dw^2 / F;

    G1 = sqrt(num/den);

    G01 = abs(G^2  - G0*G1);
    G11 = abs(G^2  - G1^2);
    F01 = abs(GB^2 - G0*G1);
    F11 = abs(GB^2 - G1^2);

    W2 = sqrt(G11 / G00) * tan(w0/2)^2;
    DW = (1 + sqrt(F00 / F11) * W2) * tan(Dw/2);

    C = F11 * DW^2 - 2 * W2 * (F01 - sqrt(F00 * F11));
    D = 2 * W2 * (G01 - sqrt(G00 * G11));

    A = sqrt((C + D) / F);
    B = sqrt((G^2 * C + GB^2 * D) / F);

    b = [(G1 + G0*W2 + B), -2*(G1 - G0*W2), (G1 - B + G0*W2)] / (1 + W2 + A);
    a = [1, [-2*(1 - W2), (1 + W2 - A)] / (1 + W2 + A)];
*/
class OrfanidisCalc
{
public:
    //==============================================================================
    OrfanidisCalc() {}

    double b0() { return b0_; };
    double b1() { return b1_; };
    double b2() { return b2_; };
    double a0() { return 1.0; };
    double a1() { return a1_; };
    double a2() { return a2_; };

    void setRate (double newSampleRate)
    {
        jassert (newSampleRate > 0);
        samplerate = newSampleRate;
    }

    double linToHz (double lin)
    {
        jassert (0 <= lin && lin <= 1);
        return 20 * std::pow (10, 3 * lin); // 3 decades in freq scale
    }

    double hzToRadPerSamp (double hz)
    {
        jassert (0 <= hz && hz <= (samplerate / 2));
        return hz * ((2 * pi) / samplerate);
    }

    void calculate (double gain, double frequency, double q)
    {
        const double gainLin  = Decibels::decibelsToGain (gain);
        const double bandwidthGain = Decibels::decibelsToGain (gain / root2);
        const double hzFrequency = linToHz (frequency);
        const double radSampFrequency = hzToRadPerSamp (hzFrequency);
        const double radSampBandwidth = radSampFrequency / (1.588308819 * q); // q scaling factor 1.58...
/*
        String str;
        str << "gainLin          " << gainLin << "\n"                                 // debug
            << "bandwidthGain    " << bandwidthGain << "\n"
            << "hzFrequency      " << hzFrequency << "\n"
            << "radSampFrequency " << radSampFrequency << "\n"
            << "radSampBandwidth " << radSampBandwidth << "\n";
        Logger::outputDebugString (str);
*/
        calculateCoefficients (1, gainLin, bandwidthGain, radSampFrequency, radSampBandwidth);
    }

    //==============================================================================
    void calculateCoefficients (double G0, double G, double GB, double w0, double Dw)
    {
        if (G == GB)    // if no boost or cut, pass audio
        {
            b0_ = 1;
            b1_ = 0;
            b2_ = 0;
            a1_ = 0;
            a2_ = 0;
        }
        else            // else calculate coefficients
        {
            const double F   = std::abs (G*G   - GB*GB);
            const double G00 = std::abs (G*G   - G0*G0);
            const double F00 = std::abs (GB*GB - G0*G0);

            const double num = G0*G0 * std::pow (w0*w0 - pi*pi, 2) + G*G * F00 * pi*pi * Dw*Dw / F;
            const double den = std::pow (w0*w0 - pi*pi, 2) + F00 * pi*pi * Dw*Dw / F;

            const double G1  = std::sqrt (num/den);

            const double G01 = std::abs (G*G  - G0*G1);
            const double G11 = std::abs (G*G  - G1*G1);
            const double F01 = std::abs (GB*GB - G0*G1);
            const double F11 = std::abs (GB*GB - G1*G1);

            const double W2 = std::sqrt (G11 / G00) * std::pow (std::tan (w0/2), 2);
            const double DW = (1 + std::sqrt (F00 / F11) * W2) * std::tan (Dw/2);

            const double C = F11 * DW*DW - 2 * W2 * (F01 - std::sqrt (F00 * F11));
            const double D = 2 * W2 * (G01 - std::sqrt (G00 * G11));

            const double A = std::sqrt ((C + D) / F);
            const double B = std::sqrt ((G*G * C + GB*GB * D) / F);

            b0_ = (G1 + G0*W2 + B) / (1 + W2 + A);
            b1_ = -2 * ((G1 - G0*W2) / (1 + W2 + A));
            b2_ = (G1 - B + G0*W2) / (1 + W2 + A);
            a1_ = -2 * ((1 - W2) / (1 + W2 + A)),
            a2_ = (1 + W2 - A) / (1 + W2 + A);
        }
    }

private:
    double b0_ {0};  // filter transfer function coefficients
    double b1_ {0};
    double b2_ {0};
    double a1_ {0};
    double a2_ {0};

    double samplerate {0};
    
    const double pi    {3.141592653589793115997963468544185161590576171875};
    const double root2 {1.4142135623730951454746218587388284504413604736328125};

};


#endif  // ORFANIDISCALC_H_INCLUDED
