/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 9 Apr 2022 11:30:54am
    Author:  klela

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(DeckGUI* _deckGUIA, DeckGUI* _deckGUIB):deckGUIA(_deckGUIA), deckGUIB(_deckGUIB)
{

    //Table Settings
	tableComponent.getHeader().addColumn("Title", 1, 250);
	tableComponent.getHeader().addColumn("Artist", 2, 250);
	tableComponent.getHeader().addColumn("", 3, 100);
	tableComponent.getHeader().addColumn("", 4, 100);
	tableComponent.getHeader().addColumn("", 5, 100);
	tableComponent.setModel(this);
	tableComponent.autoSizeAllColumns();
	addAndMakeVisible(tableComponent);
	
	//Testing Content
	//playlist.push_back(Song{ "Song Title","Artist Name",juce::URL{""} });
	//playlist.push_back(Song{ "Song Title","Artist Name",juce::URL{""} });

	//Button
	loadButton.addListener(this);
	addAndMakeVisible(loadButton);

	//Load data at start
	loadPlaylistFromFile();
}

PlaylistComponent::~PlaylistComponent()
{
	//Save Playlist on closing
	savePlaylistToFile();
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void PlaylistComponent::resized()
{
	//Table
	tableComponent.setBounds(0, 20, getWidth(), getHeight());
	//Button
	loadButton.setBounds(0, 0, getWidth(), 20);
}

int PlaylistComponent::getNumRows() {
	return playlist.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) {
	if (rowIsSelected) {
		g.fillAll(juce::Colours::orange);
	}
	else {
		g.fillAll(juce::Colours::darkgrey);
	}
}

void PlaylistComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
	//Fill Cells depending on Column and Row
	if (rowNumber < playlist.size() && playlist.size() != 0) {
		switch (columnId)
		{
		case 1:
			g.drawText(playlist[rowNumber].title, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
			break;
		case 2:
			g.drawText(playlist[rowNumber].artist, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
		default:
			break;
		}
	}
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponentToUpdate) {
	//Create buttons depending ond row and column
	if (rowNumber < playlist.size() && playlist.size() != 0) {
		switch (columnId)
		{
		case 3:
			if (existingComponentToUpdate == nullptr) {
				juce::TextButton* btn = new juce::TextButton{ "Queue Deck A" };
				juce::String id{ std::to_string((rowNumber+1)*columnId) };
				btn->setComponentID(id);
				btn->addListener(this);
				existingComponentToUpdate = btn;
			}
			break;
		case 4:
			if (existingComponentToUpdate == nullptr) {
				juce::TextButton* btn = new juce::TextButton{ "Queue Deck B" };
				juce::String id{ std::to_string((rowNumber + 1)*columnId) };
				btn->setComponentID(id);
				btn->addListener(this);
				existingComponentToUpdate = btn;
			}
			break;	
		case 5:
			if (existingComponentToUpdate == nullptr) {
				juce::TextButton* btn = new juce::TextButton{ "Remove" };
				juce::String id{ std::to_string((rowNumber + 1)*columnId) };
				btn->setComponentID(id);
				btn->addListener(this);
				existingComponentToUpdate = btn;
			}
			break;
		default:
			break;
		}
	}
	return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button* button) {
	if (button == &loadButton) { //Use button to load file into playlist
		juce::FileChooser chooser("Select a Wave file to play...");
		if (chooser.browseForFileToOpen())
		{
			juce::URL audioURL = juce::URL{ chooser.getResult() };
			playlist.push_back(Song{ "Track " + std::to_string(playlist.size() + 1),"Artist " + std::to_string(playlist.size() + 1),audioURL });
			tableComponent.updateContent();
			repaint();
		}
		return;
	}

	
	int id = std::stoi(button->getComponentID().toStdString());
	//Remove button
	if (id % 5 == 0) { // If Button in fifth Column 
			int rowNumber = id / 5;
			rowNumber--; //Adjust to get Real RowNumber
			playlist.erase(playlist.begin() + rowNumber);
			savePlaylistToFile();
			tableComponent.updateContent();
			repaint();
	}
	//Queue Deck B 
	else if (id % 4 == 0) { //If Button is in fourth coloumn
		int rowNumber = id / 4;
		rowNumber--; //Adjust to get Real RowNumber
		deckGUIB->addSong(playlist[rowNumber].trackURL);
	}
	//Queue Deck A
	else if (id % 3 == 0) {
		int rowNumber = id / 3;
		rowNumber--; //Adjust to get Real RowNumber
		deckGUIA->addSong(playlist[rowNumber].trackURL);
	}
}

bool PlaylistComponent::isInterestedInFileDrag(const juce::StringArray& files) {
	return true;
}

void PlaylistComponent::filesDropped(const juce::StringArray& files, int x, int y) {
	//Load file into Playlist with DragAndDrop
	if (files.size() == 1) {
		DBG("Loading File");
		refreshComponentForCell(playlist.size(), 1, false, tableComponent.getCellComponent(1, playlist.size()));
		playlist.push_back(Song{ "Track "+std::to_string(playlist.size()+1),"Artist "+std::to_string(playlist.size()+1),juce::URL{juce::File{files[0]} } });
	}
	tableComponent.updateContent();
	repaint();
}

void PlaylistComponent::savePlaylistToFile() {
	//Open File
	playlistFile.open("playlist.csv", std::ios::out | std::ios::trunc);
	if (playlistFile.is_open()) {
		if (playlist.size() > 0) { //If Playlist is not empty fill with data
			for (int i = 0; i <= playlist.size() - 1; i++) {
				playlistFile << playlist[i].title << "," << playlist[i].artist << "," << playlist[i].trackURL.toString(false) << "\n";
			}
		}
		else if (playlist.size() == 0) { //if Playlist is empty fill empty
			playlistFile << "";
		}
	}
	playlistFile.close();

}

void PlaylistComponent::loadPlaylistFromFile() {
	//Open File
	playlistFile.open("playlist.csv", std::ios::in);
	if (playlistFile.is_open()) {
		//Clear Vector
		playlist.clear();
		//Setup Variables for reading file
		std::vector<std::string> row;
		std::string line, word;
		while (std::getline(playlistFile, line)) {
			row.clear();
			std::stringstream s(line);
			while (std::getline(s, word, ',')) {
				row.push_back(word);
			}
			if (row.size() != 0) {
				playlist.push_back(Song{ row[0],row[1],juce::URL{row[2]} });
			}
		}
	}
	playlistFile.close();
	tableComponent.updateContent();
	repaint();
}
