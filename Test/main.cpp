//==============================================================================
/** Make sure these are defined as preprocessor definitions
    AIDIO_UNIT_TESTS=1
    gsl_CONFIG_CONTRACT_VIOLATION_THROWS=1
    NOMINMAX=1
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Aidio/Aidio.h"

int main(int argc, const char* argv[])
{
    juce::UnitTestRunner runner;
    runner.runAllTests();

    return 0;
}
