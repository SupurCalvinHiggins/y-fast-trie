#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

enum button_states{
    button_state_normal=0,
    button_state_hover,
    button_state_active
};

namespace GUI
{
    class Button {
    private:
        sf::RectangleShape rect_shape;
        sf::Font *font;
        sf::Text text;

        short unsigned button_state;

        sf::Color text_color;
        sf::Color text_hover_color;
        sf::Color text_active_color;
        sf::Color button_color;
        sf::Color hover_color;
        sf::Color active_color;

    public:
        Button(float x, float y, float width, float height, sf::Font *font, std::string text, unsigned charecter_size, sf::Color text_color, sf::Color text_hover_color, sf::Color text_active_color, sf::Color button_color, sf::Color hover_color, sf::Color active_color);
        ~Button();

        // Accessors
        const bool isClicked() const;
        const std::string getText() const;

        void setText(const std::string text);

        void update(const sf::Vector2f &mouse_pos);
        void render(sf::RenderTarget &target);
    };

    class DropDownList {
    private:
        GUI::Button *slection;
        std::vector<GUI::Button*> list;
        sf::Font &font;
       
        bool is_list_open;
        float key_timer;
        float key_timer_max;

    public:
        DropDownList(float x, float y, float width, float height, sf::Font &font, std::vector<std::string> &button_names);
        ~DropDownList();

        const bool getKeyTimer();

        void updateKeyTime(const float &dt);
        void update(const sf::Vector2f &mouse_pos, const float &dt);
        void render(sf::RenderTarget &target);
    };
}