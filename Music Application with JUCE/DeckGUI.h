/*
  ==============================================================================

    DeckGUI.h
    Created: 8 Mar 2022 11:15:09am
    Author:  klela

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "songQueue.h"

//==============================================================================
/*
*/
class DeckGUI  : public juce::Component,
				public juce::Button::Listener,
				public juce::Slider::Listener,
				public juce::FileDragAndDropTarget,
				public juce::Timer,
				public Songqueue
{
public:
    DeckGUI(std::string name, DJAudioPlayer* player, juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse);
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

	//Listeners	
	void buttonClicked(juce::Button* button) override;
	void sliderValueChanged(juce::Slider *slider) override;

	//Drag and Drop
	bool isInterestedInFileDrag(const juce::StringArray &files) override;
	void filesDropped(const juce::StringArray &files, int x, int y) override;

	//Timer
	void timerCallback() override; 

	//SongQueue
	void playSong() override;

private:
	//Name
	std::string name;

	//Buttons
	juce::TextButton playButton{ "Play" }, stopButton{ "Stop" }, pauseButton{ "Pause" }, loadButton{ "Load" };
	//Drawable Text
	juce::DrawableText queueInfo;

	//Sliders
	juce::Slider volSlider, timeline, speedSlider;

	//Labels
	juce::Label volLabel{ "Volume","Volume" }, timelineLabel{ "Timeline","Timeline" }, speedLabel{ "Speed","Speed" };

	//AudioPlayer
	DJAudioPlayer* player;

	//Other
	bool playing = false;

	//Waveform Display
	WaveformDisplay waveformDisplay;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
