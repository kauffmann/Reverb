/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   Helvetica65Medium_22443_ttf;
    const int            Helvetica65Medium_22443_ttfSize = 50404;

    extern const char*   RobotoBold_ttf;
    const int            RobotoBold_ttfSize = 167336;

    extern const char*   New_Sync_option_Hz_org2_png;
    const int            New_Sync_option_Hz_org2_pngSize = 1055;

    extern const char*   New_Sync_option_note_org2_png;
    const int            New_Sync_option_note_org2_pngSize = 1074;

    extern const char*   Retrigger_Off_png;
    const int            Retrigger_Off_pngSize = 516;

    extern const char*   Retrigger_On_png;
    const int            Retrigger_On_pngSize = 556;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 6;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
