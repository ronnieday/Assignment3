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




#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


class TutorialPluginAudioProcessor  : public AudioProcessor,
                                      public Component,
                                      public Colour,
                                      public ComboBox::Listener,
                                      public Button::Listener,
                                      public Slider::Listener,
                                      private MidiInputCallback {

public:
    // God giveth'
    TutorialPluginAudioProcessor();
    // And God taketh' away
    ~TutorialPluginAudioProcessor();

    // Function Prototypes:
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    
    
    
    // ////////////////////////////////////////////// //
    //          More Confusing JUCE Stuff             //
    //        ...that I don't understand...           //
    //           ...and didn't touch...               //
    // ////////////////////////////////////////////// //
    
    // ============================================================================== //
    AudioProcessorEditor* createEditor() override;
    // ============================================================================== //
    #ifndef JucePlugin_PreferredChannelConfigurations
    bool setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet) override;
    #endif
    // ============================================================================== //
    bool hasEditor() const override;
    const String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    // ============================================================================== //
    
    
private:
    // MIDI Input:
    ComboBox midiInputList;
    Label midiInputListLabel;
                                          
    // Confusing MIDI Stuff:
    AudioDeviceManager audioDeviceManager;
    MidiKeyboardState keyboardState;
    MidiKeyboardComponent keyboardComponent;
    AudioSourcePlayer audioSourcePlayer;
    SynthAudioSource synthAudioSource;
    int lastMidiInputIndex;
                                          
    // Globals:
    float** audioBuffer;
    double vibeSineFreq;
    int samplingRate;
    int nChans;
                                          
                                          
                                          
  
                                          
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TutorialPluginAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
