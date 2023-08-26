#include "KeyBindButton.h"

KeyBindButton::KeyBindButton()
{
    return;
}

KeyBindButton::KeyBindButton(KeyboardAction action, sf::Font* font, KeybindManager* kbm) : RectangleShape() {

    text.setString("Start/Stop: F2\n");
    text.setFont(*font);
    text.setCharacterSize(10);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);

    this->kbm = kbm;
    this->action = action;

    setSize(sf::Vector2f(112.0f, 32.0f));
    setOutlineThickness(4.0f);
    setOutlineColor(sf::Color::Black);
    setFillColor(sf::Color(50, 50, 50));
    setOrigin(getSize().x / 2, getSize().y / 2);
    isPressed = false;
}

void KeyBindButton::updateButton(sf::RenderWindow* window)
{
    //check if being pressed
    sf::FloatRect bounds = getGlobalBounds();
    auto mousePos = sf::Mouse::getPosition(*window);
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && bounds.contains(sf::Vector2f(mousePos))) {
        if (!isPressed) {
            isPressed = true;
            if (kbm->rebind == Invalid) {
                kbm->rebind = action;
            }
        }
    }
    else {
        isPressed = false;
    }

    if (kbm->rebind != action) {
        text.setString(actionNames[action] + ": " + keyNames[kbm->keybinds[action]]);
    }
    else {
        text.setString(" ... ");
    }

    //draw
    text.setPosition(getPosition());
    text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
    window->draw(*this);
    window->draw(text);
}
