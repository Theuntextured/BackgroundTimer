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
	KeyboardAction rebind;
private:
	void keyPressed(sf::Keyboard::Key key);
	void saveKeybinds();

	bool* isKeyDown;
	Timer* timer;

};

