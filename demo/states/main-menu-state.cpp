#include "main-menu-state.h"

void MainMenuState::initBackground() {
    this->background.setSize(sf::Vector2f(this->window->getSize().x, this->window->getSize().y));

    if (!this->background_texture.loadFromFile("resource/image/background/main-menu.png"))
        throw std::runtime_error("Could not load Main Menu background texture!");
    
    this->background.setTexture(&this->background_texture);
}

void MainMenuState::initFonts() {
    if (!this->title_font.loadFromFile("resource/font/Dosis-Medium.ttf"))
        throw std::runtime_error("Could not load Dosis-Medium.ttf (MainMenuState TITLE)");
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

void MainMenuState::initButtons() {
    this->buttons["ABOUT"] = new Button(320, 840, 250, 50, &this->title_font, "About", sf::Color(70,70,70,200), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200));
    
    this->buttons["EXIT"] = new Button(1350, 840, 250, 50, &this->title_font, "Quit", sf::Color(70,70,70,200), sf::Color::Red, sf::Color::Red);
}

MainMenuState::MainMenuState(sf::RenderWindow *window, std::map<std::string, int> *valid_keys, std::stack<State*> *states) : State(window, valid_keys, states) {
    this->initBackground();
    this->initFonts();
    this->initKeyBinds();
    this->initButtons();
}

MainMenuState::~MainMenuState() {
    for (auto it = this->buttons.begin(); it != this->buttons.end(); it++) {
        delete it->second;
    }
}

void MainMenuState::exitState() {
    std::cout << "Exiting MainMenuState" << std::endl;
}
 
void MainMenuState::updateInput(const float &dt) {
    this->setExit();
}

void MainMenuState::updateButtons() {
    // Update all MainMenuState buttons and handles appropriate button response
    for (auto &it : this->buttons) {
        it.second->update(this->mouse_pos_view);
    }

    if (this->buttons["ABOUT"]->isClicked()) {
        this->states->push(new VisualizerState(this->window, this->valid_keys, this->states));
    }

    if (this->buttons["EXIT"]->isClicked())
        this->exit = true;
}

void MainMenuState::update(const float &dt) {
    this->updateMousePositions();
    this->updateInput(dt);

    this->updateButtons();

    //system("clear");
    //std::cout << this->mouse_pos_view.x << " " << this->mouse_pos_view.y << std::endl;
}

void MainMenuState::renderButtons(sf::RenderTarget *target) {
    for (auto &it : this->buttons) {
        it.second->render(target);
    }
}

void MainMenuState::render(sf::RenderTarget *target) {
    if (!target) {
        target = this->window;
    }

    target->draw(this->background);

    this->renderButtons(target);

    //TODO: Remove this when no longer using (visualizes mouse position next to cursor)
    /*
    sf::Text mouse_text;
    mouse_text.setPosition(this->mouse_pos_view.x, this->mouse_pos_view.y - 30);
    mouse_text.setFont(this->title_font);
    mouse_text.setCharacterSize(20);
    mouse_text.setString(std::to_string(this->mouse_pos_view.x) + " " + std::to_string(this->mouse_pos_view.y));

    target->draw(mouse_text);
    */
}