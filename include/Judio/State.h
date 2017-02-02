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
#include "Judio/Slider.h"
#include "Judio/Parameter.h"

namespace jdo
{

//==============================================================================
/** For all methods below that require a non-const OwnedArray<AudioProcessorParameter>&
    remember to make a...
        OwnedArray<AudioProcessorParameter>& getParametersForWriting();
    ...function in your derived PluginProcessor class, which just casts away const
    on AudioProcessor::getParameters()
*/
void saveStateToXml (const OwnedArray<AudioProcessorParameter>& params, XmlElement& xml);
void loadStateFromXml (const XmlElement& xml, OwnedArray<AudioProcessorParameter>& params);

//==============================================================================
/** Handler for AB state toggling and copying in plugin.                        // improve descriptions
    Create public instance in processor and call .toggleAB() and .copyAB()
    methods from button callback in editor.
*/
class StateAB
{
public:
    explicit StateAB (OwnedArray<AudioProcessorParameter>& params);
    
    void toggleAB();
    void copyAB();

private:
    OwnedArray<AudioProcessorParameter>& parameters;
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
    StatePresets (OwnedArray<AudioProcessorParameter>& params, const String& presetFileLocation);
    ~StatePresets() {}

    void loadPreset (int presetID);

    void savePresetToDisk (const String& presetName);
    void deletePresetFromDisk();
    StringArray getPresetNamesFromDisk();

    int getNumPresets() const;
    int getCurrentPresetId() const;

private:
    OwnedArray<AudioProcessorParameter>& parameters;
    XmlElement presetXml {"PRESETS"}; // local, in-plugin representation
    File presetFile;                  // on-disk representation
    int currentPresetID {0};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StatePresets);
};

//==============================================================================
/** We need to reload the presets from disk every time the mouse drops down the 
    combobox. (This syncs changes from other plugin instances)

    We need to call refreshPresetBoxFromDisk() which is a member of the parent. 
    This sends a callback up the stack which is caught by the parent.
    
    NOT a very elegant solution!!!
*/
class ComboBoxWithRefreshOnClick  : public ComboBox,
                                    public ChangeBroadcaster
{
public:
    ComboBoxWithRefreshOnClick (const String& componentName, ChangeListener* parentStateComponent)
        : ComboBox {componentName}
    {
        addChangeListener (parentStateComponent);
    }

    void showPopup()
    {
        sendSynchronousChangeMessage();
        ComboBox::showPopup();
    }
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
                        public ComboBox::Listener,
                        public ChangeListener
{
public:
    StateComponent (StateAB& sab, StatePresets& sp);
    
    void paint (Graphics&) override;
    void resized() override;

private:
    StateAB&      procStateAB;
    StatePresets& procStatePresets;

    TextButton                  toggleABButton;
    TextButton                  copyABButton;
    ComboBoxWithRefreshOnClick  presetBox;
    TextButton                  savePresetButton;
    TextButton                  deletePresetButton;

    void buttonClicked (Button* clickedButton) override;
    void comboBoxChanged (ComboBox* changedComboBox) override;

    void refreshPresetBoxFromDisk();
    void ifPresetActiveShowInBox();
    void deletePresetAndRefresh();
    void savePresetAlertWindow();

    void changeListenerCallback (ChangeBroadcaster* source); // for refreshing presetBox from disk on click

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StateComponent);
};

//==============================================================================
} // namespace jdo

#endif  // STATE_H_INCLUDED
