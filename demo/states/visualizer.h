/**
 * @file visualizer.h
 * @author Robert Oganesian (roganesian@uri.edu)
 * @brief Visualizer state (state 2).
 * @version 1.0
 * @date 2022-04-25
 * 
 */

#pragma once

#include "state.h"
#include "../tools/console-menu.h"
#include "../src/y-fast-trie/y-fast-trie.h"
#include <random>


class VisualizerState : public State {
private:
    // Required SFML variables.
    sf::Image trie_image;
    sf::Image background_image;
    sf::Texture background_texture;
    sf::Sprite background;
    sf::Font font;
    sf::Text animation_status;

private:
    // Checks if animation mode is ebabled/disabled.
    bool is_animated;

    // Holds the y-fast tryie type selected by the user in the Main Menu.
    short unsigned trie_type;

    // Y-fast trie object of different types.
    YFastTrie<uint8_t> yfast_8;
    YFastTrie<uint16_t> yfast_16;
    YFastTrie<uint32_t> yfast_32;
    YFastTrie<uint64_t> yfast_64;

private:
    // Creates a ConsoleMenu object for the state.
    ConsoleMenu *console_menu;

    // Holds user input from the text boxes in the console.
    std::string user_input;

    // Indicates if the user is currently displaying the console.
    bool is_in_console;

    /**
     * @brief The user is currently displaying the console.
     * 
     */
    void inConsoleMenu() {
        this->is_in_console = true;
    }

    /**
     * @brief The user is currently not displaying the console.
     * 
     */
    void outConsoleMenu() {
        this->is_in_console = false;
    }

private:
    /**
     * @brief Initialize the background.
     * 
     */
    void initBackground() {
        if (!this->background_image.loadFromFile("resource/image/background/visualizer-initial.png"))
            throw std::runtime_error("Could not load inital VisualizerState background texture!");
        
        this->background_texture.loadFromImage(this->background_image);

        this->background.setTexture(this->background_texture);
    }

    /**
     * @brief Initialize the fonts.
     * 
     */
    void initFonts() {
        if (!this->font.loadFromFile("resource/font/Dosis-Medium.ttf"))
            throw std::runtime_error("Could not load Dosis-Medium.ttf (VisualizerState Font)");
    }

    /**
     * @brief Initialize the key binds.
     * 
     */
    void initKeyBinds() {
        std::ifstream file("config/visualizer-state-keys.ini");

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
     * @brief Initialize the console menu object.
     * 
     */
    void initConsoleMenu() {
        this->is_in_console = false;

        this->console_menu = new ConsoleMenu(*this->window, this->font);

        this->console_menu->addButton(900.f, "Exit");

        this->console_menu->addTextBox("INSERT", 210.f);
        this->console_menu->addTextBox("REMOVE", 325.f);
        this->console_menu->addTextBox("PREDECESSOR", 435.f);
        this->console_menu->addTextBox("SUCCESSOR", 550.f);
        this->console_menu->addTextBox("CONTAINS", 665.f);
    }

    /**
     * @brief Display if animation mode is enabled/disabled.
     * 
     */
    void displayAnimationStatus() {
        this->animation_status.setFont(this->font);
        this->animation_status.setCharacterSize(50);

        if (this->is_animated) {
            this->animation_status.setString("Animation: ON");
            this->animation_status.setFillColor(sf::Color::Green);
        } else {
            this->animation_status.setString("Animation: OFF");
            this->animation_status.setFillColor(sf::Color::Red);
        }
        
        // Positions the text at the center of the window.
        this->animation_status.setPosition(this->window->getSize().x / 2.f - this->animation_status.getGlobalBounds().width / 2.f, this->window->getSize().y / 2.f - this->animation_status.getGlobalBounds().height / 2.f);
        
        this->window->draw(this->animation_status);
        this->window->display();

        // Displays text for 1 second.
        sf::sleep(sf::seconds(1.f));

        this->window->clear();
    }
    
public:
    /**
     * @brief Construct a new VisualizerState object.
     * 
     * @param window Pointer to the GUI window.
     * @param valid_keys Pointer to the map of valid SFML keys.
     * @param states Pointer to the stack of states.
     * 
     */
    VisualizerState(sf::RenderWindow *window, std::map<std::string, int> *valid_keys, std::stack<State*> *states, unsigned short trie_type) : State(window, valid_keys, states) {
        this->initBackground();
        this->initKeyBinds();
        this->initFonts();
        this->initConsoleMenu();
        
        this->trie_type = trie_type;
        this->is_animated = true;
    }

    /**
     * @brief Destroy the VisualizerState object.
     * 
     */
    virtual ~VisualizerState() {
        delete this->console_menu;
    }

    /**
     * @brief Get the ID of this state.
     * 
     * @return unsigned short representing the ID of this state (2).
     */
    const unsigned short getStateID() const {
        return 2;
    }
    
public:
    /**
     * @brief Updates any direct user input.
     * 
     * @param dt Reference to the delta time variable from gui.h.
     * 
     */
    void updateInput(const float &dt) {
        // Updates if console is displayed or not.
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->key_binds.at("OPEN_CONSOLE"))) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->key_binds.at("OPEN_CONSOLE2")))) && this->getKeyTimer()) {
            if (!this->is_in_console)
                this->inConsoleMenu();
            else
                this->outConsoleMenu();
        }

        // Updates animation mode.
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->key_binds.at("ANIMATION_ON_OFF"))) && this->getKeyTimer()) {
            this->is_animated = !this->is_animated;
            yfast_8.set_animate(this->is_animated);
            yfast_16.set_animate(this->is_animated);
            yfast_32.set_animate(this->is_animated);
            yfast_64.set_animate(this->is_animated);
            
            this->displayAnimationStatus();
        }
    }

    /**
     * @brief Randomly inserts into selected y-fast trie.
     * 
     */
    void updateRandomInsert() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->key_binds.at("INSERT_RANDOM"))) && this->getKeyTimer()) {
            std::random_device rd;
            std::mt19937_64 gen(rd());

            if (this->trie_type == 0) {
                std::uniform_int_distribution<uint8_t> dis;
                yfast_8.insert(dis(gen));
            }
            else if (this->trie_type == 1) {
                std::uniform_int_distribution<uint16_t> dis;
                yfast_16.insert(dis(gen));
            }
            else if (this->trie_type == 2) {
                std::uniform_int_distribution<uint32_t> dis;
                yfast_32.insert(dis(gen));
            }
            else if (this->trie_type == 3) {
                std::uniform_int_distribution<uint64_t> dis;
                yfast_64.insert(dis(gen));
            }
        }
    }

    /**
     * @brief Handles all console input.
     * 
     * @details Operations: Insert, remove, predecessor, successor, contains.
     * @details The function will handle all operations on selected y-fast trie type.
     * 
     */
    void updateConsoleInput() {
        if (this->console_menu->isButtonClicked() && this->getKeyTimer())
            this->exitState();
        
        if (this->console_menu->isTextBoxEntered("INSERT")) {
            if (this->user_input.empty())
                return;

            this->outConsoleMenu();
            if (this->trie_type == 0) {
                // Checks if user input is a valid 8-bit unsigned integer.
                if (std::stoul(this->user_input) <= yfast_8.upper_bound())
                    yfast_8.insert(std::stoul(this->user_input));
                else 
                    yfast_8.insert(yfast_8.upper_bound());
            }
            else if (this->trie_type == 1) {
                // Checks if user input is a valid 16-bit unsigned integer.
                if (std::stoul(this->user_input) <= yfast_16.upper_bound())
                    yfast_16.insert(std::stoul(this->user_input));
                else 
                    yfast_16.insert(yfast_16.upper_bound());
            }
            else if (this->trie_type == 2) {
                // Checks if user input is a valid 32-bit unsigned integer.
                if (std::stoul(this->user_input) <= yfast_32.upper_bound())
                    yfast_32.insert(std::stoul(this->user_input));
                else 
                    yfast_32.insert(yfast_32.upper_bound());
            }
            else if (this->trie_type == 3) {
                // Checks if user input is a valid 64-bit unsigned integer.
                if (std::stoul(this->user_input) <= yfast_64.upper_bound())
                    yfast_64.insert(std::stoul(this->user_input));
                else 
                    yfast_64.insert(yfast_64.upper_bound());
            }
        }
        else if (this->console_menu->isTextBoxEntered("REMOVE")) {
            if (this->user_input.empty())
                return;

            this->outConsoleMenu();
            if (this->trie_type == 0) {
                // Checks if user input is a valid 8-bit unsigned integer.
                if (std::stoul(this->user_input) <= yfast_8.upper_bound())
                    yfast_8.remove(std::stoul(this->user_input));
                else 
                    yfast_8.remove(yfast_8.upper_bound());
            }
            else if (this->trie_type == 1) {
                // Checks if user input is a valid 16-bit unsigned integer.
                if (std::stoul(this->user_input) <= yfast_16.upper_bound())
                    yfast_16.remove(std::stoul(this->user_input));
                else 
                    yfast_16.remove(yfast_16.upper_bound());
            }
            else if (this->trie_type == 2) {
                // Checks if user input is a valid 32-bit unsigned integer.
                if (std::stoul(this->user_input) <= yfast_32.upper_bound())
                    yfast_32.remove(std::stoul(this->user_input));
                else 
                    yfast_32.remove(yfast_32.upper_bound());
            }
            else if (this->trie_type == 3) {
                // Checks if user input is a valid 64-bit unsigned integer.
                if (std::stoul(this->user_input) <= yfast_64.upper_bound())
                    yfast_64.remove(std::stoul(this->user_input));
                else 
                    yfast_64.remove(yfast_64.upper_bound());
            }
        }
        else if (this->console_menu->isTextBoxEntered("PREDECESSOR")) {
            if (this->user_input.empty())
                return;

            this->outConsoleMenu();
            if (this->trie_type == 0) {
                // Checks if user input is a valid 8-bit unsigned integer.
                if (std::stoul(this->user_input) <= yfast_8.upper_bound())
                    yfast_8.predecessor(std::stoul(this->user_input));
                else 
                    yfast_8.predecessor(yfast_8.upper_bound());
            }
            else if (this->trie_type == 1) {
                // Checks if user input is a valid 16-bit unsigned integer.
                if (std::stoul(this->user_input) <= yfast_16.upper_bound())
                    yfast_16.predecessor(std::stoul(this->user_input));
                else 
                    yfast_16.predecessor(yfast_16.upper_bound());
            }
            else if (this->trie_type == 2) {
                // Checks if user input is a valid 32-bit unsigned integer.
                if (std::stoul(this->user_input) <= yfast_32.upper_bound())
                    yfast_32.predecessor(std::stoul(this->user_input));
                else 
                    yfast_32.predecessor(yfast_32.upper_bound());
            }
            else if (this->trie_type == 3) {
                // Checks if user input is a valid 64-bit unsigned integer.
                if (std::stoul(this->user_input) <= yfast_64.upper_bound())
                    yfast_64.predecessor(std::stoul(this->user_input));
                else 
                    yfast_64.predecessor(yfast_64.upper_bound());
            }
        }
        else if (this->console_menu->isTextBoxEntered("SUCCESSOR")) {
            if (this->user_input.empty())
                return;

            this->outConsoleMenu();
            if (this->trie_type == 0) {
                // Checks if user input is a valid 8-bit unsigned integer.
                if (std::stoul(this->user_input) <= yfast_8.upper_bound())
                    yfast_8.successor(std::stoul(this->user_input));
                else 
                    yfast_8.successor(yfast_8.upper_bound());
            }
            else if (this->trie_type == 1) {
                // Checks if user input is a valid 16-bit unsigned integer.
                if (std::stoul(this->user_input) <= yfast_16.upper_bound())
                    yfast_16.successor(std::stoul(this->user_input));
                else 
                    yfast_16.successor(yfast_16.upper_bound());
            }
            else if (this->trie_type == 2) {
                // Checks if user input is a valid 32-bit unsigned integer.
                if (std::stoul(this->user_input) <= yfast_32.upper_bound())
                    yfast_32.successor(std::stoul(this->user_input));
                else 
                    yfast_32.successor(yfast_32.upper_bound());
            }
            else if (this->trie_type == 3) {
                // Checks if user input is a valid 64-bit unsigned integer.
                if (std::stoul(this->user_input) <= yfast_64.upper_bound())
                    yfast_64.successor(std::stoul(this->user_input));
                else 
                    yfast_64.successor(yfast_64.upper_bound());
            }
        }
        else if (this->console_menu->isTextBoxEntered("CONTAINS")) {
            if (this->user_input.empty())
                return;
                
            this->outConsoleMenu();
            if (this->trie_type == 0) {
                // Checks if user input is a valid 8-bit unsigned integer.
                if (std::stoul(this->user_input) <= yfast_8.upper_bound())
                    yfast_8.contains(std::stoul(this->user_input));
                else 
                    yfast_8.contains(yfast_8.upper_bound());
            }
            else if (this->trie_type == 1) {
                // Checks if user input is a valid 16-bit unsigned integer.
                if (std::stoul(this->user_input) <= yfast_16.upper_bound())
                    yfast_16.contains(std::stoul(this->user_input));
                else 
                    yfast_16.contains(yfast_16.upper_bound());
            }
            else if (this->trie_type == 2) {
                // Checks if user input is a valid 32-bit unsigned integer.
                if (std::stoul(this->user_input) <= yfast_32.upper_bound())
                    yfast_32.contains(std::stoul(this->user_input));
                else 
                    yfast_32.contains(yfast_32.upper_bound());
            }
            else if (this->trie_type == 3) {
                // Checks if user input is a valid 64-bit unsigned integer.
                if (std::stoul(this->user_input) <= yfast_64.upper_bound())
                    yfast_64.contains(std::stoul(this->user_input));
                else 
                    yfast_64.contains(yfast_64.upper_bound());
            }
        }
    }

    /**
     * @brief Updates the y-fast trie display.
     * 
     * @details The background is updated to reflect the current state of the y-fast trie.
     * 
     */
    void updateBackground() {
        std::string to_dot_str;

        // Updates string to contain the y-fast trie in dot format.
        if (this->trie_type == 0)
            to_dot_str = yfast_8.to_dot();            
        else if (this->trie_type == 1)
            to_dot_str = yfast_16.to_dot();
        else if (this->trie_type == 2)
            to_dot_str = yfast_32.to_dot();
        else if (this->trie_type == 3)
            to_dot_str = yfast_64.to_dot();
    
        // Writes the dot string to the dot file.
        std::ofstream output_file("resource/image/visualizer.dot");
        output_file << to_dot_str;
        output_file.close();

        // Convert dot file to png using Graphviz.
        system("dot -Tpng -Gsize=1920,1080\! -Gdpi=1 resource/image/visualizer.dot -o resource/image/background/visualizer.png");

        if (!this->trie_image.loadFromFile("resource/image/background/visualizer.png"))
            throw std::runtime_error("Could not load Y-Fast Trie PNG!");

        // Pads the image with whitespace to meet the window size.
        this->background_image.create(this->window->getSize().x, this->window->getSize().y, sf::Color::White);
        this->background_image.copy(this->trie_image, (this->background_image.getSize().x - this->trie_image.getSize().x)/2, (this->background_image.getSize().y - this->trie_image.getSize().y)/2);
        
        this->background_texture.loadFromImage(this->background_image);
        this->background.setTexture(this->background_texture);
    }

    /**
     * @brief Update any logic within this state.
     * 
     * @param dt Reference to the delta time variable from gui.h.
     * @param event Holds the current event from gui.h.
     * 
     */
    void update(const float &dt, sf::Event event) {
        this->updateMousePositions();
        this->updateKeyTime(dt);
        this->updateInput(dt);
        
        // If console menu is open, update the console menu.
        if (this->is_in_console) {
            this->console_menu->update(this->mouse_pos_view, dt, event, this->user_input);
            this->updateConsoleInput();
        }
        // If the console menu is closed, allow random insertion.
        else {
            this->updateRandomInsert();
        }
    }

    /**
     * @brief Handles all VisualizerState rendering.
     * 
     * @param target Pointer to the base SFML RenderTarget.
     */
    void render(sf::RenderTarget *target = nullptr) {
        if (!target)
            target = this->window;

        target->draw(this->background);

        // If console menu is open, render the console menu.
        if (this->is_in_console)
            this->console_menu->render(*target);
    }
};