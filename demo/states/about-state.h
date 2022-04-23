#pragma once

#include "visualizer-state.h"
#include "../tools/gui.h"

class AboutState : public State {
private:
    // Variables
    sf::Texture background_texture;
    sf::RectangleShape background;
    sf::Font button_font;

    std::map<std::string, GUI::Button*> buttons;

    void initBackground();
    void initFonts();
    void initKeyBinds();
    void initButtons();
    
public:
    AboutState(sf::RenderWindow *window, std::map<std::string, int> *valid_keys, std::stack<State*> *states);
    virtual ~AboutState();

    // Functions
    std::string getStateID();
    void updateInput(const float &dt);
    void updateButtons();
    void update(const float &dt);
    void renderButtons(sf::RenderTarget &target);
    void render(sf::RenderTarget *target = nullptr);
};