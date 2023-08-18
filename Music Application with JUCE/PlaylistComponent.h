/*
  ==============================================================================

    PlaylistComponent.h
    Created: 9 Apr 2022 11:30:54am
    Author:  klela

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include <fstream>
#include "DeckGUI.h"

//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
	public juce::TableListBoxModel,
	public juce::Button::Listener,
	public juce::FileDragAndDropTarget
{
public:
	PlaylistComponent(DeckGUI* DeckGUIA, DeckGUI* DeckGUIB);
    ~PlaylistComponent() override;

    void paint (juce::Graphics& g) override;
    void resized() override;

	//Table
	int getNumRows() override;
	void paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;
	void paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
	juce::Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponentToUpdate) override;

	//Buttons
	void buttonClicked(juce::Button* button) override;

	//Drag and Drop
	bool isInterestedInFileDrag(const juce::StringArray& files) override;
	void filesDropped(const juce::StringArray& files, int x, int y) override;

	//File Management
	void savePlaylistToFile();
	void loadPlaylistFromFile();


private:
	juce::TableListBox tableComponent;
	juce::TextButton loadButton{ "Load into Playlist" };

	//Model for Data in Playlists
	struct Song {
		std::string title;
		std::string artist;
		juce::URL trackURL;
	};

	//Playlist Data
	std::vector<Song> playlist;

	//CSV File
	std::fstream playlistFile;

	//DeckGUI
	DeckGUI* deckGUIA;
	DeckGUI* deckGUIB;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
