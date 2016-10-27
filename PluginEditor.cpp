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


#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "TitleComponent.h"


// This is our main constructor:
TutorialPluginAudioProcessorEditor::TutorialPluginAudioProcessorEditor (TutorialPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
 
    // Main window:
    setSize (600, 300);
    
    // ////////////////////////////////////////////// //
    //                                                //
    //               GUI Elements                     //
    //                                                //
    // ////////////////////////////////////////////// //
    
    // Rotary controller for "depth" param:
    dial1.setSliderStyle (Slider::Rotary);
    dial1.setTextBoxStyle (Slider::TextBoxBelow, true, 50, 20);
    addAndMakeVisible (dial1);
    dial1.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(109, 114, 120));
    dial1.setColour(juce::Slider::textBoxTextColourId, juce::Colours::lightgrey);
    dial1.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    dial1.setColour(juce::Slider::rotarySliderFillColourId , juce::Colour(54, 64, 74));
    dial1.setRange (0.0, 1000.0);
    dial1.addListener(this);
    
    // Label for "depth" rotary:
    addAndMakeVisible(tremeloDepthLabel);
    tremeloDepthLabel.setText ("Depth", dontSendNotification);
    tremeloDepthLabel.attachToComponent (&dial1, false);
    tremeloDepthLabel.setJustificationType( Justification::centredBottom);
    tremeloDepthLabel.setColour(0x1000281, juce::Colours::lightgrey);
    
    // Rotary controller for "speed" param:
    dial2.setSliderStyle (Slider::Rotary);
    dial2.setTextBoxStyle (Slider::TextBoxBelow, true, 50, 20);
    addAndMakeVisible (dial2);
    dial2.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(109, 114, 120));
    dial2.setColour(juce::Slider::textBoxTextColourId, juce::Colours::lightgrey);
    dial2.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    dial2.setColour(juce::Slider::rotarySliderFillColourId , juce::Colour(54, 64, 74));
    dial2.setRange(1,50);
    dial2.addListener(this);
    
    // Label for "speed" rotary:
    addAndMakeVisible(tremeloSpeedLabel);
    tremeloSpeedLabel.setText ("Speed", dontSendNotification);
    tremeloSpeedLabel.setJustificationType( Justification::centredBottom);
    tremeloSpeedLabel.attachToComponent (&dial2, false);
    tremeloSpeedLabel.setColour(0x1000281, juce::Colours::lightgrey);
    
    // Gain slider:
    gainSlider.setSliderStyle (Slider::Rotary);
    gainSlider.setTextBoxStyle (Slider::TextBoxBelow, true, 50, 20);
    addAndMakeVisible (gainSlider);
    gainSlider.setRange (0.0, 1.0);
    gainSlider.setValue(0.5);
    gainSlider.addListener (this);
    gainSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(109, 114, 120));
    gainSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::lightgrey);
    gainSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    gainSlider.setColour(juce::Slider::rotarySliderFillColourId , juce::Colour(54, 64, 74));
    
    // Label for gain slider
    addAndMakeVisible(gainLabel);
    gainLabel.setText ("Gain", dontSendNotification);
    gainLabel.setJustificationType( Justification::centredBottom);
    gainLabel.attachToComponent (&gainSlider, false);
    gainLabel.setColour(0x1000281, juce::Colours::lightgrey);
    
    // Set background and buttons:
    backgroundColor = Colour(36, 37, 40);
    buttonOneColor = Colour(51,71,125);
    buttonTwoColor = Colour(75,95,155);
    buttonThreeColor = Colour(66,80,133);
    buttonFourColor = Colour(57,73,142);
    buttonFiveColor = Colour(55,72,137);
    buttonSixColor = Colour(61,77,139);
    
    // Color buttons:
    addAndMakeVisible (colorOne);
    colorOne.setColour(0x1000100, buttonOneColor);
    colorOne.addListener(this);
    addAndMakeVisible (colorTwo);
    colorTwo.setColour(0x1000100, buttonTwoColor);
    colorTwo.addListener(this);
    addAndMakeVisible (colorThree);
    colorThree.setColour(0x1000100, buttonThreeColor);
    colorThree.addListener(this);
    addAndMakeVisible (colorFour);
    colorFour.setColour(0x1000100, buttonFourColor);
    colorFour.addListener(this);
    addAndMakeVisible (colorFive);
    colorFive.setColour(0x1000100, buttonFiveColor);
    colorFive.addListener(this);
    addAndMakeVisible (colorSix);
    colorSix.setColour(0x1000100, buttonSixColor);
    colorSix.addListener(this);
    
    // Color Title:
    addAndMakeVisible(colorTitle);
    
    
}








// Called when buttons are clicked:
void TutorialPluginAudioProcessorEditor::buttonClicked (Button* button) {
    
    if(button == &colorOne) {
        colorOneSelected = true;
        colorTwoSelected = false;
        colorThreeSelected = false;
        colorFourSelected = false;
        colorFiveSelected = false;
        colorSixSelected = false;
    }
    if(button == &colorTwo) {
        colorOneSelected = false;
        colorTwoSelected = true;
        colorThreeSelected = false;
        colorFourSelected = false;
        colorFiveSelected = false;
        colorSixSelected = false;
    }
    if(button == &colorThree) {
        colorOneSelected = false;
        colorTwoSelected = false;
        colorThreeSelected = true;
        colorFourSelected = false;
        colorFiveSelected = false;
        colorSixSelected = false;
    }
    if(button == &colorFour) {
        colorOneSelected = false;
        colorTwoSelected = false;
        colorThreeSelected = false;
        colorFourSelected = true;
        colorFiveSelected = false;
        colorSixSelected = false;
    }
    if(button == &colorFive) {
        colorOneSelected = false;
        colorTwoSelected = false;
        colorThreeSelected = false;
        colorFourSelected = false;
        colorFiveSelected = true;
        colorSixSelected = false;
    }
    if(button == &colorSix) {
        colorOneSelected = false;
        colorTwoSelected = false;
        colorThreeSelected = false;
        colorFourSelected = false;
        colorFiveSelected = false;
        colorSixSelected = true;
    }
    
}






// Called when sliders are slid:
void TutorialPluginAudioProcessorEditor::sliderValueChanged (Slider* slider) {
  
    if (true){ //carrier.getSamplingRate() > 0.0
        
        if (slider == &gainSlider){
            gain = gainSlider.getValue();
        }
                else if (slider == &dial1) {
            globalIndex = dial1.getValue();
        }
        else if (slider == &dial2) {
            vibeSineFreq = dial2.getValue();
        }
    }

}





// Should probably implement this at some point:
TutorialPluginAudioProcessorEditor::~TutorialPluginAudioProcessorEditor()
{
}






// Draws shit onto the window:
void TutorialPluginAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll(backgroundColor); // Damn, that's a nice color.
}






// Gets called whenever the window is resized:
void TutorialPluginAudioProcessorEditor::resized()
{

    // Box containing the rotaries:
    const int border = 10;
    Rectangle<int> area = getLocalBounds(); {
        Rectangle<int> dialArea(0,50,400,150);
        const int third = (dialArea.getWidth() / 3);
        dial1.setBounds (dialArea.removeFromLeft(third).reduced(border));
        gainSlider.setBounds (dialArea.removeFromRight (third).reduced(border));
        dial2.setBounds (dialArea.reduced(border));
    }
    
    // Box containing the top tone pads:
    Rectangle<int> area3 = getLocalBounds(); {
        const int border3 = 5;
        Rectangle<int> toneArea2(400,80,185,55);
        const int third = (toneArea2.getWidth() / 3);
        colorOne.setBounds (toneArea2.removeFromLeft(third).reduced(border3));;
        colorTwo.setBounds (toneArea2.removeFromRight (third).reduced(border3));
        colorThree.setBounds (toneArea2.reduced(border3));
    }
    
    // Box containing the bottom tone pads:
    Rectangle<int> area2 = getLocalBounds(); {
        const int border2 = 5;
        Rectangle<int> toneArea(400,135,185,55);
        const int third = (toneArea.getWidth() / 3);
        colorFour.setBounds (toneArea.removeFromLeft(third).reduced(border2));;
        colorFive.setBounds (toneArea.removeFromRight (third).reduced(border2));
        colorSix.setBounds (toneArea.reduced(border2));
    }
    
    // Box containing the tone title:
    Rectangle<int> area4 = getLocalBounds(); {
        Rectangle<int> titleArea(418,43,200,55);
        colorTitle.setBounds(titleArea);
    }
    
    
}
