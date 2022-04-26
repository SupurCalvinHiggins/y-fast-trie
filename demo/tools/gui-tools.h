/**
 * @file gui-tools.h
 * @author Robert Oganesian (roganesian@uri.edu)
 * @brief GUI tools library.
 * @version 1.0
 * @date 2022-04-26
 * 
 */

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

// Contains the different states a button can be in.
enum button_states{
    button_state_normal=0,
    button_state_hover,
    button_state_active
};

namespace GUI
{
/**
 * @brief Creates a interactive button.
 * 
 */
    class Button {
    private:
        // Required SFML variables.
        sf::RectangleShape rect_shape;
        sf::Font *font;
        sf::Text text;

        sf::Color text_color;
        sf::Color text_hover_color;
        sf::Color text_active_color;
        sf::Color button_color;
        sf::Color hover_color;
        sf::Color active_color;
        sf::Color outline_color;
        sf::Color outline_hover_color;
        sf::Color outline_active_color;
    
    private:
        // Holds the current state of the button.
        unsigned short button_state;

        // Holds the ID of the button.
        unsigned short button_id;

    public:
        /**
         * @brief Construct a new Button object.
         * 
         * @param x The x position of the button.
         * @param y The y position of the button.
         * @param width The width of the button.
         * @param height The height of the button.
         * @param font The font of the button's text.
         * @param text The text of the button.
         * @param charecter_size The character size of the button's text.
         * @param text_color The color of the button's text.
         * @param text_hover_color The color of the button's text when hovered.
         * @param text_active_color The color of the button's text when clicked.
         * @param button_color The color of the button (rectangle shape).
         * @param hover_color The color of the button (rectange shape) when hovered.
         * @param active_color The color of the button (rectangle shape) when clicked.
         * @param button_id The ID of the button.
         * @param outline_color The color of the button's outline.
         * @param outline_hover_color The color of the button's outline when hovered.
         * @param outline_active_color The color of the button's outline when clicked.
         * 
         */
        Button(float x, float y, float width, float height, sf::Font *font, std::string text, unsigned charecter_size,
                sf::Color text_color, sf::Color text_hover_color, sf::Color text_active_color,
                sf::Color button_color, sf::Color hover_color, sf::Color active_color, unsigned short button_id = 0,
                sf::Color outline_color = sf::Color::Transparent, sf::Color outline_hover_color = sf::Color::Transparent, sf::Color outline_active_color = sf::Color::Transparent) {
            // Updating button information.
            this->button_state = button_state_normal;
            this->button_id = button_id;

            // Creating button shape (rectangle).
            this->rect_shape.setPosition(sf::Vector2f(x, y));
            this->rect_shape.setSize(sf::Vector2f(width, height));
            this->rect_shape.setFillColor(this->button_color);
            this->rect_shape.setOutlineColor(this->outline_color);
            this->rect_shape.setOutlineThickness(1.0f);

            // Creating button text.
            this->font = font;
            this->text.setFont(*font);
            this->text.setString(text);
            this->text.setFillColor(text_color);
            this->text.setCharacterSize(charecter_size);
            this->text.setPosition(this->rect_shape.getPosition().x + this->rect_shape.getGlobalBounds().width /2.f - this->text.getGlobalBounds().width / 2.f,
                this->rect_shape.getPosition().y + this->rect_shape.getGlobalBounds().height /2.f - this->text.getGlobalBounds().height / 2.f);
            
            // Updating color variables with given colors.
            this->text_color = text_color;
            this->text_hover_color = text_hover_color;
            this->text_active_color = text_active_color;
            this->button_color = button_color;
            this->hover_color = hover_color;
            this->active_color = active_color;
            this->outline_color = outline_color;
            this->outline_hover_color = outline_hover_color;
            this->outline_active_color = outline_active_color;
        }

        /**
         * @brief Set an ID to the button.
         * 
         * @param button_id An unsigned short ID.
         * 
         */
        void setID(const unsigned short button_id) {
            this->button_id = button_id;
        }

        /**
         * @brief Set the text of the button.
         * 
         * @param text A string of text.
         * 
         */
        void setText(const std::string text) {
            this->text.setString(text);
        }

    public:
        /**
         * @brief Get if the button is clicked.
         * 
         * @return indication if button is active (clicked) or not.
         * 
         */
        const bool isClicked() const {
            return this->button_state == button_state_active;
        }

        /**
         * @brief Get the text of the button.
         * 
         * @return the text of the button.
         * 
         */
        const std::string getText() const {
            return this->text.getString();
        }

        /**
         * @brief Get the ID of the button.
         * 
         * @return the ID of the button.
         * 
         */
        const unsigned short &getButtonID() const {
            return this->button_id;
        }

    public:
        /**
         * @brief Update the button.
         * 
         * @param mouse_pos The current mouse position.
         *
         */
        void update(const sf::Vector2f &mouse_pos) {
            // Mouse is not over button.
            this->button_state = button_state_normal;
            this->rect_shape.setFillColor(this->button_color);
            this->rect_shape.setOutlineColor(this->outline_color);
            this->text.setFillColor(this->text_color);

            // Check if mouse is hovering over button.
            if (this->rect_shape.getGlobalBounds().contains(mouse_pos)) {
                // Update button state to represent hovering over button.
                if (this->button_state != button_state_hover) {
                    this->button_state = button_state_hover;
                    this->rect_shape.setFillColor(this->hover_color);
                    this->rect_shape.setOutlineColor(this->outline_hover_color);
                    this->text.setFillColor(this->text_hover_color);
                }
                
                // Check if mouse is clicking on button.
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    // Update button state to represent clicking on button.
                    this->button_state = button_state_active;
                    this->rect_shape.setFillColor(this->active_color);   
                    this->rect_shape.setOutlineColor(this->outline_active_color);
                    this->text.setFillColor(this->text_active_color);
                }
            }
        }

        /**
         * @brief Render (draw) the button.
         * 
         * @param target Reference to the base SFML RenderTarget.
         * 
         */
        void render(sf::RenderTarget &target) {
            target.draw(this->rect_shape);
            target.draw(this->text);
        }
    };

    /**
     * @brief Creates a interactive drop-down list.
     * 
     */
    class DropDownList {
    private:
        // Button representing selected item from the list.
        GUI::Button *slection;

        // List of buttons representing the items in the list.
        std::vector<GUI::Button*> list;
       
        sf::Font &font;
    
    private:
        // Indicates if drop down list is open or not.
        bool is_list_open;

        // Limits input given to buttons by placing time interval in-between input.
        float key_timer;
        float key_timer_max;

    public:
        /**
        * @brief Construct a new DropDownList object.
        * 
        * @details A drop-down list is a collection of buttons that work together to form a list.
        * 
        * @param x The x position of the buttons
        * @param y The y position of the selection button.
        * @param width The width of the buttons.
        * @param height The height of the buttons.
        * @param font The font of the button's text.
        * @param button_names A vector of strings that represent the names of the buttons (in the list).
        * 
        */
        DropDownList(float x, float y, float width, float height, sf::Font &font, std::vector<std::string> button_names) : font(font), is_list_open(false), key_timer_max(0.25f), key_timer(0.f) {
            // The first button will display the selected item.
            this->slection = new GUI::Button(x, y, width, height, &this->font, button_names[0], 35, sf::Color(238, 238, 238), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50), sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200), 0, sf::Color(255,255,255,200), sf::Color::White, sf::Color(20, 20, 20, 50));

            for (size_t i = 0; i < button_names.size(); i++)
                // Every button in the list is put under the selection button.
                this->list.push_back(new GUI::Button(x, y + ((i+1) * height), width, height, &this->font, button_names[i], 35, sf::Color(255,255,255,150), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50), sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200), i));
        }

        /**
         * @brief Destroy the DropDownList object.
         * 
         */
        ~DropDownList() {
            delete this->slection;

            for (size_t i = 0; i < this->list.size(); i++)
                delete this->list[i];
        }

        /**
         * @brief Get the ID of the selected item.
         * 
         * @return the button ID of the selected item.
         * 
         */
        const unsigned short &getSelectionID() const {
            return this->slection->getButtonID();
        }
    
    public:
        /**
         * @brief Check if enough time has passed since the last GUI tool input.
         * 
         * @return if enough time has passed since the last GUI tool input.
         * 
         */
        const bool getKeyTimer() {
            if (this->key_timer >= this->key_timer_max) {
                // Reset the timer.
                this->key_timer = 0.f;
                return true;
            }

            return false;
        }

        /**
         * @brief Updates the key timer.
         * 
         * @param dt Reference to the delta time variable from gui.h.
         */
        void updateKeyTime(const float &dt) {
            if (this->key_timer < this->key_timer_max)
                this->key_timer += dt;
        }

        /**
         * @brief Update the drop-down list.
         * 
         * @param mouse_pos The current mouse position.
         * @param dt Reference to the delta time variable from gui.h.
         * 
         */
        void update(const sf::Vector2f &mouse_pos, const float &dt) {
            this->updateKeyTime(dt);
            this->slection->update(mouse_pos);

            // Opens or closes the drop-down list based on the current state of the list.
            if (this->slection->isClicked() && this->getKeyTimer()) {
                this->is_list_open = !this->is_list_open;
            }

            // If the list is open, update the buttons.
            if (this->is_list_open) {
                for (auto &button : this->list) {
                    button->update(mouse_pos);

                    // If a button is clicked, update the selection button to contain the clicked button's information. 
                    if (button->isClicked() && this->getKeyTimer()) {
                        this->slection->setText(button->getText());
                        this->slection->setID(button->getButtonID());
                        this->is_list_open = false;
                    }
                }
            }
        }

        /**
         * @brief Render the drop-down list.
         * 
         * @param target Reference to the base SFML RenderTarget.
         * 
         */
        void render(sf::RenderTarget &target) {
            this->slection->render(target);

            if (this->is_list_open) {
                for (auto &button : this->list)
                    button->render(target);
            }
        }
    };

    /**
     * @brief Creates a interactive text box.
     * 
     */
    class TextBox {
    private:
        // Required SFML variables.
        sf::RectangleShape rect_shape;
        std::string text_str;
        sf::Text text;

        sf::Color text_color;
        sf::Color shape_color;
        sf::Color outline_color;
        
    private:
        // Indicates if the current text box is active or not.
        bool is_active;

        // Indicates if the the user has returned (return/enter key) to the text box.
        bool is_entered;

        // Indicates if the cursor is currently visible.
        bool show_cursor;

        // Timer to limit user input in-between last input.
        float key_timer;
        float key_timer_max;

        // Timer for cursor blinking.
        float cursor_timer;
        float cursor_timer_max;
        
        // The max length of the input text.
        unsigned short char_limit;

        // Acceptable characters for the text box.
        unsigned short ascii_range_min;
        unsigned short ascii_range_max;


    public:
        /**
         * @brief Construct a new TextBox object.
         * 
         * @param x The x position of the text box.
         * @param y The y position of the text box.
         * @param width The width of the text box.
         * @param height The height of the text box.
         * @param font The font for the text in the text box.
         * @param char_limit The max length of the input text.
         * @param ascii_range_min The minimum ASCII value of the acceptable characters.
         * @param ascii_range_max The maximum ASCII value of the acceptable characters.
         * @param text_color The color of the text.
         * @param shape_color The color of the text box shape.
         * @param outline_color The color of the text box outline.
         * 
         */
        TextBox(float x, float y, float width, float height, unsigned short char_limit, unsigned short charecter_size, unsigned short ascii_range_min, unsigned short ascii_range_max, sf::Font &font, sf::Color text_color, sf::Color shape_color, sf::Color outline_color) : key_timer_max(0.225f), key_timer(0.f), cursor_timer_max(0.5f), cursor_timer(0.f) {
            this->is_active = false;
            this->is_entered = false;
            this->show_cursor = true;

            // Create the text box shape.
            this->rect_shape.setPosition(sf::Vector2f(x, y));
            this->rect_shape.setSize(sf::Vector2f(width, height));
            this->rect_shape.setFillColor(shape_color);
            this->rect_shape.setOutlineColor(outline_color);
            this->rect_shape.setOutlineThickness(1.f);

            // Setting the text's properties.
            this->text.setFont(font);
            this->text.setFillColor(text_color);
            this->text.setCharacterSize(charecter_size);
            this->text.setPosition(sf::Vector2f(x, y));

            this->char_limit = char_limit;
            this->ascii_range_min = ascii_range_min;
            this->ascii_range_max = ascii_range_max;

            // Updating color variables.
            this->text_color = text_color;
            this->shape_color = shape_color;
            this->outline_color = outline_color;
        }

        /**
         * @brief Return wheter the user has returned from the text box.
         * 
         * @details Checks if the user has pressed the return/enter key in the text box.
         * @return if the user has returned from the text box.
         * 
         */
        const bool isTextEntered() {
            if (this->is_entered) {
                this->is_entered = false;
                return true;
            }
            
            return false;
        }

        /**
         * @brief Returns the current text in the text box.
         * 
         * @return the current text in the text box.
         * 
         */
        const std::string getText() const {
            return this->text.getString();
        }

    public:
        /**
         * @brief Display a cursor in the text box.
         * 
         * @param dt Reference to the delta time variable from gui.h.
         * 
         */
        void animateCursor(const float &dt) {
            if (this->cursor_timer < this->cursor_timer_max)
                this->cursor_timer += dt;
            else {
                // Change the cursor's visibility and reset the timer.
                show_cursor = !show_cursor;
                this->cursor_timer = 0.f;
            }

            // Update if the cursor is visible.
            text.setString(text_str + (show_cursor ? '_' : ' '));
        }

        /**
         * @brief Delete last character in the text box.
         * 
         */
        void deleteCharacter() {
            if (this->text_str.size() > 0) {
                this->text_str.pop_back();
                this->text.setString(this->text_str);
            }
        }

        /**
         * @brief Delete all characters in the text box.
         * 
         */
        void deleteString() {
            this->text_str.clear();
            this->text.setString(this->text_str);
        }
    
    public:
         /**
         * @brief Check if enough time has passed since the last GUI tool input.
         * 
         * @return if enough time has passed since the last GUI tool input.
         * 
         */
        const bool getKeyTimer() {
            if (this->key_timer >= this->key_timer_max) {
                // Reset the timer.
                this->key_timer = 0.f;
                return true;
            }

            return false;
        }

        /**
         * @brief Updates the key timer.
         * 
         * @param dt Reference to the delta time variable from gui.h.
         */
        void updateKeyTime(const float &dt) {
            if (this->key_timer < this->key_timer_max)
                this->key_timer += dt;
        }
        
        /**
         * @brief Update any input from the user in the text box.
         * 
         * @param dt Reference to the delta time variable from gui.h.
         * @param event Event tracked from gui.h.
         * @param user_input Holds the user input from the text box.
         * @param other_box_active Indicates if another text box object is active.
         * 
         */
        void updateInput(const float &dt, sf::Event &event, std::string &user_input, bool &other_box_active) {
            this->animateCursor(dt);

            // If backspace is pressed, delete the last character.
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace) && this->getKeyTimer())
                this->deleteCharacter();
            // If return/enter is pressed, get the text from the text box, then delete it.
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && this->getKeyTimer()) {
                this->is_active = false;
                this->is_entered = true;
                other_box_active = false;
                user_input = this->text_str;
                this->deleteString();
            }
            // Checks if the text in the text box is less than the character limit.
            else if (this->text_str.size() < this->char_limit) {
                // Checks if the user has pressed a key that is within the given ASCII range.
                if (event.type == sf::Event::TextEntered && (event.text.unicode >= this->ascii_range_min && event.text.unicode <= this->ascii_range_max) && this->getKeyTimer()) {
                    this->text_str.push_back(event.text.unicode);
                    this->text.setString(this->text_str);
                }
            }
        }

        /**
         * @brief Update the text box.
         * 
         * @param mouse_pos The current mouse position.
         * @param dt Reference to the delta time variable from gui.h.
         * @param event Event tracked from gui.h.
         * @param user_input Holds the user input from the text box.
         * @param other_box_active Indicates if another text box object is active.
         * 
         */
        void update(const sf::Vector2f &mouse_pos, const float &dt, sf::Event &event, std::string &user_input, bool &other_box_active) {
            this->updateKeyTime(dt);

            // Checks if the mouse is hovering over the text box.
            if (this->rect_shape.getGlobalBounds().contains(mouse_pos) && !other_box_active) {
                // If left mouse button is pressed, set the text box to active.
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    this->is_active = true;
                    other_box_active = true;
                }
            }

            // If the text box is active, update the input.
            if (this->is_active) {
                rect_shape.setOutlineColor(this->outline_color);
                this->updateInput(dt, event, user_input, other_box_active);
            }
            else
                rect_shape.setOutlineColor(sf::Color(128,128,128));
        }

        /**
         * @brief Render the text box.
         * 
         * @param target Reference to the base SFML RenderTarget.
         * 
         */
        void render(sf::RenderTarget &target) {
            target.draw(this->rect_shape);
            target.draw(this->text);
        }
    };
}