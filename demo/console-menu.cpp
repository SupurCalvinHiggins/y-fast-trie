#include "console-menu.h"

ConsoleMenu::ConsoleMenu(sf::RenderWindow &window, sf::Font &font) : font(font) {
    this->background.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    this->background.setFillColor(sf::Color(20, 20, 20, 100));

    this->console.setSize(sf::Vector2f(window.getSize().x / 4.f, window.getSize().y - 100.f));
    this->console.setFillColor(sf::Color(20, 20, 20, 200));
    this->console.setPosition(window.getSize().x / 2.f - this->console.getSize().x / 2.f, 30.f);

    this->console_text.setFont(font);
    this->console_text.setFillColor(sf::Color(255, 255, 255, 200));
    this->console_text.setCharacterSize(60);
    this->console_text.setString("Tree Editor");
    this->console_text.setPosition(this->console.getPosition().x + this->console.getSize().x / 2.f - this->console_text.getGlobalBounds().width / 2.f, this->console.getPosition().y + 15.f);
}

ConsoleMenu::~ConsoleMenu() {
    for (auto it = this->buttons.begin(); it != this->buttons.end(); it++) {
        delete it->second;
    }
}

std::map<std::string, GUI::Button*>& ConsoleMenu::getButtons() {
    return this->buttons;
}

const bool ConsoleMenu::isButtonClicked(const std::string key) {
    return this->buttons.at(key)->isClicked();
}

void ConsoleMenu::addButton(const std::string key, float y, const std::string text) {
    float width = 150.f;
    float height = 50.f;
    float x = this->console.getPosition().x + this->console.getSize().x / 2.f - width / 2.f;

    this->buttons[key] = new GUI::Button(x, y, width, height, &this->font, text, 50, sf::Color(70,70,70,200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50), sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent);
}

void ConsoleMenu::update(const sf::Vector2f &mouse_pos) {
    for (auto &it : this->buttons) {
        it.second->update(mouse_pos);
    }
}

void ConsoleMenu::render(sf::RenderTarget &target) {
    target.draw(this->background);
    target.draw(this->console);

    for (auto &it : this->buttons) {
        it.second->render(target);
    }

    target.draw(this->console_text);
}