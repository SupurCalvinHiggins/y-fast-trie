#include "visualizer-state.h"

VisualizerState::VisualizerState(sf::RenderWindow *window) : State(window) {
}

VisualizerState::~VisualizerState() {
}

void VisualizerState::exitState() {
    std::cout << "Exiting VisualizerState" << std::endl;
}
 
void VisualizerState::updateKeyBinds(const float &dt) {
    this->setExit();
}

void VisualizerState::update(const float &dt) {
    this->updateKeyBinds(dt);
}

void VisualizerState::render(sf::RenderTarget *target) {
}