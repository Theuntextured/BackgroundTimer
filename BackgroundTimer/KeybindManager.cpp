#include "KeybindManager.h"

KeybindManager::KeybindManager(Timer* timer)
{
	isKeyDown = new bool[keyCount];
	for (int i = 0; i < keyCount; i++) {
		isKeyDown[i] = false;
	}

	isButtonDown = new bool[69];
	for (int i = 0; i < 69; i++) {
		isButtonDown[i] = false;
	}

	keybinds = {
		{KeyboardAction::StartStop, sf::Keyboard::Num1},
		{KeyboardAction::Split, sf::Keyboard::Num2},
		{KeyboardAction::Reset, sf::Keyboard::Num3}
	};
	joystickButtons = {
		{KeyboardAction::StartStop, 7},
		{KeyboardAction::Split, 11},
		{KeyboardAction::Reset, 9}
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
			if (i < 3) {
				keybinds[static_cast<KeyboardAction>(i)] = static_cast<sf::Keyboard::Key>(std::stoi(line));
			}
			else {
				joystickButtons[static_cast<KeyboardAction>(i - 3)] = std::stoi(line);
			}

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
	buttonCount = sf::Joystick::getButtonCount(0);
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


	for (int i = 0; i < buttonCount; i++) {
		if (sf::Joystick::isButtonPressed(0, i)) {
			if (rebind != Invalid) {
				joystickButtons[rebind] = i;
				rebind = Invalid;
				isButtonDown[i] = true;
				saveKeybinds();
			}
			else if (!isButtonDown[i]) {
				isButtonDown[i] = true;
				buttonPressed(i);
			}

		}
		else {
			isButtonDown[i] = false;
		}
	}
}

void KeybindManager::buttonPressed(int button) {
	KeyboardAction action;
	for (int i = 0; i < KeyboardAction::ActionCount; i++) {
		action = static_cast<KeyboardAction>(i);

		if (joystickButtons[action] == button) {

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
	for (int i = 0; i < KeyboardAction::ActionCount; i++) {
		config << joystickButtons[static_cast<KeyboardAction>(i)] << "\n";
	}
	config.close();
}
