#pragma once

#include "state.h"
#include "../console-menu.h"
#include "../src/y-fast-trie/y-fast-trie.h"
#include <random>


class VisualizerState : public State {
private:
    sf::Image trie_image;
    sf::Image background_image;
    sf::Texture background_texture;
    sf::Sprite background;
    ConsoleMenu *console_menu;
    sf::Font font;

    short unsigned trie_type;
    YFastTrie<uint8_t> yfast_8;
    YFastTrie<uint16_t> yfast_16;
    YFastTrie<uint32_t> yfast_32;
    YFastTrie<uint64_t> yfast_64;
    std::string user_input;

    void initBackground();
    void initFonts();
    void initKeyBinds();
    void initConsoleMenu();
    
public:
    VisualizerState(sf::RenderWindow *window, std::map<std::string, int> *valid_keys, std::stack<State*> *states, unsigned short trie_type);
    virtual ~VisualizerState();

    std::string getStateID();
    
    // Functions
    void updateInput(const float &dt);
    void updateRandomInsert();
    void updateConsoleInput();
    void updateBackground();
    void update(const float &dt, sf::Event event);
    void render(sf::RenderTarget *target = nullptr);
};