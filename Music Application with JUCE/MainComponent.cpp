#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }

	//Decks
	addAndMakeVisible(deckGUIA);
	addAndMakeVisible(deckGUIB);
	addAndMakeVisible(playlistComponent);

	//Audio Formats
	formatManager.registerBasicFormats();

	//Mix Slider
	mixSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
	mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 0, 0);
	mixSlider.setRange(0, 1);
	mixSlider.setValue(0.5);
	addAndMakeVisible(mixSlider);
	mixSlider.addListener(this);
	crossFaderLabel.setJustificationType(juce::Justification::centred);
	//crossFaderLabel.setColour(juce::Label::outlineColourId, juce::Colours::white); //Insert to check Label Borders
	addAndMakeVisible(crossFaderLabel);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
	playerA.prepareToPlay(samplesPerBlockExpected, sampleRate);
	playerB.prepareToPlay(samplesPerBlockExpected, sampleRate);
	mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

	mixerSource.addInputSource(&playerA, false);
	mixerSource.addInputSource(&playerB, false);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
	mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
	playerA.releaseResources();
	playerB.releaseResources();
	mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
	//DeckGUI
	deckGUIA.setBounds(0, 0, getWidth() / 2, getHeight()/2);
	deckGUIB.setBounds(getWidth()/2, 0, getWidth() / 2, getHeight()/2);

	//Slider
	mixSlider.setBounds(getWidth()/4, getHeight() * 5.05/10, getWidth() / 2, getHeight() / 10);
	crossFaderLabel.setBounds(getWidth() * 4/9, getHeight() * 5/ 10, getWidth() / 9, getHeight() / 30);

	//Playlist
	playlistComponent.setBounds(0, getHeight() * 6 / 10, getWidth(), getHeight() * 4 / 10);
}

void MainComponent::sliderValueChanged(juce::Slider *slider) {
	if (slider == &mixSlider) {
		playerA.setMixerGain(1-slider->getValue());
		playerB.setMixerGain(slider->getValue());
	}
}