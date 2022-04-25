/**
 * @file gui.h
 * @author Robert Oganesian (roganesian@uri.edu)
 * @brief Parent class for all states.
 * @version 1.0
 * @date 2022-04-25
 * 
 */

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stack>
#include <map>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>


class State {
protected:
    // Required SFML variable.
    sf::RenderWindow *window;

    // Pointer to the map of valid SFML keys from gui.h.
    std::map<std::string, int> *valid_keys;

    // Map containing all buttons used in this state.
    std::map<std::string, int> key_binds;

    /**
	 * @brief Initialize keybinds for a specific state.
	 * 
	 */
    virtual void initKeyBinds() = 0;

protected:
    // Pointer to the stack of states from gui.h.
    std::stack<State*> *states;

    // Boolean to check if a state wants to be closed.
    bool exit;

    // Limits user input by placing time interval in-between input.
    float key_timer;
    float key_timer_max;

    // Necessary SFML variables to track mouse position for GUI tool input.
    sf::Vector2i mouse_pos_screen;
    sf::Vector2i mouse_pos_window;
    sf::Vector2f mouse_pos_view;
    

public:
    /**
	 * @brief Construct a new State object.
	 * 
     * @param window Pointer to the GUI window.
     * @param valid_keys Pointer to the map of valid SFML keys.
     * @param states Pointer to the stack of states.
     * 
	 */
    State(sf::RenderWindow *window, std::map<std::string, int> *valid_keys, std::stack<State*> *states) {
        this->window = window;
        this->valid_keys = valid_keys;
        this->states = states;
        this->exit = false;
        this->key_timer = 0.f;
        this->key_timer_max = 0.25f;
    }

    /**
	 * @brief Destroy the State object. Base destructor.
	 * 
	 */
    virtual ~State() {}

    /**
	 * @brief Gets exit status of a state.
	 * 
     * @return if a state wants to be closed or not.
     * 
	 */
    const bool &getExit() const {
        return this->exit;
    }

    /**
	 * @brief Sets exit status of a state.
	 * 
	 */
    void exitState() {
        this->exit = true;
    }

    /**
     * @brief Get the ID of a child state.
     * 
     * @details 0 = MainMenuState, 1 = AboutState, 2 = VisualizerState.
     * @return a value representing the ID of the current state.
     * 
     */
    virtual const unsigned short getStateID() const = 0;

public:
    /**
     * @brief Check if enough time has passed since the last user input.
     * 
     * @return if enough time has passed since the last key user input.
     * 
     */
    const bool getKeyTimer() {
        if (this->key_timer >= this->key_timer_max) {
            // Reset the timer.
            this->key_timer = 0.f;
            return true;
        }
        
        return false;
    }
    
    /**
     * @brief Update position of mouse.
     * 
     */
    virtual void updateMousePositions() {
        this->mouse_pos_screen = sf::Mouse::getPosition(*this->window);
        this->mouse_pos_window = sf::Mouse::getPosition(*this->window);
        this->mouse_pos_view = this->window->mapPixelToCoords(this->mouse_pos_window);
    }

    /**
     * @brief Updates the key timer.
     * 
     * @param dt Reference to the delta time variable from gui.h.
     */
    virtual void updateKeyTime(const float &dt) {
        if (this->key_timer < this->key_timer_max)
            this->key_timer += dt;
    }

    /**
     * @brief Update any input from the user within a state.
     * 
     * @param dt Reference to the delta time variable from gui.h.
     * 
     */
    virtual void updateInput(const float &dt) = 0;

    /**
     * @brief Update any logic within a state.
     * 
     * @param dt Reference to the delta time variable from gui.h.
     * @param event Holds the current event from gui.h.
     * 
     */
    virtual void update(const float &dt, sf::Event event) = 0;

    /**
     * @brief Render any graphics within a state.
     * 
     * @param target Base SFML RenderTarget.
     * 
     */
    virtual void render(sf::RenderTarget *target = nullptr) = 0;
};