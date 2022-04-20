#include "visualizer-state.h"

void VisualizerState::initKeyBinds() {
    std::ifstream file("config/visualizer-state-keys.ini");

    if (file.is_open()) {
        std::string key_name;
        std::string valid_key;

        while (file >> key_name >> valid_key) {
            this->key_binds[key_name] = this->valid_keys->at(valid_key);
        }
    }

    file.close();
}

VisualizerState::VisualizerState(sf::RenderWindow *window, std::map<std::string, int> *valid_keys, std::stack<State*> *states) : State(window, valid_keys, states) {
    this->initKeyBinds();
}

VisualizerState::~VisualizerState() {
}

void VisualizerState::exitState() {
    std::cout << "Exiting VisualizerState" << std::endl;
}
 
void VisualizerState::updateInput(const float &dt) {
    this->setExit();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->key_binds.at("CLOSE"))))
        std::cout << "Esc key pressed" << std::endl;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->key_binds.at("ENTER_VALUE"))))
        std::cout << "Enter key pressed" << std::endl;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->key_binds.at("DELETE_CHARACTER"))))
        std::cout << "Backspace key pressed" << std::endl;
}

void VisualizerState::update(const float &dt) {
    this->updateMousePositions();
    this->updateInput(dt);
}

void VisualizerState::render(sf::RenderTarget *target) {
    if (!target) {
        target = this->window;
    }
}