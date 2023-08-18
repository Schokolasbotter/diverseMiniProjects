#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"
#include <sstream>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent,
						public juce::Slider::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

	//Slider
	void sliderValueChanged(juce::Slider *slider) override;

private:
    //==============================================================================
    // Your private member variables go here...

	//Audio Mixer
	juce::MixerAudioSource mixerSource;
	juce::Slider mixSlider;
	juce::Label  crossFaderLabel{ "Cross Fader", " Cross Fader" };

	//Audio Player Class
	DJAudioPlayer playerA{ formatManager };
	DJAudioPlayer playerB{ formatManager };

	//GUI Class
	DeckGUI deckGUIA{"A", &playerA, formatManager, thumbCache};
	DeckGUI deckGUIB{"B", &playerB, formatManager, thumbCache};

	//Waveform Display
	juce::AudioFormatManager formatManager;
	juce::AudioThumbnailCache thumbCache{ 1000 };

	//Playlist
	PlaylistComponent playlistComponent { &deckGUIA, &deckGUIB};


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
