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
    void initGUI();
    
public:
    MainMenuState(sf::RenderWindow *window, std::map<std::string, int> *valid_keys, std::stack<State*> *states);
    virtual ~MainMenuState();

    // Functions
    std::string getStateID();
    void updateInput(const float &dt);
    void updateGUI(const float &dt);
    void update(const float &dt, sf::Event event);
    void renderGUI(sf::RenderTarget &target);
    void render(sf::RenderTarget *target = nullptr);
};