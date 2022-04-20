#pragma once

#include <iostream>
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

enum button_states{
    button_state_normal=0,
    button_state_hover,
    button_state_active
};

class Button {
private:
    sf::RectangleShape button;
    sf::Font *font;
    sf::Text text;

    short unsigned button_state;

    sf::Color button_color;
    sf::Color hover_color;
    sf::Color active_color;

public:
    Button(float x, float y, float width, float height, sf::Font *font, std::string text, sf::Color button_color, sf::Color hover_color, sf::Color active_color);
    ~Button();

    // Accessors
    const bool isClicked() const;

    void update(sf::Vector2f mouse_pos);
    void render(sf::RenderTarget *target);
};