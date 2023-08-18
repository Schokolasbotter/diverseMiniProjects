/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 16 Mar 2022 10:17:36am
    Author:  klela

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache & cacheToUse, DJAudioPlayer* _player)
	:audioThumb(2000, formatManagerToUse, cacheToUse),
	isLoaded(false),
	position(0),
	player(_player)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	audioThumb.addChangeListener(this);
	addMouseListener(this, true);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::darkred);
	if (isLoaded) {
		audioThumb.drawChannel(g, getLocalBounds(), 0, audioThumb.getTotalLength(), 0, 0.9f);
		g.setColour(juce::Colours::black);
		if (position * getWidth() != position * getWidth())
		{
			DBG("WaveformDisplay: position is NaN, not drawing playhead");
		}
		else
		{
			g.fillEllipse(position*getWidth(), 0, 2, getHeight());
		}
	}
	else {
		g.setFont (20.0f);
		g.drawText ("File not loaded...", getLocalBounds(),
					juce::Justification::centred, true);   // draw some placeholder text
	}

	if (loop.getValid()) {
		g.setColour(juce::Colours::coral);
		g.drawRect(loop.getStart()*getWidth(), 0, (loop.getEnd() - loop.getStart())*getWidth(), getHeight());
	}
}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void WaveformDisplay::loadURL(juce::URL audioURL) {
	audioThumb.clear();
	isLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source) {
	repaint();
}

void WaveformDisplay::setPositionRelative(double pos) {
	if (pos != position) {
		repaint();
	}
	position = pos;
}

void WaveformDisplay::mouseDown(const juce::MouseEvent &event) {
	//Call Mouse Drag
	mouseDrag(event);
}

void WaveformDisplay::mouseDrag(const juce::MouseEvent &event) {
	//Convert to double
	double startingPoint = event.getMouseDownX();
	double markedWidth = event.getDistanceFromDragStartX();
	double totalWidth = getWidth();

	//Set loop information to relative Positions
	loop.setStart(startingPoint / totalWidth);
	if (startingPoint + markedWidth > totalWidth) {
		loop.setEnd(1);
	}
	else {
		loop.setEnd((startingPoint + markedWidth) / totalWidth);
	}
	loop.setLoop(true);
	repaint();
}

void WaveformDisplay::mouseUp(const juce::MouseEvent &event) {
	player->setLoop(loop);
}

LoopObject WaveformDisplay::getLoop() {
	return loop;
}
