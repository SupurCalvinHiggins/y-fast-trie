#pragma once

#include "states/main-menu-state.h"

class Visualizer{
private:
    // Variables
    sf::RenderWindow *window;
    sf::Event event;
    std::vector<sf::VideoMode> videoModes;
    sf::ContextSettings window_settings;

    sf::Clock dt_clock;
    float dt;

    // Abstract class cannot be created as an object
    std::stack<State*> states;

    std::map<std::string, int> valid_keys;

    // Initializers
    void initWindow();
    void initKeys();
    void initStates();

public:
    // Constructors/Destructors
    Visualizer();
    ~Visualizer();

    // Functions //

    // Update
    void updateDt();
    void updateEvents();
    void update();
    
    // Render
    void render();
    
    // Core
    void run();
    void endApplication();

};