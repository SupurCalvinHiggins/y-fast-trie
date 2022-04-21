#include "gui.h"

////////////////// BUTTON //////////////////

GUI::Button::Button(float x, float y, float width, float height, sf::Font *font, std::string text, unsigned charecter_size, sf::Color text_color, sf::Color text_hover_color, sf::Color text_active_color, sf::Color button_color, sf::Color hover_color, sf::Color active_color) {
    this->button_state = button_state_normal;
    
    this->rect_shape.setPosition(sf::Vector2f(x, y));
    this->rect_shape.setSize(sf::Vector2f(width, height));
    this->rect_shape.setFillColor(this->button_color);
    
    this->font = font;
    this->text.setFont(*font);
    this->text.setString(text);
    this->text.setFillColor(text_color);
    this->text.setCharacterSize(charecter_size);
    this->text.setPosition(x + (width / 2) - (this->text.getGlobalBounds().width / 2), y + (height / 2) - (this->text.getGlobalBounds().height / 2));

    this->text_color = text_color;
    this->text_hover_color = text_hover_color;
    this->text_active_color = text_active_color;
    this->button_color = button_color;
    this->hover_color = hover_color;
    this->active_color = active_color;
}

GUI::Button::~Button() {
}

const bool GUI::Button::isClicked() const {
    return this->button_state == button_state_active;
}

const std::string GUI::Button::getText() const {
    return this->text.getString();
}

void GUI::Button::setText(const std::string text) {
    this->text.setString(text);
}

//TODO: Add timer to limit how often buttons can be clicked
void GUI::Button::update(const sf::Vector2f &mouse_pos) {

    // Mouse is not over button
    this->button_state = button_state_normal;
    this->rect_shape.setFillColor(this->button_color);
    this->text.setFillColor(this->text_color);

    // Check if mouse is hovering over button
    if (this->rect_shape.getGlobalBounds().contains(mouse_pos)) {
        if (this->button_state != button_state_hover) {
            this->button_state = button_state_hover;
            this->rect_shape.setFillColor(this->hover_color);
            this->text.setFillColor(this->text_hover_color);
        }
        
        // Check if mouse is clicking on button
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            this->button_state = button_state_active;
            this->rect_shape.setFillColor(this->active_color);   
            this->text.setFillColor(this->text_active_color);
        }
    }
}

void GUI::Button::render(sf::RenderTarget &target) {
    target.draw(this->rect_shape);
    target.draw(this->text);
}



////////////////// DROP DOWN LIST //////////////////
GUI::DropDownList::DropDownList(float x, float y, float width, float height, sf::Font &font, std::vector<std::string> button_names, unsigned default_index) : font(font), is_list_open(false), key_timer_max(0.25f), key_timer(0.f) {

    for (size_t i = 0; i < button_names.size(); i++) {
        this->list.push_back(new GUI::Button(x, y + (i * height), width, height, &this->font, button_names[i], 12, sf::Color(255,255,255,150), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50), sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200)));
    }

    //this->slection = new GUI::Button(*this->list[default_index]);
}

GUI::DropDownList::~DropDownList() {
    delete this->slection;
    for (auto *&button : this->list) {
        delete button;
    }
}

const bool GUI::DropDownList::getKeyTimer() {
    if (this->key_timer >= this->key_timer_max) {
        this->key_timer = 0.f;
        return true;
    }

    return false;
}

void GUI::DropDownList::updateKeyTime(const float &dt) {
    if (this->key_timer < this->key_timer_max) {
        this->key_timer += dt;
    }
}

void GUI::DropDownList::update(const sf::Vector2f &mouse_pos, const float &dt) {
    this->updateKeyTime(dt);
    this->slection->update(mouse_pos);

    if (this->slection->isClicked() && this->getKeyTimer()) {
        this->is_list_open = !this->is_list_open;
    }

    if (this->is_list_open) {
        for (auto &button : this->list) {
            button->update(mouse_pos);
        }
    }

}

void GUI::DropDownList::render(sf::RenderTarget &target) {
    this->slection->render(target);

    if (this->is_list_open) {
        for (auto &button : this->list) {
            button->render(target);
        }
    }
}