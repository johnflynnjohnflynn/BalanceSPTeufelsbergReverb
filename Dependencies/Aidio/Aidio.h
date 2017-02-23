//==============================================================================
/*
    The MIT License (MIT)

    Copyright (c) 2016 John Flynn

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
    deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/
//==============================================================================

#ifndef AIDIO_H_INCLUDED
#define AIDIO_H_INCLUDED

//==============================================================================
/** Parent header for the library
    
    @example    #include "Aidio/Aidio.h"
    
    Add the folder that holds the main 'Aidio' folder to your header search 
    paths.
    
    E.g. for a JUCE project:
    - put main 'Aidio' folder in 'Source' folder.
    - add "../../Source" in 'Header search paths' in Projucer (it's relative to
      Xcode project file).
    - #include "Aidio/Aidio.h"
*/

#include "Utility.h"
#include "Buffer.h"
#include "Convolution.h"
#include "Maths.h"
#include "Resampling.h"
#include "Test.h"

//==============================================================================
/** Note on dependencies. TLDR: If you use JUCE, no need to worry!!!

    Dependent libraries are included by relative folder paths...
    e.g. #include "Aidio/Dependencies/WDL/convoengine.h"
    
    EXCEPT for JUCE! Which is the regular JUCE way...
    e.g. "../JuceLibraryCode/JuceHeader.h"
    
    This allows a parent JUCE project to use its own JUCE modules.
    
    Since Projucer handles modules automatically, we don't want user's projects
    linking .h by relative folders inside Aidio, to .cpp (possibly different
    JUCE versions) in their own projects. Causing some horrible linker errors.
    
    TODO: Find optimimum solution for end-user, both JUCE and non-JUCE projects!
*/

#endif  // AIDIO_H_INCLUDED
