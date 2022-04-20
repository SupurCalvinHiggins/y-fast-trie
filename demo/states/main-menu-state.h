#pragma once

#include "visualizer-state.h"
#include "../tools/button.h"

class MainMenuState : public State {
private:
    // Variables
    sf::Texture background_texture;
    sf::RectangleShape background;
    sf::Font title_font;

    std::map<std::string, Button*> buttons;

    void initBackground();
    void initFonts();
    void initKeyBinds();
    void initButtons();
    
public:
    MainMenuState(sf::RenderWindow *window, std::map<std::string, int> *valid_keys, std::stack<State*> *states);
    virtual ~MainMenuState();

    // Functions
    void exitState();
    void updateInput(const float &dt);
    void updateButtons();
    void update(const float &dt);
    void renderButtons(sf::RenderTarget *target = nullptr);
    void render(sf::RenderTarget *target = nullptr);
};