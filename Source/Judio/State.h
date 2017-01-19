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

#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"
#include "Judio/Button.h"

namespace jdo
{

//==============================================================================
void saveStateToXml (const AudioProcessor& processor, XmlElement& xml);
void loadStateFromXml (const XmlElement& xml, AudioProcessor& processor);

//==============================================================================
/** Handler for AB state toggling and copying in plugin.                        // improve descriptions
    Create public instance in processor and call .toggleAB() and .copyAB()
    methods from button callback in editor.
*/
class StateAB
{
public:
    explicit StateAB (AudioProcessor& p);
    
    void toggleAB();
    void copyAB();

private:
    AudioProcessor& pluginProcessor;
    XmlElement ab {"AB"};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StateAB);
};

//==============================================================================
void createFileIfNonExistant (const File& file);
void parseFileToXmlElement (const File& file, XmlElement& xml);
void writeXmlElementToFile (const XmlElement& xml, File& file);
String getNextAvailablePresetID (const XmlElement& presetXml);

//==============================================================================
/** Create StatePresets object with XML file saved relative to user
    data directory.
    e.g. StatePresets my_sps {"JohnFlynnPlugins/ThisPlugin/presets.xml"}
    Full path Mac  = ~/Library/JohnFlynnPlugins/ThisPlugin/presets.xml
*/
class StatePresets
{
public:
    StatePresets (AudioProcessor& proc, const String& presetFileLocation);
    ~StatePresets();

    void savePreset (const String& presetName); // preset already exists? confirm overwrite
    void loadPreset (int presetID);
    void deletePreset();
                     
    StringArray getPresetNames() const;
    int getNumPresets() const;
    int getCurrentPresetId() const;

private:
    AudioProcessor& pluginProcessor;
    XmlElement presetXml {"PRESETS"}; // local, in-plugin representation
    File presetFile;                  // on-disk representation
    int currentPresetID {0};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StatePresets);
};

//==============================================================================
/** GUI-side component for the State objects. Handles GUI visual layout and 
    logic of the state handlers.

    Make private member of the PluginEditor. Initialise with the StateAB
    and StatePresets objects (these should be public members of the
    PluginProcessor).
*/
class StateComponent  : public Component,
                        public Button::Listener,
                        public ComboBox::Listener
{
public:
    StateComponent (StateAB& sab, StatePresets& sp);
    
    void paint (Graphics&) override;
    void resized() override;

private:
    StateAB&      procStateAB;
    StatePresets& procStatePresets;

    jdo::CustomButton toggleABButton;
    jdo::CustomButton copyABButton;
    ComboBox          presetBox;
    jdo::CustomButton savePresetButton;
    jdo::CustomButton deletePresetButton;

    void buttonClicked (Button* clickedButton) override;
    void comboBoxChanged (ComboBox* changedComboBox) override;
    
    void refreshPresetBox();
    void ifPresetActiveShowInBox();
    void deletePresetAndRefresh();
    void savePresetAlertWindow();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StateComponent);
};

//==============================================================================
} // namespace jdo

#endif  // STATE_H_INCLUDED
