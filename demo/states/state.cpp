#include "state.h"

State::State(sf::RenderWindow *window) {
    this->window = window;
    this->exit = false;
}

State::~State() {
}

const bool & State::getExit() const {
    return this->exit;
}

void State::setExit() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        this->exit = true;
    }
}