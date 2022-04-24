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
    sf::Text title_text;
    sf::Text text_box_names;

    std::map<std::string, GUI::Button*> buttons; // ! Don't need map since textboxes immediately return the text
    std::map<std::string, GUI::TextBox*> text_boxes;
    bool text_box_active;

    void initText();

public:
    ConsoleMenu(sf::RenderWindow &window, sf::Font &font);
    virtual ~ConsoleMenu();

    std::map<std::string, GUI::Button*>& getButtons();

    // Functions
    const bool isButtonClicked(const std::string key);
    const bool isTextBoxEntered(const std::string key);

    void addTextBox(const std::string key, float y);
    void addButton(const std::string key, float y, const std::string text);
    void update(const sf::Vector2f &mouse_pos, const float &dt, sf::Event event, std::string &user_input);
    void render(sf::RenderTarget &target);
};