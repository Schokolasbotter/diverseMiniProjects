/*
  ==============================================================================

    songQueue.h
    Created: 9 Apr 2022 5:05:10pm
    Author:  klela

  ==============================================================================
*/
#include <JuceHeader.h>
#include <Vector>

#pragma once
class Songqueue
{
public:
	Songqueue();
	~Songqueue();

	//Virtuals
	virtual void playSong() =0;

	//Regular
	void addSong(juce::URL songURL);
	juce::URL getSong();
	bool getQueueStatus() const;
	int getQueueNumber() const;

private:

	//Vector Holding URL
	std::vector<juce::URL> list;
	bool queueStatus = false;

};

