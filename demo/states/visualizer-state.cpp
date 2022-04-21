#include "visualizer-state.h"

void VisualizerState::initFonts() {
    if (!this->font.loadFromFile("resource/font/Dosis-Medium.ttf"))
        throw std::runtime_error("Could not load Dosis-Medium.ttf (VisualizerState Font)");
}


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

void VisualizerState::initConsoleMenu() {
    this->console_menu = new ConsoleMenu(*this->window, this->font);

    this->console_menu->addButton("EXIT", 900.f, "Exit");
}

VisualizerState::VisualizerState(sf::RenderWindow *window, std::map<std::string, int> *valid_keys, std::stack<State*> *states) : State(window, valid_keys, states) {
    this->initKeyBinds();
    this->initFonts();
    this->initConsoleMenu();
}

VisualizerState::~VisualizerState() {
    delete this->console_menu;
}

void VisualizerState::updateInput(const float &dt) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->key_binds.at("CLOSE"))) && this->getKeyTimer()) {
        if (!this->is_in_console)
            this->inConsoleState();
        else
            this->outConsoleState();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->key_binds.at("ENTER_VALUE"))) && this->getKeyTimer())
        std::cout << "Enter key pressed" << std::endl;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->key_binds.at("DELETE_CHARACTER"))) && this->getKeyTimer())
        std::cout << "Backspace key pressed" << std::endl;
}

void VisualizerState::updateConsoleButtons() {
    if (this->console_menu->isButtonClicked("EXIT"))
        this->exitState();
}

void VisualizerState::update(const float &dt) {
    this->updateMousePositions();
    this->updateKeyTime(dt);
    this->updateInput(dt);
    
    if (this->is_in_console)
        this->console_menu->update(this->mouse_pos_view);
        this->updateConsoleButtons();
}

void VisualizerState::render(sf::RenderTarget *target) {
    if (!target) {
        target = this->window;
    }

    if (this->is_in_console) { // Render console
        this->console_menu->render(*target);
    }
}