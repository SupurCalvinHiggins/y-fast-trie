#pragma once

#include "state.h"
#include "../console-menu.h"

class VisualizerState : public State {
private:
    ConsoleMenu *console_menu;
    sf::Font font;

    void initFonts();
    void initKeyBinds();
    void initConsoleMenu();
    
public:
    VisualizerState(sf::RenderWindow *window, std::map<std::string, int> *valid_keys, std::stack<State*> *states);
    virtual ~VisualizerState();

    // Functions
    void updateInput(const float &dt);
    void updateConsoleButtons();
    void update(const float &dt);
    void render(sf::RenderTarget *target = nullptr);
};