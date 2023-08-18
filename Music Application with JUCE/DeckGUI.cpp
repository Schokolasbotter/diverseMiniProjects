/*
  ==============================================================================

    DeckGUI.cpp
    Created: 8 Mar 2022 11:15:09am
    Author:  klela

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(std::string _name, DJAudioPlayer* _player, juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache & cacheToUse) :name(_name), 
																																					player(_player),
																																					waveformDisplay(formatManagerToUse,cacheToUse,_player)
{
	//Show Buttons
	addAndMakeVisible(playButton);
	addAndMakeVisible(stopButton);
	addAndMakeVisible(pauseButton);	
	addAndMakeVisible(loadButton);

	// Sliders
	volSlider.setRange(0, 1);
	volSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
	volSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, getWidth() / 12, 20);
	addAndMakeVisible(volSlider);

	timeline.setRange(0, 1);
	timeline.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
	timeline.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 0, 0);
	addAndMakeVisible(timeline);

	speedSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
	speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 0, 0);
	addAndMakeVisible(speedSlider);

	//Labels
	volLabel.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(volLabel);
	timelineLabel.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(timelineLabel);
	speedLabel.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(speedLabel);

	//waveformDisplay
	addAndMakeVisible(waveformDisplay);

	//Listeners
	playButton.addListener(this);
	stopButton.addListener(this);
	pauseButton.addListener(this);
	volSlider.addListener(this);
	loadButton.addListener(this);
	timeline.addListener(this);
	speedSlider.addListener(this);

	//Timer
	startTimer(500);

	//Text
	queueInfo.setColour(juce::Colours::white);
	addAndMakeVisible(queueInfo);
}

DeckGUI::~DeckGUI()
{
	stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::white);
    g.setFont (25.0f);
    g.drawText ("	Deck " + name, getLocalBounds(),juce::Justification::topLeft, true);   // draw some placeholder text

	queueInfo.setText("Songs in Queue : " + std::to_string(getQueueNumber()));
	queueInfo.draw(g, 1.0f);
	//g.drawText(,getLocalBounds(), juce::Justification::topRight, true);
}

void DeckGUI::resized()
{
	double rowH = getHeight() / 7.0;
	double buttonWidth = getWidth()/3;
	double buttonHeight = getWidth() / 10;
	double spacing = 3.0;

	//Button Positions
	playButton.setBounds(spacing, 4 * rowH, buttonWidth- 1.5*spacing, buttonHeight);
	stopButton.setBounds(buttonWidth, 4 * rowH, buttonWidth - spacing, buttonHeight);
	pauseButton.setBounds(2 * buttonWidth , 4 * rowH, buttonWidth - spacing, buttonHeight);

	loadButton.setBounds(spacing, 3 * rowH, 2 * buttonWidth - 1.5 * spacing, buttonHeight * 0.7);

	//Vol Sliders
	volSlider.setBounds(2 * buttonWidth + spacing, 3 * rowH, buttonWidth - 1.5*spacing, buttonHeight * 0.7);
	volLabel.setBounds(2 * buttonWidth + spacing, 2.6 * rowH, buttonWidth, rowH / 2);
	volSlider.setValue(1);

	//TimeLine Slider
	timeline.setBounds(spacing, 5.5 * rowH, 3 * buttonWidth-2*spacing, buttonHeight / 2);
	timelineLabel.setBounds(spacing, 5 * rowH, buttonWidth, buttonHeight / 2);

	//Speed Slider
	speedSlider.setBounds(spacing, 6.5 * rowH, 3 * buttonWidth - 2 * spacing, buttonHeight / 2);
	speedLabel.setBounds(spacing, 6 * rowH, buttonWidth, buttonHeight / 2);

	//Waveform Display
	waveformDisplay.setBounds(spacing, rowH * 2/3, getWidth() - spacing, 2 * rowH);

	//Text
	queueInfo.setBoundingBox(juce::Parallelogram<float>(juce::Rectangle<float>(getWidth() - 85, 0, 80, 20)));
	queueInfo.setFontHeight(10);

}

void DeckGUI::buttonClicked(juce::Button* button)
{
	//Load
	if (button == &loadButton)
	{
		juce::FileChooser chooser("Select a Wave file to play...");
		if (chooser.browseForFileToOpen())
		{
			juce::URL audioURL = juce::URL{ chooser.getResult() };
			player->loadURL(audioURL);
			waveformDisplay.loadURL(audioURL);
		}
	}
	//Play
	if (button == &playButton)
	{
		playing = true;
		player->play();
	}
	//Stop
	if (button == &stopButton)
	{
		playing = false;
		player->stop();
	}
	//Pause
	if (button == &pauseButton) {
		if (playing) {
			playing = false;
			player->pause();
		}
		else {
			playing = true;
			player->play();
		}
	}
}

void DeckGUI::sliderValueChanged(juce::Slider *slider) {
	if (slider == &volSlider){
		player->setGain(slider->getValue());
	}
	if (slider == &timeline) {
		player->setPositionRelative(slider->getValue());
	}
	if (slider == &speedSlider) {
		player->setSpeed(slider->getValue());
	}
}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray &files) {
	return true;
}

void DeckGUI::filesDropped(const juce::StringArray &files, int x, int y){
	if (files.size() == 1) {
		player->loadURL(juce::URL{ juce::File{files[0]} });
		waveformDisplay.loadURL(juce::URL{ juce::File{files[0]} });
	}
}

void DeckGUI::timerCallback() {
	//Update display for playHead
	waveformDisplay.setPositionRelative(player->getPositionRelative());
	
	//If song finished
	if (player->getSongStatus()) {
		//Load Next Song
		playSong();
	}
	
	//Set Status of Queue in Player
	player->setQueueStatus(getQueueStatus());

	//Repaint if needed
	if (getQueueStatus()) {
		repaint();
	}
}

void DeckGUI::playSong() {
		//Create URL and load into player and WaveformDisplay
		juce::URL nextSong = getSong();
		player->loadURL(nextSong);
		waveformDisplay.loadURL(nextSong);
}