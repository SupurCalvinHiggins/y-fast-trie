#pragma once

#include "state.h"
#include "../console-menu.h"
#include "../src/y-fast-trie/y-fast-trie.h"
#include <random>


class VisualizerState : public State {
private:
    sf::Texture background_texture;
    sf::RectangleShape background;
    ConsoleMenu *console_menu;
    sf::Font font;
    
    YFastTrie<uint8_t> yfast;

    void initFonts();
    void initKeyBinds();
    void initConsoleMenu();
    
public:
    VisualizerState(sf::RenderWindow *window, std::map<std::string, int> *valid_keys, std::stack<State*> *states);
    virtual ~VisualizerState();

    std::string getStateID();
    
    // Functions
    void updateInput(const float &dt);
    void updateConsoleButtons();
    void writeFile(const std::string to_dot);
    void updateBackground();
    void update(const float &dt);
    void render(sf::RenderTarget *target = nullptr);
};