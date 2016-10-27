
// ////////////////////////////////////////////// //
// Assignment 3 - Music 265A - Stanford/CCRMA     //
// Polyphonic, MIDI Enabled, Synth Plug-In        //
//                                                //
// Written by: Ron Guglielmone                    //
// Date: October 27, 2016                         //
//                                                //
// Notes:                                         //
//                                                //
//                                                //
// ////////////////////////////////////////////// //



#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "TitleComponent.h"



class TutorialPluginAudioProcessorEditor  : public AudioProcessorEditor,
                                            private Slider::Listener,
                                            private Button::Listener,
                                            private Colour
{
    
    
public:
    TutorialPluginAudioProcessorEditor (TutorialPluginAudioProcessor&);
    ~TutorialPluginAudioProcessorEditor();
    
    // Public function prototypes:
    void paint (Graphics&) override;
    void resized() override;

private:
    
    // Private function prototypes:
    void sliderValueChanged (Slider* slider) override;
    void buttonClicked (Button* button) override;
    
    
    // Callback refrence to the processor:
    TutorialPluginAudioProcessor& processor;

    
    
    // ////////////////////////////////////////////// //
    //                                                //
    //            GUI Element Declarations            //
    //                                                //
    // ////////////////////////////////////////////// //
 
    // Rotary controls:
    Slider dial1;
    Slider dial2;
    Slider gainSlider;
    
    // Buttons for the harmonics:
    TextButton colorOne;
    TextButton colorTwo;
    TextButton colorThree;
    TextButton colorFour;
    TextButton colorFive;
    TextButton colorSix;
    
    // Labels:
    Label tremeloSpeedLabel;
    Label tremeloDepthLabel;
    Label gainLabel;
    TitleComponent colorTitle;
    
    // Colors:
    Colour backgroundColor;
    Colour buttonOneColor;
    Colour buttonTwoColor;
    Colour buttonThreeColor;
    Colour buttonFourColor;
    Colour buttonFiveColor;
    Colour buttonSixColor;
    
    
    
    
    // I have no idea what this thing does:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TutorialPluginAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
