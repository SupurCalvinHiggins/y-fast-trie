#pragma once

#include "about-state.h"

class MainMenuState : public State {
private:
    // Variables
    sf::Texture background_texture;
    sf::RectangleShape background;
    sf::Font button_font;

    std::map<std::string, GUI::Button*> buttons;

    GUI::DropDownList *trie_types_list;

    void initBackground();
    void initFonts();
    void initKeyBinds();
    void initButtons();
    
public:
    MainMenuState(sf::RenderWindow *window, std::map<std::string, int> *valid_keys, std::stack<State*> *states);
    virtual ~MainMenuState();

    // Functions
    void updateInput(const float &dt);
    void updateButtons();
    void update(const float &dt);
    void renderButtons(sf::RenderTarget &target);
    void render(sf::RenderTarget *target = nullptr);
};