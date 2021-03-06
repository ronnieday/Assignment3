// ////////////////////////////////////////////// //
// Assignment 3 - Music 265A - Stanford/CCRMA     //
// Polyphonic, MIDI Enabled, Synth Plug-In        //
//                                                //
// Written by: Ron Guglielmone                    //
// Date: October 17, 2016                         //
//                                                //
// Notes: TitleComponent just adds the words      //
//        "Tone Colors" to the main window.       //
//                                                //
// ////////////////////////////////////////////// //


#ifndef TITLECOMPONENT_H_INCLUDED
#define TITLECOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class TitleComponent   : public Component
{
public:
    TitleComponent()
    {
    }

    ~TitleComponent()
    {
    }
    
    void paint (Graphics& g) override
    {
        g.setColour (Colours::lightgrey);
        g.setFont (30.0f);
        g.drawText ("Tone Colors", 0,0,250,30, 1, true);        }


    
private:
 
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TitleComponent)
};

#endif  // TITLECOMPONENT_H_INCLUDED
