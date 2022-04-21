#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stack>
#include <map>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include "tools/gui.h"

class ConsoleMenu {
private:
    sf::RectangleShape background;
    sf::RectangleShape console;
    sf::Font &font;
    sf::Text console_text;

    std::map<std::string, GUI::Button*> buttons;

public:
    ConsoleMenu(sf::RenderWindow &window, sf::Font &font);
    virtual ~ConsoleMenu();

    std::map<std::string, GUI::Button*>& getButtons();

    // Functions
    const bool isButtonClicked(const std::string key);
    void addButton(const std::string key, float y, const std::string text);
    void update(const sf::Vector2f &mouse_pos);
    void render(sf::RenderTarget &target);
};