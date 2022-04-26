/**
 * @file about.h
 * @author Robert Oganesian (roganesian@uri.edu)
 * @brief About state of the GUI (state 1).
 * @version 1.0
 * @date 2022-04-26
 * 
 */

#pragma once

#include "visualizer.h"
#include "../tools/gui-tools.h"

class AboutState : public State {
private:
    // Required SFML variables
    sf::Image background_image;
    sf::Texture background_texture;
    sf::Sprite background;
    sf::Font button_font;

    // Map containing all buttons used in this state
    std::map<std::string, GUI::Button*> buttons;

private:
    /**
     * @brief Initialize the background.
     * 
     */
    void initBackground() {
        if (!this->background_image.loadFromFile("demo/resource/image/background/about.png"))
            throw std::runtime_error("Could not load AboutState background texture!");
        
        this->background_texture.loadFromImage(this->background_image);
        this->background.setTexture(this->background_texture);
    }

    /**
     * @brief Initialize the fonts.
     * 
     */
    void initFonts() {
        if (!this->button_font.loadFromFile("demo/resource/font/Dosis-Medium.ttf"))
            throw std::runtime_error("Could not load Dosis-Medium.ttf (AboutState Font)");
    }

    /**
     * @brief Initialize the key binds.
     * 
     */
    void initKeyBinds() {
        std::ifstream file("demo/config/about-state-keys.ini");

        if (file.is_open()) {
            std::string key_name;
            std::string valid_key;

            while (file >> key_name >> valid_key) {
                // Finds the key_value in the valid keys map and assigns it to the key_binds map.
                this->key_binds[key_name] = this->valid_keys->at(valid_key);
            }
        }

        file.close();
    }

    /**
     * @brief Initialize GUI tools: buttons.
     * 
     */
    void initButtons() {
        this->buttons["GITHUB"] = new GUI::Button(500.f, 475.f, 150.f, 50.f, &this->button_font, "Github", 50, sf::Color(37, 197, 230, 200), sf::Color::Cyan, sf::Color(20, 20, 20, 50), sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent);
        
        this->buttons["BACK"] = new GUI::Button(1412.f, 800.f, 160.f, 50.f, &this->button_font, "Go Back", 50, sf::Color(212, 17, 17), sf::Color::Red, sf::Color(20, 20, 20, 50), sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent);
    }
    
public:
     /**
     * @brief Construct a new AbotuState object.
     * 
     * @param window Pointer to the GUI window.
     * @param valid_keys Pointer to the map of valid SFML keys.
     * @param states Pointer to the stack of states.
     * 
     */
    AboutState(sf::RenderWindow *window, std::map<std::string, int> *valid_keys, std::stack<State*> *states) : State(window, valid_keys, states) {
    this->initBackground();
    this->initFonts();
    this->initKeyBinds();
    this->initButtons();
}

    virtual ~AboutState() {
        for (auto it = this->buttons.begin(); it != this->buttons.end(); it++)
            delete it->second;
    }
    /**
     * @brief Get the ID of this state.
     * 
     * @return unsigned short representing the ID of this state (1).
     */
    const unsigned short getStateID() const {
        return 1;
    }

public:
    /**
     * @brief Updates any direct user input.
     * 
     * @param dt Reference to the delta time variable from gui.h.
     * 
     */
    void updateInput(const float &dt) {}

    /**
     * @brief Updates the GUI tools: buttons.
     * 
     */
    void updateButtons() {
        // Update all AboutState buttons and handles appropriate button response
        for (auto &it : this->buttons)
            it.second->update(this->mouse_pos_view);

        if (this->buttons["GITHUB"]->isClicked() && this->getKeyTimer()) {
            #ifdef __APPLE__ || __MACH__
                system("open https://github.com/SupurCalvinHiggins/y-fast-trie");
            #elif __linux__
                system("xdg-open https://github.com/SupurCalvinHiggins/y-fast-trie");
            #elif _WIN32
                system("start https://github.com/SupurCalvinHiggins/y-fast-trie");
            #endif
        }
        if (this->buttons["BACK"]->isClicked() && this->getKeyTimer())
            this->exitState();
    }

    /**
     * @brief Update any logic within this state.
     * 
     * @param dt Reference to the delta time variable from gui.h.
     * @param event Holds the current event from gui.h.
     * 
     */
    void update(const float &dt, sf::Event &event) {
        this->updateMousePositions();
        this->updateKeyTime(dt);
        this->updateInput(dt);

        this->updateButtons();
    }

    /**
     * @brief Render the GUI tools: buttons.
     * 
     * @param target Reference to the base SFML RenderTarget.
     */
    void renderButtons(sf::RenderTarget &target) {
        for (auto &it : this->buttons)
            it.second->render(target);
    }

    /**
     * @brief Handles all AboutState rendering.
     * 
     * @param target Pointer to the base SFML RenderTarget.
     */
    void render(sf::RenderTarget *target = nullptr) {
        if (!target)
            target = this->window;

        target->draw(this->background);

        this->renderButtons(*target);
    }
};
