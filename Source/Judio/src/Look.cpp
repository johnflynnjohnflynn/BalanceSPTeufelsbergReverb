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

#include "Judio/Look.h"

namespace jdo
{

//--------//--------//--------//--------//--------//--------//--------//--------

CustomLook::CustomLook()
{
    const Colour almostWhite {0xffeeeeee};                    // Slider stylings set in SliderStep not here!!!
    const Colour notSoWhite  {0xffb0b0b0};
    const Colour grey20      {0xff202020};
    const Colour grey30      {0xff303030};
    const Colour grey50      {0xff505050};
    const Colour transparent {0x00000000};

    setColour (Slider::textBoxTextColourId,       notSoWhite);
    setColour (Slider::textBoxBackgroundColourId, transparent);
    setColour (Slider::textBoxHighlightColourId,  transparent);
    setColour (Slider::textBoxOutlineColourId,    transparent);
    setColour (Slider::thumbColourId,             grey50);

    setColour (TextButton::buttonColourId,   grey30);
    setColour (TextButton::buttonOnColourId, Colour (0xff0000ff));  // change something sensible later
    setColour (TextButton::textColourOffId,  notSoWhite);
    setColour (TextButton::textColourOnId,   almostWhite);

    setColour (ComboBox::backgroundColourId, grey30);
    setColour (ComboBox::textColourId,       almostWhite);
    setColour (ComboBox::outlineColourId,    grey30);
    setColour (ComboBox::buttonColourId,     grey30); // outline when focussed ?
    setColour (ComboBox::arrowColourId,      grey50);

    setColour (PopupMenu::backgroundColourId,             grey30);
    setColour (PopupMenu::textColourId,                   notSoWhite);
    setColour (PopupMenu::headerTextColourId,             almostWhite);
    setColour (PopupMenu::highlightedBackgroundColourId,  Colour (0xff404040));
    setColour (PopupMenu::highlightedTextColourId,        Colour (0xfff3f3f3));

    setColour (AlertWindow::backgroundColourId, grey20);
    setColour (AlertWindow::textColourId,       notSoWhite);
    setColour (AlertWindow::outlineColourId,    grey20);

    setColour (TextEditor::backgroundColourId,       grey30);
    setColour (TextEditor::textColourId,             notSoWhite);
    setColour (TextEditor::highlightColourId,        grey50);
    setColour (TextEditor::highlightedTextColourId,  almostWhite);
    setColour (TextEditor::outlineColourId,          transparent);
    setColour (TextEditor::focusedOutlineColourId,   grey30);
    setColour (TextEditor::shadowColourId,           grey20);
}

//==============================================================================
Typeface::Ptr CustomLook::getTypefaceForFont (const Font& /*font*/)
{
    auto typPtr = Typeface::createSystemTypefaceFor(BinaryData::OpenSansRegular_ttf,
                                                    BinaryData::OpenSansRegular_ttfSize);
    return typPtr;
}

//==============================================================================
void CustomLook::drawRotarySlider (Graphics& g,
                                         int x,                 // offset position
                                         int y,
                                         int width,
                                         int height,
                                         float sliderPos,       // 0to1,
                                         float rotaryStartAngle,
                                         float rotaryEndAngle,
                                         Slider& /*slider*/)
{
    width > height ?                    // Centre knob in largest square that fits in slider rectangle
        (x = (width - height) / 2)
       : y = (height - width) / 2;

    const int smallestSide {jmin (width, height)};
    const float scaleFactor = static_cast<float> (smallestSide) / knob.getWidth();

    g.drawImageTransformed (knob, AffineTransform{}.scaled (scaleFactor)
                                                   .translated (static_cast<float> (x), 
                                                                static_cast<float> (y)));

        // Calculate rotation
    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    const float rotationCentre = static_cast<float> (knob.getWidth()) / 2;
    AffineTransform rotationTransform;
    rotationTransform = rotationTransform.rotation (angle, rotationCentre, rotationCentre);

    g.drawImageTransformed (knobMarker, rotationTransform.scaled (scaleFactor)
                                                         .translated(static_cast<float> (x),
                                                                     static_cast<float> (y)));
}

Slider::SliderLayout CustomLook::getSliderLayout (Slider& slider)
{
    Slider::SliderLayout layout;

    layout.sliderBounds = slider.getLocalBounds();

    const int h = slider.getHeight();
    const int w = slider.getWidth();

    const int smallestSide {jmin (w, h)};

    const int textBoxWidth {55};
    const int textBoxHeight{18};

    if (slider.getSliderStyle() == Slider::SliderStyle::RotaryHorizontalVerticalDrag)
        layout.textBoxBounds = Rectangle<int> {w / 2 + 4,                       // x pos
                                              (h / 2 - smallestSide / 2) + 2,   // y pos
                                               textBoxWidth,
                                               textBoxHeight};
    else
        layout.textBoxBounds = Rectangle<int> {w / 2 - textBoxWidth / 2 + 17,   // x pos
                                               h / 2 - textBoxHeight / 2,       // y pos
                                               textBoxWidth,
                                               textBoxHeight};

    return layout;
}

class CustomLook::SliderLabelComp  : public Label
{
public:
    SliderLabelComp() : Label (String(), String()) {}

    void mouseWheelMove (const MouseEvent&, const MouseWheelDetails&) {}
};

Label* CustomLook::createSliderTextBox (Slider& slider)
{
    Label* const l = new SliderLabelComp();

    l->setFont(Font {fontSize + 1});

    l->setJustificationType (Justification::left);
    l->setKeyboardType (TextInputTarget::decimalKeyboard);

    l->setColour (Label::textColourId, slider.findColour (Slider::textBoxTextColourId));
    l->setColour (Label::backgroundColourId,
                  (slider.getSliderStyle() == Slider::LinearBar || slider.getSliderStyle() == Slider::LinearBarVertical)
                            ? Colours::transparentBlack
                            : slider.findColour (Slider::textBoxBackgroundColourId));
    l->setColour (Label::outlineColourId, slider.findColour (Slider::textBoxOutlineColourId));
    l->setColour (TextEditor::textColourId, slider.findColour (Slider::textBoxTextColourId));
    l->setColour (TextEditor::backgroundColourId,
                  slider.findColour (Slider::textBoxBackgroundColourId)
                        .withAlpha ((slider.getSliderStyle() == Slider::LinearBar || slider.getSliderStyle() == Slider::LinearBarVertical)
                                        ? 0.7f : 1.0f));
    l->setColour (TextEditor::outlineColourId, slider.findColour (Slider::textBoxOutlineColourId));
    l->setColour (TextEditor::highlightColourId, slider.findColour (Slider::textBoxHighlightColourId));

    return l;
}

//==============================================================================
Font CustomLook::getTextButtonFont (TextButton&, int /*buttonHeight*/)
{
    return Font (fontSize);
}

static void drawButtonShape (Graphics& g, const Path& outline, Colour baseColour, float height)
{
    g.setGradientFill (ColourGradient (baseColour.brighter (0.025f), 0.0f, 0.0f,
                                       baseColour.darker (0.025f), 0.0f, height, false));
    g.fillPath (outline);   // Don't draw outline, just fill
}

void CustomLook::drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour,
                                           bool isMouseOverButton, bool isButtonDown)
{
    Colour baseColour (backgroundColour.withMultipliedSaturation (button.hasKeyboardFocus (true) ? 1.1f : 1.0f)
                                       .withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.9f));

    if (isButtonDown || isMouseOverButton)
        baseColour = baseColour.contrasting (isButtonDown ? 0.1f : 0.05f);

    const bool flatOnLeft   = button.isConnectedOnLeft();
    const bool flatOnRight  = button.isConnectedOnRight();
    const bool flatOnTop    = button.isConnectedOnTop();
    const bool flatOnBottom = button.isConnectedOnBottom();

    float border = 0.0f;
    if (dynamic_cast<ToggleButton*> (&button))                      // hideous again!
        border = 16.0f;

    const float width  = button.getWidth() + border;
    const float height = button.getHeight() + border;
    const float pos = border / 2;

    if (width > 0 && height > 0)
    {
        const float cornerSize = 4.0f;

        Path outline;
        outline.addRoundedRectangle (pos, pos,
                                     width - 2 * border, height - 2 * border,
                                     cornerSize, cornerSize,
                                     ! (flatOnLeft  || flatOnTop),
                                     ! (flatOnRight || flatOnTop),
                                     ! (flatOnLeft  || flatOnBottom),
                                     ! (flatOnRight || flatOnBottom));

        if (auto* b = dynamic_cast<ToggleButton*> (&button))        // this is hideous! should clearly be
        {                                                           // handled in the function below
            if (b->getToggleState())
            {
                DropShadow dropShadow {Colour {0xff1eaedb},
                                       8,
                                       Point<int> {0, 0}};

                dropShadow.drawForPath (g, outline); // draw x2 for more density
                dropShadow.drawForPath (g, outline);
            }
        }

        drawButtonShape (g, outline, baseColour, height);
    }
}

void CustomLook::drawToggleButton (Graphics& g, ToggleButton& button,
                                   bool isMouseOverButton, bool isButtonDown)
{
    Colour backgroundColour {0xff0000ff};

        // darken when on
    button.getToggleState() ?
        backgroundColour = Colour {0xff272727}
      : backgroundColour = Colour {0xff303030};

    drawButtonBackground(g, button, backgroundColour, isMouseOverButton, isButtonDown);

        // text stuff
    if (button.getToggleState())
        g.setColour (button.findColour (TextButton::textColourOnId));
    else
        g.setColour (button.findColour (TextButton::textColourOffId));

    g.setFont (fontSize);

    g.drawFittedText (button.getButtonText(),
                      0, 0,
                      button.getWidth(), button.getHeight(),
                      Justification::centred, 10);
}

//==============================================================================
Font CustomLook::getComboBoxFont (ComboBox& /*box*/)
{
    return Font (fontSize);
}

//==============================================================================
Font CustomLook::getPopupMenuFont()
{
    return Font (fontSize);
}

//==============================================================================
} // namespace
