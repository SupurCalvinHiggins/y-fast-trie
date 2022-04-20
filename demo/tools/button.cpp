#include "button.h"

Button::Button(float x, float y, float width, float height, sf::Font *font, std::string text, sf::Color button_color, sf::Color hover_color, sf::Color active_color) {
    this->button_state = button_state_normal;
    
    this->button.setPosition(sf::Vector2f(x, y));
    this->button.setSize(sf::Vector2f(width, height));
    
    this->font = font;
    this->text.setFont(*font);
    this->text.setString(text);
    this->text.setFillColor(sf::Color::White);
    this->text.setCharacterSize(12);
    this->text.setPosition(x + (width / 2) - (this->text.getGlobalBounds().width / 2), y + (height / 2) - (this->text.getGlobalBounds().height / 2));


    this->button_color = button_color;
    this->hover_color = hover_color;
    this->active_color = active_color;

    this->button.setFillColor(this->button_color);
}

Button::~Button() {
}

const bool Button::isClicked() const {
    return this->button_state == button_state_active;
}

void Button::update(sf::Vector2f mouse_pos) {

    // Mouse is not over button
    this->button_state = button_state_normal;
    this->button.setFillColor(this->button_color);

    // Check if mouse is hovering over button
    if (this->button.getGlobalBounds().contains(mouse_pos)) {
        if (this->button_state != button_state_hover) {
            this->button_state = button_state_hover;
            this->button.setFillColor(this->hover_color);
        }
        
        // Check if mouse is clicking on button
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            this->button_state = button_state_active;
            this->button.setFillColor(this->active_color);   
        }
    }
}

void Button::render(sf::RenderTarget *target) {
    target->draw(this->button);
    target->draw(this->text);
}