/**
 * @file gui.h
 * @author Robert Oganesian (roganesian@uri.edu)
 * @brief GUI controller.
 * @version 1.0
 * @date 2022-04-25
 * 
 */

#pragma once

#include "states/main-menu.h"
#include "states/visualizer.h"

class Visualizer {
private:
    // Required SFML variables.
    sf::RenderWindow *window;
    std::vector<sf::VideoMode> videoModes;
    sf::ContextSettings window_settings;
    sf::Event event;

    // Map of every valid key.
    std::map<std::string, int> valid_keys;

private:
    // Clock to measure delta time.
    sf::Clock dt_clock;

    // Delta time (dt) will measure the time between updating and rendering the last frame to the current frame.
    float dt;

    // Stack that will contain every state that is open and has not been exited.
    std::stack<State*> states;

private:
    /**
     * @brief Initialize the window settings.
     * 
     */
    void initWindow() {
        std::ifstream file("demo/config/window.ini");
        
        this->videoModes = sf::VideoMode::getFullscreenModes();

        std::string title = "";
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

    /**
     * @brief Initialize valid SFML accepted keys.
     * 
     */
    void initKeys() {
        std::ifstream file("demo/config/valid-keys.ini");

        if (file.is_open()) {
            std::string key_name;
            int key_code;

            // Organizes all valid keys from valid-keys.ini into a map.
            while (file >> key_name >> key_code) {
                this->valid_keys[key_name] = key_code;
            }
        }

        file.close();
    }

    /**
     * @brief Initialize the states.
     * 
     * @details This function will push the main menu to the states stack. The main menu will always be at the bottom of the stack.
     * 
     */
    void initStates() {
        this->states.push(new MainMenuState(this->window, &this->valid_keys, &this->states));
    }

    /**
     * @brief Check if the current state is the visualizer state.
     * 
     * @return if the current state is the visualizer state.
     * 
     */
    const bool isVisualizerState() const {
        return states.top()->getStateID() == 2;
    }

public:
    /**
     * @brief Construct a new Visualizer/GUI object.
     * 
     */
    Visualizer() {
        this->initWindow();
        this->initKeys();
        this->initStates();
    }

    /**
     * @brief Destroy the Visualizer/GUI object.
     * 
     */
    ~Visualizer() {
        delete this->window;

        while(!this->states.empty()) {
            delete this->states.top();
            this->states.pop();
        }
    }

public:
    /**
     * @brief Updates visualizer state background.
     * 
     * @details Updates the background of the visualizer state if the current state is the visualizer state. The background update will update the y-fast trie that is being visualized.
     * 
     */
    void updateVisualizerBackground() {
        if (this->isVisualizerState()) {
            // Cast the current state to a VisualizerState if state is visualizer state.
            VisualizerState *visualizer_state = (VisualizerState*)states.top();
            visualizer_state->updateBackground();
        }
    }

    /**
     * @brief Updates the dt variable.
     * 
     */
    void updateDt() {
        this->dt = this->dt_clock.restart().asSeconds();
    }

    /**
     * @brief Updates the window's events.
     * 
     */
    void updateEvents() {
        while (this->window->pollEvent(this->event)) {
            if (this->event.type == sf::Event::Closed)
                this->window->close();
        }
    }

    /**
     * @brief Updates the current state.
     * 
     */
    void update() {
        this->updateEvents();

        // While the states stack is not empty, update the current state.
        if (!this->states.empty()) {
            this->states.top()->update(this->dt, this->event);

            // If the current state is being exited, pop it from the stack.
            if (this->states.top()->getExit()) {
                this->states.top()->exitState();
                delete this->states.top();
                this->states.pop();
            }
        }

        // If the states stack is empty, close the window which will end the program.
        else
            this->window->close();
    }
    
    /**
     * @brief Renders the current state.
     * 
     */
    void render() {
        this->window->clear();

        // While the states stack is not empty, render the current state.
        if (!this->states.empty())
            this->states.top()->render();
        
        this->window->display();
    }
    
    /**
     * @brief Runs the visualizer.
     * 
     * @details This function will run the visualizer by calling all core functions. The visualizer will run until the window is closed.
     * 
     */
    void run() {
        // While the program is running, make calls to the core functions.
        while (this->window->isOpen()) {
            this->updateDt();
            this->update();
            this->render();
        }
    }
};

// Telling the compiler that this is the main function.
extern Visualizer gui;

/**
 * @brief Calls the updateBackground() function.
 * 
 * @details This function is a wrapper to call the updateBackground() function of the visualizer state, which will update the y-fast trie that is being visualized.
 */
void UPDATE_GUI() {
    gui.updateVisualizerBackground();
    gui.render();
}