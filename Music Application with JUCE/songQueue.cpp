/*
  ==============================================================================

    songQueue.cpp
    Created: 9 Apr 2022 5:05:10pm
    Author:  klela

  ==============================================================================
*/
#include "songQueue.h"

Songqueue::Songqueue() {

}

Songqueue::~Songqueue() {
	list.clear();
}

void Songqueue::addSong(juce::URL songURL) {
	list.push_back(songURL);
}

juce::URL Songqueue::getSong() {
	juce::URL songURL = list[0];
	list.erase(list.begin());
	return songURL;
}

bool Songqueue::getQueueStatus() const {
	if (list.size() == 0) {
		return false;
	}
	else {
		return true;
	}
}

int Songqueue::getQueueNumber() const {
	return list.size();
}
