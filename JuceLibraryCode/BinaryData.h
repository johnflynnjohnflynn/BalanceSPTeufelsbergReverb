/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#ifndef BINARYDATA_H_11574160_INCLUDED
#define BINARYDATA_H_11574160_INCLUDED

namespace BinaryData
{
    extern const char*   Europa_wav;
    const int            Europa_wavSize = 1764146;

    extern const char*   layout04knob01dotofffs8_png;
    const int            layout04knob01dotofffs8_pngSize = 331;

    extern const char*   layout04knob01dotonfs8_png;
    const int            layout04knob01dotonfs8_pngSize = 3299;

    extern const char*   layout04knob01offfs8_png;
    const int            layout04knob01offfs8_pngSize = 5119;

    extern const char*   layout04knob01onfs8_png;
    const int            layout04knob01onfs8_pngSize = 7072;

    extern const char*   layout04NoKnobsfs8_png;
    const int            layout04NoKnobsfs8_pngSize = 209191;

    extern const char*   PercussionAir_wav;
    const int            PercussionAir_wavSize = 198590;

    extern const char*   PercussionAirShort_wav;
    const int            PercussionAirShort_wavSize = 834;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Number of elements in the namedResourceList array.
    const int namedResourceListSize = 8;

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes) throw();
}

#endif
