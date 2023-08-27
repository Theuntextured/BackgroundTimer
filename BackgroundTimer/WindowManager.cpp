#include "WindowManager.h"
#include <Windows.h>
#include <Dwmapi.h>

#pragma comment (lib, "Dwmapi.lib")

WindowManager::WindowManager(Timer* timer, KeybindManager* kbm)
{
    this->timer = timer;
    this->kbm = kbm;
    grabbedWindow = false;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    windowSize = sf::VideoMode(400, 400);
	window.create(windowSize, "Background Timer", sf::Style::None, settings);
	window.setFramerateLimit(30);
    font.loadFromFile("Anonymous_Pro.ttf");
    arial.loadFromFile("arial.ttf");
    sf::Image icon;
    icon.loadFromFile("icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    MARGINS margins;
    margins.cxLeftWidth = -1;
    SetWindowLong(window.getSystemHandle(), GWL_STYLE, WS_POPUP | WS_VISIBLE);
    DwmExtendFrameIntoClientArea(window.getSystemHandle(), &margins);


    closeButtonTex.loadFromFile("closeButton.png");
    closeButtonTex.setSmooth(true);
    closeButton.setRadius(16.0f);
    closeButton.setTexture(&closeButtonTex);
    closeButton.setOrigin(closeButton.getRadius(), closeButton.getRadius());
    closeButton.setPosition(windowSize.width - 32.0f, 32.0f);
    closeButton.setFillColor(sf::Color(255, 255, 255, 255));

    mainTimerText.setString("00:00:000");
    mainTimerText.setFont(font);
    mainTimerText.setCharacterSize(32);
    mainTimerText.setFillColor(sf::Color(255, 255, 255));
    mainTimerText.setStyle(sf::Text::Bold);
    mainTimerText.setOrigin(mainTimerText.getLocalBounds().width / 2, mainTimerText.getLocalBounds().height);
    mainTimerText.setPosition(windowSize.width / 2, 48.0f);

    for (int i = 0; i < 3; i++) {
        bindButtons[i] = KeyBindButton(static_cast<KeyboardAction>(i), &arial, kbm);
        bindButtons[i].setPosition(float(windowSize.width) * (2 * float(i) + 1.0f) / 6.0f, windowSize.height - 64.0f);
    }

    splitsBorder.setSize(sf::Vector2f(windowSize.width - 24.0f, 180.0f));
    splitsBorder.setPosition(12.0f, 64.0f);
    splitsBorder.setOutlineThickness(4.0f);

    splitsTitleText.setFont(font);
    splitsTitleText.setCharacterSize(24);
    splitsTitleText.setFillColor(sf::Color::White);
    splitsTitleText.setString("Splits:");
    splitsTitleText.setOrigin(0, splitsTitleText.getLocalBounds().height * 1.7f);
    splitsTitleText.setPosition(splitsBorder.getPosition());
    splitsTitleText.setStyle(sf::Text::Bold);

    splitsText.setFont(font);
    splitsText.setCharacterSize(14);
    splitsText.setFillColor(sf::Color::White);
    splitsText.setPosition(splitsBorder.getPosition() + sf::Vector2f(8.0f, 16.0f));
    splitsText.setLineSpacing(1.5f);

    splitsBarText = splitsText;
    splitsBarText.setStyle(sf::Text::Underlined);
    splitsBarText.setString(" Split    Total Time    Split Time    Var    ");

}

std::string getSplitsString(Timer* timer) {
    std::string str = "\n ";
    sf::Time tmp;

    for (int i = timer->splits.size() - 1; (i >= 0) && (i >= int(timer->splits.size()) - 6); i--) {
        str.append((std::to_string(1 + i) + "                  ").substr(0, 5) + "    " + Timer::getTimeString(timer->splits[i]) + "     ");

        if (i == 0) {
            str.append(Timer::getTimeString(timer->splits[i]));
        }
        else {
            str.append(Timer::getTimeString(timer->splits[i] - timer->splits[i - 1]));
        }
        str.append("     ");

        if (i != 0) {
            if (i == 1) {
                tmp = (timer->splits[i] - timer->splits[i - 1]) - timer->splits[i - 1];
            }
            else {
                tmp = (timer->splits[i] - timer->splits[i - 1]) - (timer->splits[i - 1] - timer->splits[i - 2]);
            }

            if (tmp >= sf::Time()) {
                str.append("+");
            }
            else {
                str.append("-");
            }

            str.append(Timer::getTimeString(tmp).substr(3));
        }

        str.append("\n ");
    }

    return str;
}

void WindowManager::drawComponents(Timer* timer) {


    std::string mainTimerStr = Timer::getTimeString((timer->state == Running) ? timer->currentTimeElapsed + timer->mainTimer.getElapsedTime() : timer->currentTimeElapsed);
    mainTimerText.setString(mainTimerStr);

    for (int i = 0; i < 3; i++) {
        bindButtons[i].updateButton(&window, isHovering);
    }

    splitsText.setString(getSplitsString(timer));

    window.draw(mainTimerText);
    splitsBorder.setFillColor(sf::Color(50, 50, 50, isHovering ? 255 : 80));
    splitsBorder.setOutlineColor(sf::Color(0, 0, 0, isHovering ? 255 : 80));
    window.draw(splitsBorder);
    if (isHovering) window.draw(splitsTitleText);
    window.draw(splitsText);
    window.draw(splitsBarText);
    if(isHovering) window.draw(closeButton);
}

void WindowManager::Tick()
{
    auto mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
    isHovering = mousePos.x >= 0 && mousePos.y >= 0 && mousePos.x <= windowSize.width && mousePos.y <= windowSize.height;

    // check all the window's events that were triggered since the last iteration of the loop
    sf::Event event;
    while (window.pollEvent(event))
    {
        auto mousePos = sf::Vector2f(sf::Mouse::getPosition(window)) - closeButton.getPosition();
        if ((mousePos.x * mousePos.x + mousePos.y * mousePos.y) <= closeButton.getRadius() * closeButton.getRadius()) {
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                window.close();
            }
            closeButton.setFillColor(sf::Color(255, 0, 0, 255));
        }
        else {
            closeButton.setFillColor(sf::Color(255, 255, 255, 255));
        }
        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                grabbedOffset = window.getPosition() - sf::Mouse::getPosition();
                grabbedWindow = true;
                window.setFramerateLimit(360);
            }
        }
        else if (event.type == sf::Event::MouseButtonReleased)
        {
            if (event.mouseButton.button == sf::Mouse::Left) {
                grabbedWindow = false;
                window.setFramerateLimit(30);
            }
        }
        else if (event.type == sf::Event::MouseMoved)
        {
            if (grabbedWindow)
                window.setPosition(sf::Mouse::getPosition() + grabbedOffset);
        }
    }

    window.clear(isHovering ? sf::Color(35, 35, 35, 95) : sf::Color::Transparent);
    drawComponents(timer);
    window.display();

    HWND hwnd = window.getSystemHandle();
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

