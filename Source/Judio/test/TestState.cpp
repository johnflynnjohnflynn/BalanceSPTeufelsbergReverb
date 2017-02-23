/*
  ==============================================================================

    StateTest.cpp
    Created: 28 Jul 2016 9:46:22pm
    Author:  John Flynn

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Aidio/Aidio.h"

#include "Judio/State.h"
#include "Judio/Parameter.h"

//==============================================================================

                                            // now that we are using OwnedArray<AudioProcessorParameters>
                                            // we should make some robust multi instance tests here
                                            // testing the state functionality!!!

AIDIO_DECLARE_UNIT_TEST_WITH_STATIC_INSTANCE(StateTest)

StateTest::StateTest() : UnitTest ("StateTest") {}

void StateTest::runTest()
{
    beginTest("jdo::createFileIfNonExistant()");
    File file {File::getSpecialLocation (File::userApplicationDataDirectory)
               .getChildFile ("JohnFlynn/TestParameters02/unit-test-createFileIfNonExistant.xml")};
    expect (! file.exists());

    jdo::createFileIfNonExistant(file);
    expect (file.exists());

    jdo::createFileIfNonExistant(file);
    expect (file.exists());

    file.deleteFile();
    expect (! file.exists());

    beginTest("jdo::writeXmlElementToFile()");
    jdo::createFileIfNonExistant(file);

    XmlElement xml1 {"TEST"};
    xml1.setAttribute ("name", "Nigel");
    xml1.setAttribute ("age", "10");

    expectDoesNotThrow(jdo::writeXmlElementToFile(xml1, file));

    beginTest("jdo::parseFileToElement()");
    XmlElement xml2 {"CheckTEST"};
    expectDoesNotThrow(jdo::parseFileToXmlElement(file, xml2));

    expect (xml2.getStringAttribute("name") == "Nigel");
    expect (xml2.getStringAttribute("age") == "10");

    beginTest("jdo::getNextAvailablePresetID");
    expect (jdo::getNextAvailablePresetID(xml1) == "preset1");

    ScopedPointer<XmlElement> child {new XmlElement {"CHILD"}};
    child->setAttribute ("name", "Terry");
    child->setAttribute ("age", "11");
    xml1.addChildElement(child.release());

    expect (jdo::getNextAvailablePresetID(xml1) == "preset2");

    file.deleteFile();
}
