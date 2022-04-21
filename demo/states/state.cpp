#include "state.h"

State::State(sf::RenderWindow *window, std::map<std::string, int> *valid_keys, std::stack<State*> *states) {
    this->window = window;
    this->valid_keys = valid_keys;
    this->states = states;
    this->exit = false;
    this->is_in_console = false;
    this->key_timer = 0.f;
    this->key_timer_max = 0.25f;
}

State::~State() {
}

const bool & State::getExit() const {
    return this->exit;
}

const bool State::getKeyTimer() {
    if (this->key_timer >= this->key_timer_max) {
        this->key_timer = 0.f;
        return true;
    }
    
    return false;
}

void State::exitState() {
    this->exit = true;
}

void State::inConsoleState() {
    this->is_in_console = true;
}

void State::outConsoleState() {
    this->is_in_console = false;
}

void State::updateMousePositions() {
    this->mouse_pos_screen = sf::Mouse::getPosition(*this->window);
    this->mouse_pos_window = sf::Mouse::getPosition(*this->window);
    this->mouse_pos_view = this->window->mapPixelToCoords(this->mouse_pos_window);
}

void State::updateKeyTime(const float &dt) {
    if (this->key_timer < this->key_timer_max) {
        this->key_timer += dt;
    }
}