#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stack>
#include <map>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>


class State {
protected:
    sf::RenderWindow *window;
    std::vector<sf::Texture> textures;
    bool exit;

public:
    State(sf::RenderWindow *window);
    virtual ~State();

    const bool &getExit() const;
    virtual void setExit();
    virtual void exitState() = 0; 
    virtual void updateKeyBinds(const float &dt) = 0;
    virtual void update(const float &dt) = 0;
    virtual void render(sf::RenderTarget *target = nullptr) = 0;
};