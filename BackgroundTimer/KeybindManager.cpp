#include "KeybindManager.h"

KeybindManager::KeybindManager(Timer* timer)
{
	isKeyDown = new bool[keyCount];
	for (int i = 0; i < keyCount; i++) {
		isKeyDown[i] = false;
	}

	keybinds = {
		{KeyboardAction::StartStop, sf::Keyboard::Num1},
		{KeyboardAction::Split, sf::Keyboard::Num2},
		{KeyboardAction::Reset, sf::Keyboard::Num3}
	};

	this->timer = timer;
	rebind = Invalid;

	std::ifstream config;
	config.open("cfg.txt");
	std::string line;
	if (config.is_open()) {
		std::cout << "Loaded config file.\n";
		int i = 0;
		while (std::getline(config, line)) {
			keybinds[static_cast<KeyboardAction>(i)] = static_cast<sf::Keyboard::Key>(std::stoi(line));
			i++;
		}
		config.close();
	}
	else {
		std::cout << "Could not load config file.\n";
		saveKeybinds();
	}
}

void KeybindManager::tick()
{
	sf::Keyboard::Key key;
	for (int i = 0; i < keyCount; i++) {
		key = static_cast<sf::Keyboard::Key>(i);

		if (sf::Keyboard::isKeyPressed(key)) {
			if (rebind != Invalid) {
				keybinds[rebind] = key;
				rebind = Invalid;
				isKeyDown[i] = true;
				saveKeybinds();
			}
			else if (!isKeyDown[i]) {
				isKeyDown[i] = true;
				keyPressed(key);
			}
		}
		else {
			isKeyDown[i] = false;
		}
	}
}

void KeybindManager::keyPressed(sf::Keyboard::Key key)
{
	KeyboardAction action;
	for (int i = 0; i < KeyboardAction::ActionCount; i++) {
		action = static_cast<KeyboardAction>(i);

		if (keybinds[action] == key) {

				if (action == StartStop) {
					if (timer->state == Running) {
						timer->Pause();
					}
					else {
						timer->Start();
					}
				}

				if (action == Split) {
					timer->saveSplit();
				}
				if (action == Reset) {
					timer->Reset();
				}
			return;
		}
	}
}

void KeybindManager::saveKeybinds()
{
	std::ofstream config;
	config.open("cfg.txt");
	for (int i = 0; i < KeyboardAction::ActionCount; i++) {
		config << keybinds[static_cast<KeyboardAction>(i)] << "\n";
	}
	config.close();
}
