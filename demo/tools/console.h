/**
 * @file console.h
 * @author Robert Oganesian (roganesian@uri.edu)
 * @brief Console for the GUI.
 * @version 1.0
 * @date 2022-04-26
 * 
 */

#pragma once

#include <iostream>
#include <string>
#include <stack>
#include <map>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include "gui-tools.h"

class Console {
private:
    // Required SFML variables.
    sf::RectangleShape background;
    sf::RectangleShape console_background;
    sf::Font &font;
    sf::Text title_text;
    sf::Text text_box_names;

private:
    // Console variables.
    GUI::Button* exit_button;
    std::map<std::string, GUI::TextBox*> text_boxes;

    // Indicates if a text box is currently selected.
    bool text_box_active;

    /**
     * @brief Initializes the console's text.
     * 
     */
    void initText() {
        // Create the title text.
        this->title_text.setFont(font);
        this->title_text.setFillColor(sf::Color(255, 255, 255, 200));
        this->title_text.setCharacterSize(60);
        this->title_text.setString("Trie Editor");
        this->title_text.setPosition(this->console_background.getPosition().x + this->console_background.getSize().x / 2.f - this->title_text.getGlobalBounds().width / 2.f, this->console_background.getPosition().y + 15.f);

        // Create the text labels for the text boxes.
        this->text_box_names.setFont(font);
        this->text_box_names.setFillColor(sf::Color(255, 255, 255, 200));
        this->text_box_names.setCharacterSize(45);
        this->text_box_names.setString("Insert: \n\nRemove: \n\nPredecessor: \n\nSuccessor: \n\nContains:");
        this->text_box_names.setPosition(this->console_background.getPosition().x + this->console_background.getSize().x / 2.f - this->title_text.getGlobalBounds().width / 2.f - 25.f, this->console_background.getPosition().y + 120.f);
    }

public:
    /**
     * @brief Construct a new console object
     * 
     * @param window The window to draw to.
     * @param font The font for the text.
     * 
     */
    Console(sf::RenderWindow &window, sf::Font &font) : font(font) {
        this->text_box_active = false;

        // Creates a background that displays when in the console.
        this->background.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
        this->background.setFillColor(sf::Color(20, 20, 20, 100));

        // Creates the console shape.
        this->console_background.setSize(sf::Vector2f(window.getSize().x / 4.f, window.getSize().y - 100.f));
        this->console_background.setFillColor(sf::Color(20, 20, 20, 200));
        this->console_background.setPosition(window.getSize().x / 2.f - this->console_background.getSize().x / 2.f, 30.f);

        this->initText();
    }

    
    virtual ~Console() {
        delete exit_button;

        for (auto it = this->text_boxes.begin(); it != this->text_boxes.end(); it++)
            delete it->second;
    }

public:
    /**
     * @brief Adds a button to the console.
     * 
     * @param y The y position of the button.
     * @param text The text of the button.
     * 
     */
    void addButton(float y, const std::string text) {
        float width = 150.f;
        float height = 50.f;
        float x = this->console_background.getPosition().x + this->console_background.getSize().x / 2.f - width / 2.f;

        this->exit_button = new GUI::Button(x, y, width, height, &this->font, text, 50, sf::Color(212, 17, 17, 200), sf::Color::Red, sf::Color(20, 20, 20, 50), sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent);
    }

    /**
     * @brief Adds a text box to the console.
     * 
     * @param key The key in the map that will be used to access the text box.
     * @param y The y position of the text box.
     * 
     */
    void addTextBox(const std::string key, float y) {
        float width = 300.f;
        float height = 50.f;
        float x = this->console_background.getPosition().x + this->console_background.getSize().x / 2.f - width / 2.f;

        this->text_boxes[key] = new GUI::TextBox(x, y, width, height, 20, 30, 48, 57, this->font, sf::Color::Black, sf::Color::White, sf::Color::Blue);
    }

    /**
     * @brief Checks if the exit button was clicked.
     * 
     * @return if the exit button was clicked.
     * 
     */
    const bool isButtonClicked() {
        return this->exit_button->isClicked();
    }
    
    /**
     * @brief Checks if the text box was returned.
     * 
     * @return if the text box was returned.
     * 
     */
    const bool isTextBoxEntered(const std::string key) {
        return this->text_boxes.at(key)->isTextEntered();
    }

public:
    /**
     * @brief Updates the console.
     * 
     * @param mouse_pos The current mouse position.
     * @param dt Reference to the delta time variable from gui.h.
     * @param event Event tracked from gui.h.
     * @param user_input Holds the user input from the text box.
     * 
     */
    void update(const sf::Vector2f &mouse_pos, const float &dt, sf::Event &event, std::string &user_input) {
        this->exit_button->update(mouse_pos);

        for (auto &it : this->text_boxes)
            it.second->update(mouse_pos, dt, event, user_input, this->text_box_active);
    }

    /**
     * @brief Renders/draws the console.
     * 
     * @param target Reference to the base SFML RenderTarget.
     * 
     */
    void render(sf::RenderTarget &target) {
        target.draw(this->background);
        target.draw(this->console_background);

        this->exit_button->render(target);

        for (auto &it : this->text_boxes)
            it.second->render(target);

        target.draw(this->title_text);
        target.draw(this->text_box_names);
    }
};