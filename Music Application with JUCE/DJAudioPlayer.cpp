#include "DJAudioPlayer.h"
DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager) :formatManager(_formatManager)
{
}
DJAudioPlayer::~DJAudioPlayer()
{
}

void DJAudioPlayer::loadURL(const juce::URL& audioURL)
{
	auto * reader =
	formatManager.createReaderFor(audioURL.createInputStream(false));
	if (reader) // good file!
	{
		std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
		transportSource.setSource(newSource.get(), 0, nullptr, reader -> sampleRate);
		readerSource.reset(newSource.release());
	}
	songEnded = false;
	transportSource.setPosition(0);
	transportSource.start();
}

void DJAudioPlayer::play()
{
	transportSource.start();
}
void DJAudioPlayer::stop()
{	
	transportSource.stop();
	transportSource.setPosition(0);
}

void DJAudioPlayer::pause() {
	transportSource.stop();
}
void DJAudioPlayer::setPosition(double posInSecs)
{
	if (posInSecs < 0 || posInSecs > transportSource.getLengthInSeconds()) {
		DBG("DJAudioPlayer::setPosition: warning set position ");
		DBG(posInSecs);
		DBG(" greater than length ");
		DBG(transportSource.getLengthInSeconds());
		return;
	}
	transportSource.setPosition(posInSecs);
}

double DJAudioPlayer::getRelativePosition() {
	return (transportSource.getCurrentPosition()/transportSource.getLengthInSeconds());
}

void DJAudioPlayer::setPositionRelative(double pos)
{
	double posInSecs = pos * transportSource.getLengthInSeconds();
	setPosition(posInSecs);
}

void DJAudioPlayer::setGain(double _gain)
{
	gain = _gain;
}

void DJAudioPlayer::setMixerGain(double _mixerGain)
{
	if (_mixerGain >= 0.5) {
		mixerGain = 1;
	}
	else {
		mixerGain = (_mixerGain * 2);
	}
}

void DJAudioPlayer::setSpeed(double ratio) {
	resampleSource.setResamplingRatio(ratio);
}

//AudioSource Functions
void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
	resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

}
void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo&bufferToFill)
{
	//Set Gain
	transportSource.setGain(gain * mixerGain);

	//Loop if loop is on
	if (loop.getValid() && transportSource.getCurrentPosition() > loop.getEnd()*transportSource.getLengthInSeconds()) {
		setPositionRelative(loop.getStart());
	}
	//If Song ended
	if (transportSource.getCurrentPosition() >= transportSource.getLengthInSeconds()) {
		//If queue has song
		if (queueHasSong) {
			songEnded = true;
			DBG("Song Ended");
		}
	}

	if (!readerSource.get())
	{
		bufferToFill.clearActiveBufferRegion();
		return;
	}
	resampleSource.getNextAudioBlock(bufferToFill);
}
void DJAudioPlayer::releaseResources()
{
	transportSource.releaseResources();
	resampleSource.releaseResources();
}

double DJAudioPlayer::getPositionRelative() {
	return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

void DJAudioPlayer::setLoop(LoopObject _loop) {
	loop = _loop;
	setPositionRelative(loop.getStart());
}

bool DJAudioPlayer::getSongStatus() {
	return songEnded;
}

void DJAudioPlayer::setQueueStatus(bool queueStatus) {
	queueHasSong = queueStatus;
}

