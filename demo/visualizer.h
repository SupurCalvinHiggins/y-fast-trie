#pragma once

#include "states/main-menu-state.h"
#include "states/visualizer-state.h"

class Visualizer {
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

    bool isVisualizerState();
    void updateVisualizerBackground();

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

static Visualizer visualizer;

void UPDATE_GUI() {
    visualizer.updateVisualizerBackground();
}


// Initializers
void Visualizer::initWindow() {
    // Initialize window and set its properties
    std::ifstream file("config/window.ini");
    
    this->videoModes = sf::VideoMode::getFullscreenModes();

    std::string title = "None";
    sf::VideoMode window_size = sf::VideoMode::getDesktopMode();

    unsigned framerate_limit = 120;
    unsigned antialiasing_level = 0;

    if (file.is_open()) {
        std::getline(file, title);
        file >> window_size.width >> window_size.height;
        file >> framerate_limit;
        file >> antialiasing_level;
    }

    file.close();

    this->window_settings.antialiasingLevel = antialiasing_level;

    this->window = new sf::RenderWindow(window_size, title, sf::Style::Default, window_settings);
    this->window->setFramerateLimit(framerate_limit);
    }

void Visualizer::initKeys() {
    // Initialize valid keys
    //TODO: Add number support to insert ints

    std::ifstream file("config/valid-keys.ini");

    if (file.is_open()) {
        std::string key_name;
        int key_code;

        while (file >> key_name >> key_code) {
            this->valid_keys[key_name] = key_code;
        }
    }

    file.close();
}

void Visualizer::initStates() {
    // Initialize states
    this->states.push(new MainMenuState(this->window, &this->valid_keys, &this->states));
}

// Constructors/Destructors
Visualizer::Visualizer() {
    this->initWindow();
    this->initKeys();
    this->initStates();
}

Visualizer::~Visualizer() {
    delete this->window;

    while(!this->states.empty()) {
        delete this->states.top();
        this->states.pop();
    }

}

bool Visualizer::isVisualizerState() {
    if (states.top()->getStateID() == "VISUALIZER_STATE") {
        return true;
    }
    return false;
}

void Visualizer::updateVisualizerBackground() {
    if (this->isVisualizerState()) {
        VisualizerState *visualizer_state = (VisualizerState*)states.top();
        visualizer_state->updateBackground();
    }
}

// Functions
void Visualizer::updateDt() {
    // Update delta time with the time it took to update and render the last frame.
    this->dt = this->dt_clock.restart().asSeconds();
    
    //system("clear");
    //std::cout << "dt: " << this->dt << std::endl;
    }

void Visualizer::updateEvents() {
    while (this->window->pollEvent(this->event)) {
        if (this->event.type == sf::Event::Closed)
            this->window->close();
    }
}

void Visualizer::update() {
    this->updateEvents();

    if (!this->states.empty()) {
        this->states.top()->update(this->dt);

        if (this->states.top()->getExit()) {
            this->states.top()->exitState();
            delete this->states.top();
            this->states.pop();
        }
    }
    else {
        this->endApplication();
        this->window->close();
    }

} 

void Visualizer::render() {
   this->window->clear();

    if (!this->states.empty())
        this->states.top()->render();

   this->window->display();
}

void Visualizer::run() {
    while (this->window->isOpen()) {

        this->updateDt();
        this->update();
        this->render();
    }
}

void Visualizer::endApplication() {
    std::cout << "Ending Application!" << std::endl;
    this->window->close();
}


/*
// TODO Make it work with dot converter function directly rather than generating before
// TODO Function to find PNG pixel dimensions

    system("dot -Tpng resource/test.gv -o resource/output.png");

    sf::Texture texture;
    if (!texture.loadFromFile("resource/output.png"))
    {
        std::cout << "File not found... terminating!" << std::endl;
        return 0;
    }
*/