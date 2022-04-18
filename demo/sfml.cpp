#include <iostream>
#include <SFML/Graphics.hpp>
#include "y-fast-trie.visualize.h"

///////////////////////////////////////////////////////////


int main(int argc, char *argv[]) {

    // TODO Make it work with dot converter function directly rather than generating before
    // TODO Function to find PNG pixel dimensions

    system("dot -Tpng assets/test.gv -o assets/output.png");

    // 840 × 859 used temporarily since it's the size of the empty PNG
    sf::RenderWindow window(sf::VideoMode(371, 347), "RB Tree Visualizer");

    sf::Texture texture;
    if (!texture.loadFromFile("assets/output.png"))
    {
        std::cout << "File not found... terminating!" << std::endl;
        return 0;
    }

    sf::Sprite sprite;
    sprite.setTexture(texture);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }
} 
