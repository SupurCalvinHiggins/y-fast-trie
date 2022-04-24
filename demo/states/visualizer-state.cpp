#include "visualizer-state.h"
#include <iostream>

void VisualizerState::initBackground() {
    if (!this->background_image.loadFromFile("resource/image/background/visualizer-initial.png"))
        throw std::runtime_error("Could not load inital VisualizerState background texture!");
    
    this->background_texture.loadFromImage(this->background_image);

    this->background.setTexture(this->background_texture);
}

void VisualizerState::initFonts() {
    if (!this->font.loadFromFile("resource/font/Dosis-Medium.ttf"))
        throw std::runtime_error("Could not load Dosis-Medium.ttf (VisualizerState Font)");
}

void VisualizerState::initKeyBinds() {
    std::ifstream file("config/visualizer-state-keys.ini");

    if (file.is_open()) {
        std::string key_name;
        std::string valid_key;

        while (file >> key_name >> valid_key) {
            this->key_binds[key_name] = this->valid_keys->at(valid_key);
        }
    }

    file.close();
}

void VisualizerState::initConsoleMenu() {
    this->console_menu = new ConsoleMenu(*this->window, this->font);

    this->console_menu->addButton("EXIT", 900.f, "Exit");

    this->console_menu->addTextBox("INSERT", 200.f);
    this->console_menu->addTextBox("REMOVE", 300.f);
    this->console_menu->addTextBox("PREDECESSOR", 400.f);
    this->console_menu->addTextBox("SUCCESSOR", 500.f);
    this->console_menu->addTextBox("CONTAINS", 600.f);
}

VisualizerState::VisualizerState(sf::RenderWindow *window, std::map<std::string, int> *valid_keys, std::stack<State*> *states, short unsigned trie_type) : State(window, valid_keys, states) {
    this->initBackground();
    this->initKeyBinds();
    this->initFonts();
    this->initConsoleMenu();
    this->trie_type = trie_type;
}

VisualizerState::~VisualizerState() {
    delete this->console_menu;
}

std::string VisualizerState::getStateID() {
    return "VISUALIZER_STATE";
}

void VisualizerState::updateInput(const float &dt) {
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->key_binds.at("OPEN_CONSOLE"))) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->key_binds.at("OPEN_CONSOLE2")))) && this->getKeyTimer()) {
        if (!this->is_in_console)
            this->inConsoleState();
        else
            this->outConsoleState();
    }
}

void VisualizerState::updateRandomInsert() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->key_binds.at("INSERT_RANDOM"))) && this->getKeyTimer()) {
        if (this->trie_type == 0)
            yfast_8.insert(std::rand() % 256);
        else if (this->trie_type == 1)
            yfast_16.insert(std::rand() % 65536);
        else if (this->trie_type == 2)
            yfast_32.insert(std::rand() % 4294967296);
        else if (this->trie_type == 3)
            std::cout << "Inserting random number into uint64_t trie is not supported yet!" << std::endl;
            //yfast_64.insert(std::rand() % 18446744073709551616);
            // FIX
    }
}

void VisualizerState::updateBackground() {
    std::string to_dot_str;
     if (this->trie_type == 0)
        to_dot_str = yfast_8.to_dot();            
    else if (this->trie_type == 1)
        to_dot_str = yfast_16.to_dot();
    else if (this->trie_type == 2)
        to_dot_str = yfast_32.to_dot();
    else if (this->trie_type == 3)
        to_dot_str = yfast_64.to_dot();
    
    std::ofstream output_file("resource/image/visualizer.dot");
    output_file << to_dot_str;
    output_file.close();

    system("dot -Tpng -Gsize=1920,1080\! -Gdpi=1 resource/image/visualizer.dot -o resource/image/background/visualizer.png");

    if (!this->trie_image.loadFromFile("resource/image/background/visualizer.png"))
        throw std::runtime_error("Could not load Y-Fast Trie Visualizer background texture!");

    this->background_image.create(this->window->getSize().x, this->window->getSize().y, sf::Color::White);
    this->background_image.copy(this->trie_image, (this->background_image.getSize().x - this->trie_image.getSize().x)/2, (this->background_image.getSize().y - this->trie_image.getSize().y)/2);
    
    this->background_texture.loadFromImage(this->background_image);
    this->background.setTexture(this->background_texture);
}

void VisualizerState::updateConsoleInput() { // ! CONDENSE PLEASE
    if (this->console_menu->isButtonClicked("EXIT") && this->getKeyTimer())
        this->exitState();
    /*
    if (this->console_menu->isTextBoxEntered("INSERT")) {
        if (this->trie_type == 0) {
            this->outConsoleState();
            yfast_8.insert(std::stoi(user_input));
        }
        else if (this->trie_type == 1)
            std::cout << "YFAST-16 INSERT: " << user_input << std::endl;
        else if (this->trie_type == 2)
            std::cout << "YFAST-32 INSERT: " << user_input << std::endl;
        else if (this->trie_type == 3)
            std::cout << "YFAST-64 INSERT: " << user_input << std::endl;
    }
    */

    
    if (this->console_menu->isTextBoxEntered("INSERT")) {
        if (this->trie_type == 0) {
            if (std::stoul(this->user_input) <= yfast_8.upper_bound())
                yfast_8.insert(std::stoul(this->user_input));
            else 
                yfast_8.insert(yfast_8.upper_bound());
        }
        else if (this->trie_type == 1) {
            if (std::stoul(this->user_input) <= yfast_16.upper_bound())
                yfast_16.insert(std::stoul(this->user_input));
            else 
                yfast_16.insert(yfast_16.upper_bound());
        }
        else if (this->trie_type == 2) {
            if (std::stoul(this->user_input) <= yfast_32.upper_bound())
                yfast_32.insert(std::stoul(this->user_input));
            else 
                yfast_32.insert(yfast_32.upper_bound());
        }
        else if (this->trie_type == 3) {
            if (std::stoul(this->user_input) <= yfast_64.upper_bound())
                yfast_64.insert(std::stoul(this->user_input));
            else 
                yfast_64.insert(yfast_64.upper_bound());
        }
    }
    else if (this->console_menu->isTextBoxEntered("REMOVE")) {
        if (this->trie_type == 0) {
            if (std::stoul(this->user_input) <= yfast_8.upper_bound())
                yfast_8.remove(std::stoul(this->user_input));
            else 
                yfast_8.remove(yfast_8.upper_bound());
        }
        else if (this->trie_type == 1) {
            if (std::stoul(this->user_input) <= yfast_16.upper_bound())
                yfast_16.remove(std::stoul(this->user_input));
            else 
                yfast_16.remove(yfast_16.upper_bound());
        }
        else if (this->trie_type == 2) {
            if (std::stoul(this->user_input) <= yfast_32.upper_bound())
                yfast_32.remove(std::stoul(this->user_input));
            else 
                yfast_32.remove(yfast_32.upper_bound());
        }
        else if (this->trie_type == 3) {
            if (std::stoul(this->user_input) <= yfast_64.upper_bound())
                yfast_64.remove(std::stoul(this->user_input));
            else 
                yfast_64.remove(yfast_64.upper_bound());
        }
    }
    else if (this->console_menu->isTextBoxEntered("PREDECESSOR")) {
        if (this->trie_type == 0) {
            if (std::stoul(this->user_input) <= yfast_8.upper_bound())
                yfast_8.predecessor(std::stoul(this->user_input));
            else 
                yfast_8.predecessor(yfast_8.upper_bound());
        }
        else if (this->trie_type == 1) {
            if (std::stoul(this->user_input) <= yfast_16.upper_bound())
                yfast_16.predecessor(std::stoul(this->user_input));
            else 
                yfast_16.predecessor(yfast_16.upper_bound());
        }
        else if (this->trie_type == 2) {
            if (std::stoul(this->user_input) <= yfast_32.upper_bound())
                yfast_32.predecessor(std::stoul(this->user_input));
            else 
                yfast_32.predecessor(yfast_32.upper_bound());
        }
        else if (this->trie_type == 3) {
            if (std::stoul(this->user_input) <= yfast_64.upper_bound())
                yfast_64.predecessor(std::stoul(this->user_input));
            else 
                yfast_64.predecessor(yfast_64.upper_bound());
        }
    }
    else if (this->console_menu->isTextBoxEntered("SUCCESSOR")) {
        if (this->trie_type == 0) {
            if (std::stoul(this->user_input) <= yfast_8.upper_bound())
                yfast_8.successor(std::stoul(this->user_input));
            else 
                yfast_8.successor(yfast_8.upper_bound());
        }
        else if (this->trie_type == 1) {
            if (std::stoul(this->user_input) <= yfast_16.upper_bound())
                yfast_16.successor(std::stoul(this->user_input));
            else 
                yfast_16.successor(yfast_16.upper_bound());
        }
        else if (this->trie_type == 2) {
            if (std::stoul(this->user_input) <= yfast_32.upper_bound())
                yfast_32.successor(std::stoul(this->user_input));
            else 
                yfast_32.successor(yfast_32.upper_bound());
        }
        else if (this->trie_type == 3) {
            if (std::stoul(this->user_input) <= yfast_64.upper_bound())
                yfast_64.successor(std::stoul(this->user_input));
            else 
                yfast_64.successor(yfast_64.upper_bound());
        }
    }
    else if (this->console_menu->isTextBoxEntered("CONTAINS")) {
        if (this->trie_type == 0) {
            if (std::stoul(this->user_input) <= yfast_8.upper_bound())
                yfast_8.contains(std::stoul(this->user_input));
            else 
                yfast_8.contains(yfast_8.upper_bound());
        }
        else if (this->trie_type == 1) {
            if (std::stoul(this->user_input) <= yfast_16.upper_bound())
                yfast_16.contains(std::stoul(this->user_input));
            else 
                yfast_16.contains(yfast_16.upper_bound());
        }
        else if (this->trie_type == 2) {
            if (std::stoul(this->user_input) <= yfast_32.upper_bound())
                yfast_32.contains(std::stoul(this->user_input));
            else 
                yfast_32.contains(yfast_32.upper_bound());
        }
        else if (this->trie_type == 3) {
            if (std::stoul(this->user_input) <= yfast_64.upper_bound())
                yfast_64.contains(std::stoul(this->user_input));
            else 
                yfast_64.contains(yfast_64.upper_bound());
        }
    }
}

void VisualizerState::update(const float &dt, sf::Event event) {
    this->updateMousePositions();
    this->updateKeyTime(dt);
    this->updateInput(dt);
    
    if (this->is_in_console) {
        this->console_menu->update(this->mouse_pos_view, dt, event, this->user_input);
        this->updateConsoleInput();
    }
    else {
        this->updateRandomInsert();
    }
}

void VisualizerState::render(sf::RenderTarget *target) {
    if (!target) {
        target = this->window;
    }

    target->draw(this->background);

    if (this->is_in_console) { // Render console
        this->console_menu->render(*target);
    }
}