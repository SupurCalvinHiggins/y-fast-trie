#include "gui.h"

////////////////// BUTTON //////////////////

GUI::Button::Button(float x, float y, float width, float height, sf::Font *font, std::string text, unsigned charecter_size,
    sf::Color text_color, sf::Color text_hover_color, sf::Color text_active_color,
    sf::Color button_color, sf::Color hover_color, sf::Color active_color,
    unsigned short button_id, sf::Color outline_color, sf::Color outline_hover_color, sf::Color outline_active_color) {
    this->button_state = button_state_normal;
    this->button_id = button_id;

    this->rect_shape.setPosition(sf::Vector2f(x, y));
    this->rect_shape.setSize(sf::Vector2f(width, height));
    this->rect_shape.setFillColor(this->button_color);
    this->rect_shape.setOutlineColor(this->outline_color);
    this->rect_shape.setOutlineThickness(1.0f);

    this->font = font;
    this->text.setFont(*font);
    this->text.setString(text);
    this->text.setFillColor(text_color);
    this->text.setCharacterSize(charecter_size);
    this->text.setPosition(this->rect_shape.getPosition().x + this->rect_shape.getGlobalBounds().width /2.f - this->text.getGlobalBounds().width / 2.f,
        this->rect_shape.getPosition().y + this->rect_shape.getGlobalBounds().height /2.f - this->text.getGlobalBounds().height / 2.f);

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

GUI::Button::~Button() {
}

const bool GUI::Button::isClicked() const {
    return this->button_state == button_state_active;
}

const std::string GUI::Button::getText() const {
    return this->text.getString();
}

const unsigned short &GUI::Button::getButtonID() const {
    return this->button_id;
}

void GUI::Button::setID(const unsigned short button_id) {
    this->button_id = button_id;
}

void GUI::Button::setText(const std::string text) {
    this->text.setString(text);
}

void GUI::Button::update(const sf::Vector2f &mouse_pos) {

    // Mouse is not over button
    this->button_state = button_state_normal;
    this->rect_shape.setFillColor(this->button_color);
    this->rect_shape.setOutlineColor(this->outline_color);
    this->text.setFillColor(this->text_color);

    // Check if mouse is hovering over button
    if (this->rect_shape.getGlobalBounds().contains(mouse_pos)) {
        if (this->button_state != button_state_hover) {
            this->button_state = button_state_hover;
            this->rect_shape.setFillColor(this->hover_color);
            this->rect_shape.setOutlineColor(this->outline_hover_color);
            this->text.setFillColor(this->text_hover_color);
        }
        
        // Check if mouse is clicking on button
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            this->button_state = button_state_active;
            this->rect_shape.setFillColor(this->active_color);   
            this->rect_shape.setOutlineColor(this->outline_active_color);
            this->text.setFillColor(this->text_active_color);
        }
    }
}

void GUI::Button::render(sf::RenderTarget &target) {
    target.draw(this->rect_shape);
    target.draw(this->text);
}



////////////////// DROP DOWN LIST //////////////////
GUI::DropDownList::DropDownList(float x, float y, float width, float height, sf::Font &font, std::vector<std::string> button_names) : font(font), is_list_open(false), key_timer_max(0.25f), key_timer(0.f) {
    this->slection = new GUI::Button(x, y, width, height, &this->font, button_names[0], 35, sf::Color(238, 238, 238), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50), sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200), 0, sf::Color(255,255,255,200), sf::Color::White, sf::Color(20, 20, 20, 50));

    for (size_t i = 0; i < button_names.size(); i++) {
        this->list.push_back(new GUI::Button(x, y + ((i+1) * height), width, height, &this->font, button_names[i], 35, sf::Color(255,255,255,150), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50), sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200), i));
    }
}

GUI::DropDownList::~DropDownList() {
    delete this->slection;

    for (size_t i = 0; i < this->list.size(); i++) {
        delete this->list[i];
    }
}

const bool GUI::DropDownList::getKeyTimer() {
    if (this->key_timer >= this->key_timer_max) {
        this->key_timer = 0.f;
        return true;
    }

    return false;
}

const unsigned short &GUI::DropDownList::getSelectionID() const {
    return this->slection->getButtonID();
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

            if (button->isClicked() && this->getKeyTimer()) {
                this->slection->setText(button->getText());
                this->slection->setID(button->getButtonID());
                this->is_list_open = false;
            }
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

////////////////// TEXT BOXES //////////////////
GUI::TextBox::TextBox(float x, float y, float width, float height, unsigned short char_limit, unsigned short charecter_size, unsigned short ascii_range_min, unsigned short ascii_range_max, sf::Font &font, sf::Color text_color, sf::Color shape_color, sf::Color outline_color) : key_timer_max(0.225f), key_timer(0.f), cursor_timer_max(0.5f), cursor_timer(0.f) {
    this->is_active = false;
    this->is_entered = false;
    this->show_cursor = true;

    this->rect_shape.setPosition(sf::Vector2f(x, y));
    this->rect_shape.setSize(sf::Vector2f(width, height));
    this->rect_shape.setFillColor(shape_color);
    this->rect_shape.setOutlineColor(outline_color);
    this->rect_shape.setOutlineThickness(1.f);
    
    this->text.setFont(font);
    this->text.setFillColor(text_color);
    this->text.setCharacterSize(charecter_size);
    this->text.setPosition(sf::Vector2f(x, y));

    this->char_limit = char_limit;
    this->ascii_range_min = ascii_range_min;
    this->ascii_range_max = ascii_range_max;

    this->text_color = text_color;
    this->shape_color = shape_color;
    this->outline_color = outline_color;
}

GUI::TextBox::~TextBox() {
}

const bool GUI::TextBox::getKeyTimer() {
    if (this->key_timer >= this->key_timer_max) {
        this->key_timer = 0.f;
        return true;
    }

    return false;
}

const std::string GUI::TextBox::getText() const {
    return this->text.getString();
}

const bool GUI::TextBox::isTextEntered() {
    if (this->is_entered) {
        this->is_entered = false;
        return true;
    }
    
    return false;
}

void GUI::TextBox::animateCursor(const float &dt) {
    if (this->cursor_timer < this->cursor_timer_max) {
        this->cursor_timer += dt;
    } else {
        show_cursor = !show_cursor;
        this->cursor_timer = 0.f;
    }

    text.setString(text_str + (show_cursor ? '_' : ' '));
}

void GUI::TextBox::updateKeyTime(const float &dt) {
    if (this->key_timer < this->key_timer_max) {
        this->key_timer += dt;
    }
}

void GUI::TextBox::deleteCharacter() {
    if (this->text_str.size() > 0) {
        this->text_str.pop_back();
        this->text.setString(this->text_str);
    }
}

void GUI::TextBox::deleteString() {
    this->text_str.clear();
    this->text.setString(this->text_str);
}

void GUI::TextBox::updateInput(const float &dt, sf::Event event, std::string &user_input, bool &other_box_active) {
    this->animateCursor(dt);

    if (event.text.unicode == 8 && this->getKeyTimer())
        this->deleteCharacter();
    else if (event.key.code == sf::Keyboard::Return && this->getKeyTimer()) {
        this->is_active = false;
        this->is_entered = true;
        other_box_active = false;
        user_input = this->text_str;
        this->deleteString();
    }
    else if (this->text_str.size() < this->char_limit) {
        if (event.text.unicode >= this->ascii_range_min && event.text.unicode <= this->ascii_range_max && this->getKeyTimer()) {
            this->text_str.push_back(event.text.unicode);
            this->text.setString(this->text_str);
        }
    }
}

void GUI::TextBox::update(const sf::Vector2f &mouse_pos, const float &dt, sf::Event event, std::string &user_input, bool &other_box_active) {
    this->updateKeyTime(dt);

    if (this->rect_shape.getGlobalBounds().contains(mouse_pos) && !other_box_active) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            this->is_active = true;
            other_box_active = true;
        }
    }

    if (this->is_active) {
        rect_shape.setOutlineColor(this->outline_color);
        this->updateInput(dt, event, user_input, other_box_active);
    }
    else {
        rect_shape.setOutlineColor(sf::Color(128,128,128));
    }
}

void GUI::TextBox::render(sf::RenderTarget &target) {
    target.draw(this->rect_shape);
    target.draw(this->text);
}