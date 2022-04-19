#include "visualizer.h"

// Initializers
void Visualizer::initWindow() {
    // Initialize window and set its properties
    std::ifstream file("config/window.ini");
    
    std::string title = "None";
    sf::VideoMode window_size(800, 600);
    unsigned framerate_limit = 120;
    bool vertical_sync_enabled = false;

    if (file.is_open()) {
        std::getline(file, title);
        file >> window_size.width >> window_size.height;
        file >> framerate_limit;
        file >> vertical_sync_enabled;
    }

    file.close();

    this->window = new sf::RenderWindow(sf::VideoMode(window_size), title);
    this->window->setFramerateLimit(framerate_limit);
    this->window->setVerticalSyncEnabled(vertical_sync_enabled);
}

void Visualizer::initStates() {
    // Initialize states
    this->states.push(new VisualizerState(this->window));
}

// Constructors/Destructors
Visualizer::Visualizer() {
    this->initWindow();
    this->initStates();
}

Visualizer::~Visualizer() {
    delete this->window;

    while(!this->states.empty()) {
        delete this->states.top();
        this->states.pop();
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

    system("dot -Tpng assets/test.gv -o assets/output.png");

    sf::Texture texture;
    if (!texture.loadFromFile("assets/output.png"))
    {
        std::cout << "File not found... terminating!" << std::endl;
        return 0;
    }
*/