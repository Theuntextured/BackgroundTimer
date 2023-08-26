#include "Timer.h"

Timer::Timer() {
	state = Paused;
}

bool Timer::Start()
{
	if(state != Running) {
		std::cout << "Timer Started\n";
		mainTimer.restart();
		state = Running;
		return true;
	}
	return false;
}

bool Timer::Pause()
{
	if (state == Running) {
		currentTimeElapsed = currentTimeElapsed + mainTimer.getElapsedTime();
		std::cout << "Timer Paused at " << currentTimeElapsed.asSeconds() << " seconds.\n";
		state = Paused;
		return true;
	}
	return false;
}

sf::Time Timer::saveSplit()
{
	if (state == Running) {
		sf::Time currentSplit = mainTimer.getElapsedTime() + currentTimeElapsed;
		splits.push_back(currentSplit);
		std::cout << "Split created: " << currentSplit.asSeconds() << " seconds\n";
		return currentSplit;
	}
}

void Timer::Reset()
{
	currentTimeElapsed = sf::Time();
	splits.clear();
	state = Paused;
	std::cout << "Timer Reset\n\n";
}

std::string Timer::getTimeString(sf::Time t)
{
	if (t < sf::Time()) {
		t = -t;
	}
	std::string zeros = "00000";
	int ms = t.asMilliseconds();
	int m = (ms / 1000) / 60;
	int s = (ms / 1000) % 60;
	ms = ms % 1000;
	return zeros.substr(1, 2 - std::to_string(m).length()) + std::to_string(m) + ":" + zeros.substr(1, 2 - std::to_string(s).length()) + std::to_string(s) + ":" + zeros.substr(1, 3 - std::to_string(ms).length()) + std::to_string(ms);
}


