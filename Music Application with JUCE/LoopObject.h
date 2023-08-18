/*
  ==============================================================================

    LoopObject.h
    Created: 9 Apr 2022 11:03:40am
    Author:  klela

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class LoopObject
{
public:
		
	void setStart(double _start) {
		start = _start;
	}

	void setEnd(double _end) {
		end = _end;
	}

	void setLoop(bool _newLoop) {
		newLoop = _newLoop;
		if (end <= start) {
			isValid = false;
		}
		else {
			isValid = true;
		}
	}

	bool getNewLoop() const {
		return newLoop;
	}

	bool getValid() const {
		return isValid;
	}

	double getStart() const {
		return start;
	}

	double getEnd() const {
		return end;
	}

private:
	double start = 0;
	double end = 0;
	bool isValid = false;
	bool newLoop = false;
};
