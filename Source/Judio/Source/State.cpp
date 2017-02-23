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

#include "Judio/State.h"
#include "Judio/Look.h"

namespace jdo
{

//==============================================================================
void saveStateToXml (const OwnedArray<AudioProcessorParameter>& params, XmlElement& xml)
{
    xml.removeAllAttributes(); // clear first

    for (const auto& param : params)
        if (auto* p = dynamic_cast<AudioProcessorParameterWithID*> (param))
            xml.setAttribute (p->paramID, p->getValue()); // 0to1
}

void loadStateFromXml (const XmlElement& xml, OwnedArray<AudioProcessorParameter>& params)
{
    for (const auto& param : params)
        if (auto* p = dynamic_cast<AudioProcessorParameterWithID*> (param))
                                                                                  // if not in xml set current
            p->setValueNotifyingHost ((float) xml.getDoubleAttribute (p->paramID, p->getValue()));
}

//==============================================================================
StateAB::StateAB (OwnedArray<AudioProcessorParameter>& params)
    : parameters {params}
{
    copyAB();
}

void StateAB::toggleAB()
{
    XmlElement temp {"Temp"};
    saveStateToXml (parameters, temp); // current to temp
    loadStateFromXml (ab, parameters); // ab to current
    ab = temp;                         // temp to ab
}

void StateAB::copyAB()
{
    saveStateToXml (parameters, ab);
}

//==============================================================================
bool createFileIfNonExistant (const File& file) // returns true if file didn't exist
{
    if (! file.exists())
    {
        file.create();
        return true;
    }
    jassert (file.exists());
    return false;
}

void parseStringToXmlElement (const String& string, XmlElement& xml)                // what could go wrong here?
{
    ScopedPointer<XmlElement> parsed {XmlDocument::parse (string)};
    if (parsed)
        xml = *parsed;
}

void parseFileToXmlElement (const File& file, XmlElement& xml)                  // what could go wrong here?
{
    ScopedPointer<XmlElement> parsed {XmlDocument::parse (file)};
    if (parsed)
        xml = *parsed;
}

void writeXmlElementToFile (const XmlElement& xml, File& file)
{
    createFileIfNonExistant (file);
    xml.writeToFile (file, "");         // "" is DTD (unused)
}

String getNextAvailablePresetID (const XmlElement& presetXml)
{
    int newPresetIDNumber = presetXml.getNumChildElements() + 1; // 1 indexed to match ComboBox
    return "preset" + static_cast<String> (newPresetIDNumber);   // format: preset##
}

//==============================================================================
StatePresets::StatePresets (OwnedArray<AudioProcessorParameter>& params, const String& presetFileLocation)
    : parameters {params},
      presetFile {File::getSpecialLocation (File::userApplicationDataDirectory)
                                            .getChildFile (presetFileLocation)}
{
    parseStringToXmlElement (defaultPresetsString, defaultPresetsXml);
    parseFileToXmlElement (presetFile, presetXml);

    if (createFileIfNonExistant (presetFile))
        writeXmlElementToFile (defaultPresetsXml, presetFile); 
}

void StatePresets::loadPreset (int presetID)
{
    if (1 <= presetID && presetID <= presetXml.getNumChildElements()) // 1 indexed to match ComboBox
    {
        XmlElement loadThisChild {*presetXml.getChildElement (presetID - 1)}; // (0 indexed method)
        loadStateFromXml (loadThisChild, parameters);
    }
    // else do nothing, on preset delete, refreshing will try to load a non-existent presetID

    currentPresetID = presetID; // allow 0 for 'no preset selected' (?)
}

void StatePresets::savePresetToDisk (const String& presetName)
{
    String newPresetID = getNextAvailablePresetID (presetXml); // presetID format: "preset##"

    ScopedPointer<XmlElement> currentState {new XmlElement {newPresetID}};  // must be pointer as
    saveStateToXml (parameters, *currentState);                        // parent takes ownership
    currentState->setAttribute ("presetName", presetName);
    
    presetXml.addChildElement (currentState.release());                     // will be deleted by parent element

    writeXmlElementToFile (presetXml, presetFile);  // write changes to shared file on disk
}

void StatePresets::deletePresetFromDisk()
{
    XmlElement* childToDelete {presetXml.getChildElement (currentPresetID - 1)};
    if (childToDelete)
        presetXml.removeChildElement (childToDelete, true);

    writeXmlElementToFile (presetXml, presetFile);  // write changes to shared file on disk
}

StringArray StatePresets::getPresetNamesFromDisk()
{
    parseFileToXmlElement (presetFile, presetXml); // refresh from disk

    StringArray names;

    forEachXmlChildElement(presetXml, child)                                    // should avoid macro?
    {
        String n = child->getStringAttribute("presetName");
        if (n == "")
            n = "(Unnamed preset)";
        names.add (n);
    }

    return names; // hopefully moves
}

int StatePresets::getNumPresets() const
{
    return presetXml.getNumChildElements();
}

int StatePresets::getCurrentPresetId() const
{
    return currentPresetID;
}

//==============================================================================
void populateComboBox (ComboBox& comboBox, const StringArray& listItems)
{
    for (int i = 0; i < listItems.size(); ++i)
        comboBox.addItem (listItems[i], i + 1); // 1-indexed ID for ComboBox
}

//==============================================================================
StateComponent::StateComponent (StateAB& sab, StatePresets& sp)
    : procStateAB {sab},
      procStatePresets {sp},
      toggleABButton {"A-B"},
      copyABButton {"Copy"},
      presetBox {"PresetBoxID", this},
      savePresetButton {"Save"},
      deletePresetButton {"Delete"}
{
    addAndMakeVisible (toggleABButton);
    addAndMakeVisible (copyABButton);
    toggleABButton.addListener (this);
    copyABButton.addListener (this);

    addAndMakeVisible (presetBox);
    presetBox.setTextWhenNothingSelected("Load preset...");
    refreshPresetBoxFromDisk();
    ifPresetActiveShowInBox();
    presetBox.addListener (this);

    addAndMakeVisible (savePresetButton);
    savePresetButton.addListener (this);
    addAndMakeVisible (deletePresetButton);
    deletePresetButton.addListener (this);
}

void StateComponent::paint (Graphics& /*g*/)
{
}

void StateComponent::resized()
{
    const int sideBorder {20};
    const int spacer {5};

    juce::Rectangle<int> r (getLocalBounds());                // preset and state bar (explicit juce:: for MSVC2013)

    r.removeFromTop   (77);
    r.removeFromLeft  (sideBorder);
    r.removeFromRight (sideBorder);
    r.removeFromBottom(r.getHeight() - CustomLook::buttonHeight);

    toggleABButton    .setBounds (r.removeFromLeft (CustomLook::buttonWidth));
    r.removeFromLeft  (spacer);
    copyABButton      .setBounds (r.removeFromLeft (CustomLook::buttonWidth));

    deletePresetButton.setBounds (r.removeFromRight (CustomLook::buttonWidth));
    r.removeFromRight (spacer);
    presetBox         .setBounds (r.removeFromRight (CustomLook::buttonWidth * 4));
    r.removeFromRight (spacer);
    savePresetButton  .setBounds (r.removeFromRight (CustomLook::buttonWidth));
}

void StateComponent::buttonClicked (Button* clickedButton)
{
    if (clickedButton == &toggleABButton)     procStateAB.toggleAB();
    if (clickedButton == &copyABButton)       procStateAB.copyAB();
    if (clickedButton == &savePresetButton)   savePresetAlertWindow();
    if (clickedButton == &deletePresetButton) deletePresetAndRefresh();
}

void StateComponent::comboBoxChanged (ComboBox* changedComboBox)
{
    const int selectedId {changedComboBox->getSelectedId()};
    procStatePresets.loadPreset (selectedId);
}

void StateComponent::refreshPresetBoxFromDisk()
{
    presetBox.clear();
    StringArray presetNames {procStatePresets.getPresetNamesFromDisk()};

    StringArray empty = StringArray();

    if (presetNames != empty)
        populateComboBox (presetBox, presetNames);
    else
        presetBox.getRootMenu()->addItem(1, "No presets", false, false);
}

void StateComponent::ifPresetActiveShowInBox()
{
    const int currentPreset {procStatePresets.getCurrentPresetId()};
    const int numPresets    {procStatePresets.getNumPresets()};
    if (1 <= currentPreset && currentPreset <= numPresets)
        presetBox.setSelectedId (currentPreset, dontSendNotification);
}

void StateComponent::deletePresetAndRefresh()
{
    procStatePresets.deletePresetFromDisk();
    refreshPresetBoxFromDisk();
}

void StateComponent::savePresetAlertWindow()
{
    enum choice { ok, cancel };

    AlertWindow alert   {"Save preset...", "", AlertWindow::AlertIconType::NoIcon};
    alert.addTextEditor ("presetEditorID", "Enter preset name");
    alert.addButton     ("OK",     choice::ok,     KeyPress (KeyPress::returnKey, 0, 0));
    alert.addButton     ("Cancel", choice::cancel, KeyPress (KeyPress::escapeKey, 0, 0));
    
    if (alert.runModalLoop() == choice::ok)                                     // LEAKS when quit while open !!!
    {
        String presetName {alert.getTextEditorContents ("presetEditorID")};

        procStatePresets.savePresetToDisk (presetName);
        refreshPresetBoxFromDisk();
        presetBox.setSelectedId (procStatePresets.getNumPresets());
    }
}

void StateComponent::changeListenerCallback (ChangeBroadcaster* source)
{
    refreshPresetBoxFromDisk();
}

} // namespace jdo
