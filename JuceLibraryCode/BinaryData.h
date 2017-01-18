/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#ifndef BINARYDATA_H_115001851_INCLUDED
#define BINARYDATA_H_115001851_INCLUDED

namespace BinaryData
{
    extern const char*   _4Channel_wav;
    const int            _4Channel_wavSize = 428;

    extern const char*   layout04knob01dotofffs8_png;
    const int            layout04knob01dotofffs8_pngSize = 331;

    extern const char*   layout04knob01dotonfs8_png;
    const int            layout04knob01dotonfs8_pngSize = 3299;

    extern const char*   layout04knob01offfs8_png;
    const int            layout04knob01offfs8_pngSize = 5119;

    extern const char*   layout04knob01onfs8_png;
    const int            layout04knob01onfs8_pngSize = 7072;

    extern const char*   Stereo64SamplesAllOnes_wav;
    const int            Stereo64SamplesAllOnes_wavSize = 428;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Number of elements in the namedResourceList array.
    const int namedResourceListSize = 6;

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes) throw();
}

#endif
