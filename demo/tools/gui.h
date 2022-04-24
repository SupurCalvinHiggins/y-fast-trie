#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>

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

        unsigned short button_state;
        unsigned short button_id;

        sf::Color text_color;
        sf::Color text_hover_color;
        sf::Color text_active_color;
        sf::Color button_color;
        sf::Color hover_color;
        sf::Color active_color;
        sf::Color outline_color;
        sf::Color outline_hover_color;
        sf::Color outline_active_color;

    public:
        Button(float x, float y, float width, float height, sf::Font *font, std::string text, unsigned charecter_size,
        sf::Color text_color, sf::Color text_hover_color, sf::Color text_active_color,
        sf::Color button_color, sf::Color hover_color, sf::Color active_color,
        unsigned short button_id = 0, sf::Color outline_color = sf::Color::Transparent, sf::Color outline_hover_color = sf::Color::Transparent, sf::Color outline_active_color = sf::Color::Transparent);
        ~Button();

        // Accessors
        const bool isClicked() const;
        const std::string getText() const;
        const unsigned short &getButtonID() const;

        void setID(const unsigned short button_id);
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
        DropDownList(float x, float y, float width, float height, sf::Font &font, std::vector<std::string> button_names);
        ~DropDownList();

        const bool getKeyTimer();
        const unsigned short &getSelectionID() const;

        void updateKeyTime(const float &dt);
        void update(const sf::Vector2f &mouse_pos, const float &dt);
        void render(sf::RenderTarget &target);
    };

    class TextBox {
    private:
        sf::RectangleShape rect_shape;
        std::string text_str;
        sf::Text text;

        bool is_active;
        bool is_entered;
        float key_timer;
        float key_timer_max;
        unsigned short char_limit;
        unsigned short ascii_range_min;
        unsigned short ascii_range_max;

        float cursor_timer;
        float cursor_timer_max;
        bool show_cursor;

        sf::Color text_color;
        sf::Color shape_color;
        sf::Color outline_color;

    public:
        TextBox(float x, float y, float width, float height, unsigned short char_limit, unsigned short charecter_size, unsigned short ascii_range_min, unsigned short ascii_range_max, sf::Font &font, sf::Color text_color, sf::Color shape_color, sf::Color outline_color);
        ~TextBox();

        // Accessors
        const bool getKeyTimer();
        const bool isTextEntered();
        const std::string getText() const; // When enter is pressed

        void animateCursor(const float &dt);
        void updateKeyTime(const float &dt);
        void deleteCharacter(); // When backspace is pressed
        void deleteString(); // When enter is pressed return and erase the string that was typed
        void updateInput(const float &dt, sf::Event event, std::string &user_input);
        void update(const sf::Vector2f &mouse_pos, const float &dt, sf::Event event, std::string &user_input);
        void render(sf::RenderTarget &target);
    };
}