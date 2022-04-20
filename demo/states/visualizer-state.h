#pragma once

#include "state.h"

class VisualizerState : public State {
private:
    void initKeyBinds();
    
public:
    VisualizerState(sf::RenderWindow *window, std::map<std::string, int> *valid_keys, std::stack<State*> *states);
    virtual ~VisualizerState();

    // Functions
    void exitState();
    void updateInput(const float &dt);
    void update(const float &dt);
    void render(sf::RenderTarget *target = nullptr);
};