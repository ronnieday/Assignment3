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



#include "Sine.h"
#include "Smooth.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"



// ////////////////////////////////////////////// //
//        STUFF I'M SUPPOSED TO UNDERSTAND        //
//   (and therefore adjusted to various effect)   //
// ////////////////////////////////////////////// //


// Taboo global variables:
float gain;
double globalCarrierFrequency;
double globalModulatorFrequency;
double globalIndex;
// Color Button Trackers:
bool colorOneSelected = false;
bool colorTwoSelected = true;
bool colorThreeSelected = false;
bool colorFourSelected = false;
bool colorFiveSelected = false;
bool colorSixSelected = false;



// This needs to be here for...  Reasons...
struct FMSound : public SynthesiserSound {
    FMSound() {}
    bool appliesToNote (int /*midiNoteNumber*/) override        { return true; }
    bool appliesToChannel (int /*midiChannel*/) override        { return true; }
};



// FM synth voice:
struct FMVoice : public SynthesiserVoice {
    FMVoice():
    carrierFrequency(440.0),
    index(0.0),
    level(0.0),
    envelope(0.0),
    onOff (false),
    tailOff(true)
    {
        carrier.setSamplingRate(getSampleRate());
        modulator.setSamplingRate(getSampleRate());
    };
    
    
    
    bool canPlaySound (SynthesiserSound* sound) override {
        return dynamic_cast<FMSound*> (sound) != nullptr;
    }
    
    
    
    void startNote (int midiNoteNumber, float velocity,
                    SynthesiserSound*, int /*currentPitchWheelPosition*/) override {
        // converting MIDI note number into freq
        carrierFrequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        globalCarrierFrequency = carrierFrequency;
        
        // Color Select Logic:
        if(colorOneSelected) {
            globalModulatorFrequency = carrierFrequency*(0.999/2);
        }
        else if(colorTwoSelected) {
            globalModulatorFrequency = carrierFrequency*(0.999);
        }
        else if(colorThreeSelected) {
            globalModulatorFrequency = carrierFrequency*(1.999);
        }
        else if(colorFourSelected) {
            globalModulatorFrequency = carrierFrequency*(0.999/3);
        }
        else if(colorFiveSelected) {
            globalModulatorFrequency = carrierFrequency*(2.999);
        }
        else if(colorSixSelected) {
            globalModulatorFrequency = carrierFrequency*(3.999);
        }
        else {
            globalModulatorFrequency = 50;
        }
        
        // we don't want an ugly sweep when the note begins...
        smooth[0].setSmooth(0);
        smooth[0].tick(carrierFrequency);
        
        // standard smoothing...
        for(int i=0; i<2; i++){
            smooth[i].setSmooth(0.999);
        }
        
        level = velocity;
        // level = pow(velocity,2); // if we want linear dynamic
        
        // tells the note to begin!
        onOff = true;
        
        // These parameters could be controlled with UI elements and could
        // be assigned to specific MIDI controllers. If you do so,
        // don't forget to smooth them!
        modulator.setFrequency(globalModulatorFrequency);
        index = globalIndex;
    }
    
    
    
    void stopNote (float /*velocity*/, bool allowTailOff) override
    {
        onOff = false; // end the note
        level = 0; // ramp envelope to 0 if tail off is allowed
        
        tailOff = allowTailOff;
    }
    
    
    
    void pitchWheelMoved (int /*newValue*/) override
    {
        // Pitch wheel is an important standard MIDI feature that should be implemented
    }
    
    
    
    
    void controllerMoved (int /*controllerNumber*/, int /*newValue*/) override
    {
        // Thats where you would change the value of the modulator index and frequency
        // if you wanted to control them with MIDI controllers
    }
    
    
    
    
    
    // This is our main audio buffer
    // ...I think... ***************
    void renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override {
        // only compute block if note is on!
        if(envelope != 0 || onOff){
            while (--numSamples >= 0){
                index = globalIndex;
                envelope = smooth[1].tick(level); // here we use a smoother as an envelope generator
                carrier.setFrequency(smooth[0].tick(carrierFrequency) + modulator.tick()*index);
                const float currentSample = (float) carrier.tick() * envelope * gain;
                for (int i = outputBuffer.getNumChannels(); --i >= 0;){
                    outputBuffer.addSample (i, startSample, currentSample);
                }
                ++startSample;
                
                // if tail off is disabled, we end the note right away, otherwise, we wait for envelope
                // to reach a safe value
                if(!onOff && (envelope < 0.001 || !tailOff)){
                    envelope = 0;
                    clearCurrentNote();
                }
            }
        }
    }
    
    
    
    
private:
    Sine carrier, modulator;
    Smooth smooth[2];
    double carrierFrequency, index, level, envelope;
    bool onOff, tailOff;
};












struct SynthAudioSource : public AudioSource {
    SynthAudioSource (MidiKeyboardState& keyState) : keyboardState (keyState) {
        // Add some voices to our synth, to play the sounds..
        int nVoices = 4;
        for (int i = nVoices; --i >= 0;)
        {
            synth.addVoice (new FMVoice());
        }
        
        synth.clearSounds();
        synth.addSound (new FMSound());
    }
    
    
    
    void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate) override
    {
        midiCollector.reset (sampleRate);
        synth.setCurrentPlaybackSampleRate (sampleRate);
    }
    
    
    
    void releaseResources() override
    {
    }
    
    
    
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override {
        // the synth always adds its output to the audio buffer, so we have to clear it
        // first..
        bufferToFill.clearActiveBufferRegion();
        
        // fill a midi buffer with incoming messages from the midi input.
        MidiBuffer incomingMidi;
        midiCollector.removeNextBlockOfMessages (incomingMidi, bufferToFill.numSamples);
        //midiCollector.
        
        // pass these messages to the keyboard state so that it can update the component
        // to show on-screen which keys are being pressed on the physical midi keyboard.
        // This call will also add midi messages to the buffer which were generated by
        // the mouse-clicking on the on-screen keyboard.
        keyboardState.processNextMidiBuffer (incomingMidi, 0, bufferToFill.numSamples, true);
        
        // and now get the synth to process the midi events and generate its output.
        synth.renderNextBlock (*bufferToFill.buffer, incomingMidi, 0, bufferToFill.numSamples);
    }
    
    // this collects real-time midi messages from the midi input device, and
    // turns them into blocks that we can process in our audio callback
    MidiMessageCollector midiCollector;
    
    // this represents the state of which keys on our on-screen keyboard are held
    // down. When the mouse is clicked on the keyboard component, this object also
    // generates midi messages for this, which we can pass on to our synth.
    MidiKeyboardState& keyboardState;
    
    // the synth itself!
    Synthesiser synth;
};






















// Main Constructor:
TutorialPluginAudioProcessor::TutorialPluginAudioProcessor() :
    keyboardComponent (keyboardState, MidiKeyboardComponent::horizontalKeyboard) {
    
    audioDeviceManager.initialise (0, 2, nullptr, true, String(), nullptr);
    
    audioSourcePlayer.setSource (&synthAudioSource);
    
    audioDeviceManager.addAudioCallback (&audioSourcePlayer);
    audioDeviceManager.addMidiInputCallback (String(), &(synthAudioSource.midiCollector));
    
    // MIDI Inputs
    addAndMakeVisible (midiInputListLabel);
    midiInputListLabel.setText ("MIDI Input:", dontSendNotification);
    midiInputListLabel.attachToComponent (&midiInputList, true);
    
    // menu listing all the available MIDI inputs
    addAndMakeVisible (midiInputList);
    midiInputList.setTextWhenNoChoicesAvailable ("Plug In Your MIDI Keyboard To Rock");
    const StringArray midiInputs (MidiInput::getDevices());
    midiInputList.addItemList (midiInputs, 1);
    midiInputList.setSelectedId(0); // default
    midiInputList.addListener (this);
    // Make it pretty:
    midiInputList.setColour(0x1000b00, Colour(25, 25, 25)); //BG
    midiInputList.setColour(0x1000a00, juce::Colours::lightgrey); //Text
    midiInputList.setColour(0x1000c00, Colour(50, 50, 50)); //Border
    midiInputList.setColour(0x1000d00, Colour(50, 50, 50)); //Border
    
    
    // Set default MIDI input:
    setMidiInput(0);
    
    // The MIDI keyboard:
    addAndMakeVisible(keyboardComponent);
    
}




// Main Destructor:
TutorialPluginAudioProcessor::~TutorialPluginAudioProcessor() {
    audioSourcePlayer.setSource (nullptr);
    audioDeviceManager.removeMidiInputCallback (String(), &(synthAudioSource.midiCollector));
    audioDeviceManager.removeAudioCallback (&audioSourcePlayer);
    audioDeviceManager.removeMidiInputCallback (MidiInput::getDevices()[midiInputList.getSelectedItemIndex()], this);
    midiInputList.removeListener (this);
}




// Prepare to play method.
void TutorialPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock) {
    // midiCollector.reset (sampleRate);  // DID Touch this.
    // synth.setCurrentPlaybackSampleRate (sampleRate); // DID Touch this.
}





// Processes a block of audio:
void TutorialPluginAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);
        
        // ..do something to the data...
    }
}










// Gets called when the midi input list
// is changed.  "ComboBox" is the drop down.
void comboBoxChanged (ComboBox* box) override {
    if (box == &midiInputList) setMidiInput  (midiInputList.getSelectedItemIndex());
}








// Handles MIDI inputs in some mysterious
// black magic way.
void setMidiInput (int index) {
    const StringArray list (MidiInput::getDevices());
    audioDeviceManager.removeMidiInputCallback (list[lastMidiInputIndex], this);
    const String newInput (list[index]);
    if (! audioDeviceManager.isMidiInputEnabled (newInput))
        audioDeviceManager.setMidiInputEnabled (newInput, true);
    audioDeviceManager.addMidiInputCallback (newInput, this);
    midiInputList.setSelectedId (index + 1, dontSendNotification);
    lastMidiInputIndex = index;
}

// Needed virtual function
void handleIncomingMidiMessage (MidiInput*, const MidiMessage& message) override {}





// ////////////////////////////////////////////// //
//           STUFF I DON'T UNDERSTAND             //
//         (and therefore didn't touch)           //
// ////////////////////////////////////////////// //

// MIDI Program Stuff (Not using):
int TutorialPluginAudioProcessor::getCurrentProgram() { return 0; }
void TutorialPluginAudioProcessor::setCurrentProgram (int index) { }
const String TutorialPluginAudioProcessor::getProgramName (int index) {return String();}
void TutorialPluginAudioProcessor::changeProgramName (int index, const String& newName) {}

// Free up memory:
void TutorialPluginAudioProcessor::releaseResources() { }

// Not sure what this does:
#ifndef JucePlugin_PreferredChannelConfigurations
bool TutorialPluginAudioProcessor::setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet)
{
    // Reject any bus arrangements that are not compatible with your plugin
    const int numChannels = preferredSet.size();
#if JucePlugin_IsMidiEffect
    if (numChannels != 0)
        return false;
#elif JucePlugin_IsSynth
    if (isInput || (numChannels != 1 && numChannels != 2))
        return false;
#else
    if (numChannels != 1 && numChannels != 2)
        return false;
    if (! AudioProcessor::setPreferredBusArrangement (! isInput, bus, preferredSet))
        return false;
#endif
    return AudioProcessor::setPreferredBusArrangement (isInput, bus, preferredSet);
}
#endif

// Not touching this:
bool TutorialPluginAudioProcessor::hasEditor() const { return true; }
// Nor this:
AudioProcessorEditor* TutorialPluginAudioProcessor::createEditor(){
    return new TutorialPluginAudioProcessorEditor (*this);
}
// Not sure how to use this:
void TutorialPluginAudioProcessor::getStateInformation (MemoryBlock& destData) { }
// Nor this:
void TutorialPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes) { }

// Don't know what this does:
int TutorialPluginAudioProcessor::getNumPrograms() { return 1; }

// Does some midi thing:
double TutorialPluginAudioProcessor::getTailLengthSeconds() const { return 0.0; }

// Returns Plug-in name:
const String TutorialPluginAudioProcessor::getName() const
{return JucePlugin_Name; }

// Let's DAW know we want to receive MIDI:
bool TutorialPluginAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

// Let's DAW know we don't give MIDI:
bool TutorialPluginAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new TutorialPluginAudioProcessor();
}
