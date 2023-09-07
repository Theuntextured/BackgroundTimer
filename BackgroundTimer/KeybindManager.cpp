#include "KeybindManager.h"

KeybindManager::KeybindManager(Timer* timer)
{
	isKeyDown = new bool[keyCount];
	for (int i = 0; i < keyCount; i++) {
		isKeyDown[i] = false;
	}

//50 buttons max per joystick * 8 max joysticks
	isButtonDown = new bool[400];
	for (int i = 0; i < 400; i++) {
		isButtonDown[i] = false;
	}

	keybinds = {
		{KeyboardAction::StartStop, sf::Keyboard::Num1},
		{KeyboardAction::Split, sf::Keyboard::Num2},
		{KeyboardAction::Reset, sf::Keyboard::Num3}
	};
	joystickButtons = {
		{KeyboardAction::StartStop, joystickButton()},
		{KeyboardAction::Split, joystickButton()},
		{KeyboardAction::Reset, joystickButton()}
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
			else if(i < 6){
				joystickButtons[static_cast<KeyboardAction>(i - 3)].buttonID = std::stoi(line);
			}
			else {
				joystickButtons[static_cast<KeyboardAction>(i - 6)].joystickID = std::stoi(line);
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

	//get total button count
	for (int i = 0; i < 8; i++) {
		buttonCount[i] = (sf::Joystick::isConnected(i) ? sf::Joystick::getButtonCount(i) : 0);
	}
	joystickButton tmp;
	for (int n = 0; n < 8; n++) {
		for (int i = 0; i < buttonCount[n]; i++) {
			if (sf::Joystick::isButtonPressed(0, i)) {
				tmp.joystickID = sf::Joystick::getIdentification(n).productId;
				tmp.buttonID = i;
				if (rebind != Invalid) {
					joystickButtons[rebind] = tmp;
					rebind = Invalid;
					isButtonDown[50 * n + i] = true;
					saveKeybinds();
				}
				else if (!isButtonDown[50 * n + i]) {
					isButtonDown[50 * n + i] = true;
					buttonPressed(tmp);
				}

			}
			else {
				isButtonDown[50 * n + i] = false;
			}
		}
	}
}

void KeybindManager::buttonPressed(joystickButton button) {
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
		config << joystickButtons[static_cast<KeyboardAction>(i)].buttonID << "\n";
	}
	for (int i = 0; i < KeyboardAction::ActionCount; i++) {
		config << joystickButtons[static_cast<KeyboardAction>(i)].joystickID << "\n";
	}
	config.close();
}
