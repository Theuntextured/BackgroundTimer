#pragma once
#include <SFML/Window.hpp>
#include <iostream>
#include <map>
#include "Timer.h"
#include <fstream>

#define keyCount sf::Keyboard::KeyCount

enum KeyboardAction {Invalid = -1, StartStop, Split, Reset, ActionCount};

class KeybindManager
{

public:
	KeybindManager(Timer* timer);
	void tick();

	std::map<KeyboardAction, sf::Keyboard::Key> keybinds;
	std::map<KeyboardAction, int> joystickButtons;
	KeyboardAction rebind;
private:
	void keyPressed(sf::Keyboard::Key key);
	void buttonPressed(int button);
	void saveKeybinds();

	bool* isKeyDown;
	bool* isButtonDown;
	int buttonCount;
	Timer* timer;

};

