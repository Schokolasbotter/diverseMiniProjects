#pragma once
// assumes your JuceHeader file is here
// check your MainComponent.h to see
// how the include is configured on your
// system
#include "../JuceLibraryCode/JuceHeader.h"
#include "LoopObject.h"

class DJAudioPlayer : public juce::AudioSource
{
public:
	DJAudioPlayer(juce::AudioFormatManager& _formatManager);
	~DJAudioPlayer();
	void loadURL(const juce::URL& file); //Load URL to an AudioFile
	void play(); //Start Song
	void stop(); //Stop Song
	void pause(); //Pause Song
	void setPosition(double posInSecs); //Set Song Position
	double getRelativePosition(); //Get Song Position
	void setPositionRelative(double pos);
	void setGain(double gain); //Set Volume
	void setMixerGain(double _mixerGain); //Set the Gain from CrossFader
	void setSpeed(double ratio); //Set Speed
	double getPositionRelative(); //Get relative position of Playhead
	void setLoop(LoopObject _loop); //Set the Loop on or off

	//AudioSource Functions
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
	void releaseResources() override;

	//Queue Functions
	bool getSongStatus();
	void setQueueStatus(bool queueStatus);

private:
	std::string name;
	bool songEnded = false;
	bool queueHasSong = false;

	double gain = 1;
	double mixerGain = 1;
	LoopObject loop;

	juce::AudioFormatManager& formatManager;
	std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
	juce::AudioTransportSource transportSource;
	juce::ResamplingAudioSource resampleSource{ &transportSource, false, 2 };
};
