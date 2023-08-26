#pragma once
#include <SFML/System.hpp>
#include <vector>
#include <iostream>
#include <string>

enum TimerState {Running, Paused};

class Timer
{
public:
	Timer();
	bool Start();
	bool Pause();
	sf::Time saveSplit();
	void Reset();
	static std::string getTimeString(sf::Time t);

	sf::Clock mainTimer;
	sf::Time currentTimeElapsed;
	std::vector<sf::Time> splits;
	TimerState state;
};

