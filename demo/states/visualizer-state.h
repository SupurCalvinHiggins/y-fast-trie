#pragma once

#include "state.h"

class VisualizerState : public State {
public:
    VisualizerState(sf::RenderWindow *window);
    virtual ~VisualizerState();

    // Functions
    void exitState();
    void updateKeyBinds(const float &dt);
    void update(const float &dt);
    void render(sf::RenderTarget *target = nullptr);
};