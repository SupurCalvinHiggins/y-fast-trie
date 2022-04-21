#include "about-state.h"

void AboutState::initBackground() {
    this->background.setSize(sf::Vector2f(this->window->getSize().x, this->window->getSize().y));
    if (!this->background_texture.loadFromFile("resource/image/background/about-state.png"))
        throw std::runtime_error("Could not load About State background texture!");
    
    this->background.setTexture(&this->background_texture);
}

void AboutState::initFonts() {
    if (!this->button_font.loadFromFile("resource/font/Dosis-Medium.ttf"))
        throw std::runtime_error("Could not load Dosis-Medium.ttf (AboutState Font)");
}

void AboutState::initKeyBinds() {
    std::ifstream file("config/about-state-keys.ini");

    if (file.is_open()) {
        std::string key_name;
        std::string valid_key;

        while (file >> key_name >> valid_key) {
            this->key_binds[key_name] = this->valid_keys->at(valid_key);
        }
    }

    file.close();
}

void AboutState::initButtons() {
    // TODO: Make this button the GitHub logo
    this->buttons["GITHUB"] = new GUI::Button(500.f, 475.f, 150.f, 50.f, &this->button_font, "Github", 50, sf::Color(70,70,70,200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50), sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent);
    
    this->buttons["BACK"] = new GUI::Button(1412.f, 800.f, 160.f, 50.f, &this->button_font, "Go Back", 50, sf::Color(70,70,70,200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50), sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent);
}

AboutState::AboutState(sf::RenderWindow *window, std::map<std::string, int> *valid_keys, std::stack<State*> *states) : State(window, valid_keys, states) {
    this->initBackground();
    this->initFonts();
    this->initKeyBinds();
    this->initButtons();
}

AboutState::~AboutState() {
    for (auto it = this->buttons.begin(); it != this->buttons.end(); it++) {
        delete it->second;
    }
}
 
std::string AboutState::getStateID() {
    return "ABOUT_STATE";
}

void AboutState::updateInput(const float &dt) {
}

void AboutState::updateButtons() {
    // Update all AboutState buttons and handles appropriate button response
    for (auto &it : this->buttons) {
        it.second->update(this->mouse_pos_view);
    }

    if (this->buttons["GITHUB"]->isClicked())
        system("open https://github.com/SupurCalvinHiggins/y-fast-trie");
    if (this->buttons["BACK"]->isClicked())
        this->exitState();
}

void AboutState::update(const float &dt) {
    this->updateMousePositions();
    this->updateInput(dt);

    this->updateButtons();

    //system("clear");
    //std::cout << this->mouse_pos_view.x << " " << this->mouse_pos_view.y << std::endl;
}

void AboutState::renderButtons(sf::RenderTarget &target) {
    for (auto &it : this->buttons) {
        it.second->render(target);
    }
}

void AboutState::render(sf::RenderTarget *target) {
    if (!target) {
        target = this->window;
    }

    target->draw(this->background);

    this->renderButtons(*target);

    //TODO: Remove this when no longer using (visualizes mouse position next to cursor)

    sf::Text mouse_text;
    mouse_text.setPosition(this->mouse_pos_view.x, this->mouse_pos_view.y - 30);
    mouse_text.setFont(this->button_font);
    mouse_text.setCharacterSize(20);
    mouse_text.setString(std::to_string(this->mouse_pos_view.x) + " " + std::to_string(this->mouse_pos_view.y));

    target->draw(mouse_text);
}