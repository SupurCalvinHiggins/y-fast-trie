#include "console-menu.h"

void ConsoleMenu::initText() {
    this->title_text.setFont(font);
    this->title_text.setFillColor(sf::Color(255, 255, 255, 200));
    this->title_text.setCharacterSize(60);
    this->title_text.setString("Trie Editor");
    this->title_text.setPosition(this->console.getPosition().x + this->console.getSize().x / 2.f - this->title_text.getGlobalBounds().width / 2.f, this->console.getPosition().y + 15.f);

    this->text_box_names.setFont(font);
    this->text_box_names.setFillColor(sf::Color(255, 255, 255, 200));
    this->text_box_names.setCharacterSize(45);
    this->text_box_names.setString("Insert: \n\nRemove: \n\nPredecessor: \n\nSuccessor: \n\nContains:");
    this->text_box_names.setPosition(this->console.getPosition().x + this->console.getSize().x / 2.f - this->title_text.getGlobalBounds().width / 2.f - 25.f, this->console.getPosition().y + 120.f);
}

ConsoleMenu::ConsoleMenu(sf::RenderWindow &window, sf::Font &font) : font(font) {
    this->text_box_active = false;

    this->background.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    this->background.setFillColor(sf::Color(20, 20, 20, 100));

    this->console.setSize(sf::Vector2f(window.getSize().x / 4.f, window.getSize().y - 100.f));
    this->console.setFillColor(sf::Color(20, 20, 20, 200));
    this->console.setPosition(window.getSize().x / 2.f - this->console.getSize().x / 2.f, 30.f);

    this->initText();
}

ConsoleMenu::~ConsoleMenu() {
    for (auto it = this->buttons.begin(); it != this->buttons.end(); it++) {
        delete it->second;
    }

    for (auto it = this->text_boxes.begin(); it != this->text_boxes.end(); it++) {
        delete it->second;
    }
}

std::map<std::string, GUI::Button*>& ConsoleMenu::getButtons() {
    return this->buttons;
}

const bool ConsoleMenu::isButtonClicked(const std::string key) {
    return this->buttons.at(key)->isClicked();
}

const bool ConsoleMenu::isTextBoxEntered(const std::string key) {
    return this->text_boxes.at(key)->isTextEntered();
}

void ConsoleMenu::addButton(const std::string key, float y, const std::string text) {
    float width = 150.f;
    float height = 50.f;
    float x = this->console.getPosition().x + this->console.getSize().x / 2.f - width / 2.f;

    this->buttons[key] = new GUI::Button(x, y, width, height, &this->font, text, 50, sf::Color(212, 17, 17, 200), sf::Color::Red, sf::Color(20, 20, 20, 50), sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent);
}

void ConsoleMenu::addTextBox(const std::string key, float y) {
    float width = 300.f;
    float height = 50.f;
    float x = this->console.getPosition().x + this->console.getSize().x / 2.f - width / 2.f;

    this->text_boxes[key] = new GUI::TextBox(x, y, width, height, 20, 30, 48, 57, this->font, sf::Color::Black, sf::Color::White, sf::Color::Blue);
}

void ConsoleMenu::update(const sf::Vector2f &mouse_pos, const float &dt, sf::Event event, std::string &user_input) {
    for (auto &it : this->buttons) {
        it.second->update(mouse_pos);
    }

    for (auto &it : this->text_boxes) {
        it.second->update(mouse_pos, dt, event, user_input, this->text_box_active);
    }
}

void ConsoleMenu::render(sf::RenderTarget &target) {
    target.draw(this->background);
    target.draw(this->console);

    for (auto &it : this->buttons) {
        it.second->render(target);
    }

    for (auto &it : this->text_boxes) {
        it.second->render(target);
    }

    target.draw(this->title_text);
    target.draw(this->text_box_names);
}