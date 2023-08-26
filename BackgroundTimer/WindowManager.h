#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Timer.h"
#include "KeyBindButton.h"

class WindowManager
{
public:
	WindowManager(Timer* timer, KeybindManager* kbm);
	void Tick();

	sf::RenderWindow window;
	sf::VideoMode windowSize;

private:
	void drawComponents(Timer* timer);
	sf::Font font;
	sf::Font arial;
	sf::CircleShape timerCircle;
	sf::Text mainTimerText;
	sf::Text splitsText;
	sf::Text splitsBarText;
	KeyBindButton bindButtons[3];
	sf::RectangleShape splitsBorder;
	sf::Text splitsTitleText;
	bool grabbedWindow;
	sf::Vector2i grabbedOffset;

	Timer* timer;
	KeybindManager* kbm;
};

