#pragma once

#include "states/visualizer-state.h"

class Visualizer{
private:
    // Variables
    sf::RenderWindow *window;
    sf::Event event;

    sf::Clock dt_clock;
    float dt;

    // Abstract class cannot be created as an object
    std::stack<State*> states;

    // Initializers
    void initWindow();
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