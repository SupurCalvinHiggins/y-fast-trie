#include "main-menu-state.h"

void MainMenuState::initBackground() {
    this->background.setSize(sf::Vector2f(this->window->getSize().x, this->window->getSize().y));

    if (!this->background_texture.loadFromFile("resource/image/background/main-menu.png"))
        throw std::runtime_error("Could not load Main Menu background texture!");
    
    this->background.setTexture(&this->background_texture);
}

void MainMenuState::initFonts() {
    if (!this->button_font.loadFromFile("resource/font/Dosis-Medium.ttf"))
        throw std::runtime_error("Could not load Dosis-Medium.ttf (MainMenuState Font)");
}

void MainMenuState::initKeyBinds() {
    std::ifstream file("config/main-menu-state-keys.ini");

    if (file.is_open()) {
        std::string key_name;
        std::string valid_key;

        while (file >> key_name >> valid_key) {
            this->key_binds[key_name] = this->valid_keys->at(valid_key);
        }
    }

    file.close();
}

void MainMenuState::initGUI() {
    this->buttons["ABOUT"] = new GUI::Button(320.f, 800.f, 150.f, 50.f, &this->button_font, "About", 50, sf::Color(70,70,70,200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50), sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent);
    this->buttons["EXIT"] = new GUI::Button(1450.f, 800.f, 150.f, 50.f, &this->button_font, "Quit", 50, sf::Color(70,70,70,200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50), sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent);
    this->buttons["START"] = new GUI::Button(860.f, 650.f, 150.f, 50.f, &this->button_font, "Generate", 50, sf::Color(70,70,70,200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50), sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent);

    std::vector<std::string> drop_down_button_names {"uint8_t", "uint_16t", "uint32_t", "uint64_t"};
    this->trie_types_list = new GUI::DropDownList(860.f, 400.f, 150.f, 50.f, this->button_font, drop_down_button_names);
}

MainMenuState::MainMenuState(sf::RenderWindow *window, std::map<std::string, int> *valid_keys, std::stack<State*> *states) : State(window, valid_keys, states) {
    this->initBackground();
    this->initFonts();
    this->initKeyBinds();
    this->initGUI();
}

MainMenuState::~MainMenuState() {
    for (auto it = this->buttons.begin(); it != this->buttons.end(); it++) {
        delete it->second;
    }
    
    delete this->trie_types_list;
}

std::string MainMenuState::getStateID() {
    return "MAIN_MENU";
}
 
void MainMenuState::updateInput(const float &dt) {
}

void MainMenuState::updateGUI(const float &dt) {
    // Update all MainMenuState buttons and handles appropriate button response
    for (auto &it : this->buttons) {
        it.second->update(this->mouse_pos_view);
    }

    if (this->buttons["ABOUT"]->isClicked() && this->getKeyTimer())
        this->states->push(new AboutState(this->window, this->valid_keys, this->states));
    if (this->buttons["EXIT"]->isClicked() && this->getKeyTimer())
        this->exitState();
    if (this->buttons["START"]->isClicked() && this->getKeyTimer())
        this->states->push(new VisualizerState(this->window, this->valid_keys, this->states, this->trie_types_list->getSelectionID()));

    this->trie_types_list->update(this->mouse_pos_view, dt);
}

void MainMenuState::update(const float &dt, sf::Event event) {
    this->updateMousePositions();
    this->updateKeyTime(dt);
    this->updateInput(dt);

    this->updateGUI(dt);
}

void MainMenuState::renderGUI(sf::RenderTarget &target) {
    for (auto &it : this->buttons) {
        it.second->render(target);
    }

    this->trie_types_list->render(target);
}

void MainMenuState::render(sf::RenderTarget *target) {
    if (!target) {
        target = this->window;
    }

    target->draw(this->background);

    this->renderGUI(*target);

    //TODO: Remove this when no longer using (visualizes mouse position next to cursor)

    sf::Text mouse_text;
    mouse_text.setPosition(this->mouse_pos_view.x, this->mouse_pos_view.y - 30);
    mouse_text.setFont(this->button_font);
    mouse_text.setCharacterSize(20);
    mouse_text.setString(std::to_string(this->mouse_pos_view.x) + " " + std::to_string(this->mouse_pos_view.y));

    target->draw(mouse_text);

}