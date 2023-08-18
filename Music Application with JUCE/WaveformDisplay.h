/*
  ==============================================================================

    WaveformDisplay.h
    Created: 16 Mar 2022 10:17:36am
    Author:  klela

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LoopObject.h"
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
class WaveformDisplay  : public juce::Component,
							public juce::ChangeListener,
							public juce::MouseListener
{
public:
    WaveformDisplay(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache & cacheToUse, DJAudioPlayer* _player);
    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;

	void changeListenerCallback(juce::ChangeBroadcaster* source) override;

	void mouseDown(const juce::MouseEvent &event) override;
	void mouseDrag(const juce::MouseEvent &event) override;
	void mouseUp(const juce::MouseEvent &event) override;


	void loadURL(juce::URL audioURL);

	void setPositionRelative(double pos); //Set relative position of playhead

	LoopObject getLoop();

private:

	//AudioThumbnail
	juce::AudioThumbnail audioThumb;
	bool isLoaded;
	double position;

	//Loop
	LoopObject loop;

	//Player
	DJAudioPlayer* player;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
