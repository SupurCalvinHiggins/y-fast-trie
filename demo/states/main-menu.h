/**
 * @file main-menu.h
 * @author Robert Oganesian (roganesian@uri.edu)
 * @brief Main menu state (state 0).
 * @version 1.0
 * @date 2022-04-26
 * 
 */

#pragma once

#include "about.h"


class MainMenuState : public State {
private:
    // Required SFML variables.
    sf::Image background_image;
    sf::Texture background_texture;
    sf::Sprite background;
    sf::Font button_font;

private:
    // Map of all buttons used in this state.
    std::map<std::string, GUI::Button*> buttons;

    // Drop-down list to select the y-fast trie type.
    GUI::DropDownList *trie_types_list;

private:
    /**
     * @brief Initialize the background.
     * 
     */
    void initBackground() {
        if (!this->background_image.loadFromFile("demo/resource/image/background/main-menu.png"))
            throw std::runtime_error("Could not load Main Menu background texture!");
        
        this->background_texture.loadFromImage(this->background_image);
        this->background.setTexture(this->background_texture);
    }

    /**
     * @brief Initialize the fonts.
     * 
     */
    void initFonts() {
        if (!this->button_font.loadFromFile("demo/resource/font/Dosis-Medium.ttf"))
            throw std::runtime_error("Could not load Dosis-Medium.ttf (MainMenuState Font)");
    }

    /**
     * @brief Initialize the key binds.
     * 
     */
    void initKeyBinds() {
        std::ifstream file("demo/config/main-menu-state-keys.ini");

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
     * @brief Initialize GUI tools: buttons, drop-down list.
     * 
     */
    void initGUITools() {
        this->buttons["ABOUT"] = new GUI::Button(320.f, 800.f, 150.f, 50.f, &this->button_font, "About", 50, sf::Color(230, 209, 28), sf::Color::Yellow, sf::Color(20, 20, 20, 50), sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent);
        this->buttons["EXIT"] = new GUI::Button(1450.f, 800.f, 150.f, 50.f, &this->button_font, "Quit", 50, sf::Color(212, 17, 17), sf::Color::Red, sf::Color(20, 20, 20, 50), sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent);
        this->buttons["START"] = new GUI::Button(860.f, 650.f, 150.f, 50.f, &this->button_font, "Generate", 50, sf::Color(83, 209, 33), sf::Color::Green, sf::Color((20, 20, 20, 50)), sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent);

        std::vector<std::string> drop_down_button_names {"uint8_t", "uint_16t", "uint32_t", "uint64_t"};
        this->trie_types_list = new GUI::DropDownList(860.f, 400.f, 150.f, 50.f, this->button_font, drop_down_button_names);
    }
    
public:
    /**
     * @brief Construct a new MainMenuState object.
     * 
     * @param window Pointer to the GUI window.
     * @param valid_keys Pointer to the map of valid SFML keys.
     * @param states Pointer to the stack of states.
     * 
     */
    MainMenuState(sf::RenderWindow *window, std::map<std::string, int> *valid_keys, std::stack<State*> *states) : State(window, valid_keys, states)  {
        this->initBackground();
        this->initFonts();
        this->initKeyBinds();
        this->initGUITools();
    }

    /**
     * @brief Destroy the MainMenuState object.
     * 
     */
    virtual ~MainMenuState() {
        for (auto it = this->buttons.begin(); it != this->buttons.end(); it++)
            delete it->second;
        
        delete this->trie_types_list;
    }

    /**
     * @brief Get the ID of this state.
     * 
     * @return unsigned short representing the ID of this state (0).
     */
    const unsigned short getStateID() const {
        return 0;
    }

public:
    /**
     * @brief Updates any direct user input.
     * 
     * @param dt Reference to the delta time variable from gui.h.
     * 
     */
    void updateInput(const float &dt) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->key_binds.at("CLOSE"))) && this->getKeyTimer())
            this->exitState();
    }

    /**
     * @brief Updates the GUI tools: buttons, drop-down list.
     * 
     * @param dt Reference to the delta time variable from gui.h.
     */
    void updateGUI(const float &dt) {
        // Update all MainMenuState buttons.
        for (auto &it : this->buttons)
            it.second->update(this->mouse_pos_view);

        // If the user clicks on the "Generate" button, VisualizerState is pushed to the stack.
        if (this->buttons["START"]->isClicked() && this->getKeyTimer())
            this->states->push(new VisualizerState(this->window, this->valid_keys, this->states, this->trie_types_list->getSelectionID()));
        
        // If the user clicks on the "About" button, AboutState is pushed to the stack.
        if (this->buttons["ABOUT"]->isClicked() && this->getKeyTimer())
            this->states->push(new AboutState(this->window, this->valid_keys, this->states));

        // If the user clicks on the "Quit" button, the MainMenuState is closed.
        if (this->buttons["EXIT"]->isClicked() && this->getKeyTimer())
            this->exitState();

        // Update the drop-down list.
        this->trie_types_list->update(this->mouse_pos_view, dt);
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

        this->updateGUI(dt);
    }

    /**
     * @brief Render the GUI tools: buttons, drop-down list.
     * 
     * @param target Reference to the base SFML RenderTarget.
     */
    void renderGUI(sf::RenderTarget &target) {
        for (auto &it : this->buttons)
            it.second->render(target);

        this->trie_types_list->render(target);
    }

    /**
     * @brief Handles all MainMenuState rendering.
     * 
     * @param target Pointer to the base SFML RenderTarget.
     */
    void render(sf::RenderTarget *target = nullptr) {
        if (!target)
            target = this->window;

        target->draw(this->background);

        this->renderGUI(*target);
    }
};