#include "state.h"

State::State(sf::RenderWindow *window, std::map<std::string, int> *valid_keys, std::stack<State*> *states) {
    this->window = window;
    this->valid_keys = valid_keys;
    this->states = states;
    this->exit = false;
}

State::~State() {
}

const bool & State::getExit() const {
    return this->exit;
}

void State::setExit() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->key_binds.at("CLOSE"))))
        this->exit = true;
}

void State::updateMousePositions() {
    this->mouse_pos_screen = sf::Mouse::getPosition(*this->window);
    this->mouse_pos_window = sf::Mouse::getPosition(*this->window);
    this->mouse_pos_view = this->window->mapPixelToCoords(this->mouse_pos_window);
}