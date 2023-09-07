#pragma once
#include <SFML/Window.hpp>
#include <iostream>
#include <map>
#include "Timer.h"
#include <fstream>

#define keyCount sf::Keyboard::KeyCount

enum KeyboardAction {Invalid = -1, StartStop, Split, Reset, ActionCount};

struct joystickButton {
	int joystickID;
	int buttonID;
	inline bool operator==(joystickButton other) {
		return (joystickID == other.joystickID) && (buttonID == other.buttonID);
	}
};

class KeybindManager
{

public:
	KeybindManager(Timer* timer);
	void tick();

	std::map<KeyboardAction, sf::Keyboard::Key> keybinds;
	std::map<KeyboardAction, joystickButton> joystickButtons;
	KeyboardAction rebind;
private:
	void keyPressed(sf::Keyboard::Key key);
	void buttonPressed(joystickButton button);
	void saveKeybinds();

	bool* isKeyDown;
	bool* isButtonDown;
	int buttonCount[8];
	Timer* timer;

};

