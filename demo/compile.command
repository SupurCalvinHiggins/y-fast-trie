#! /bin/bash

OBJS="main.cpp visualizer.cpp console-menu.cpp states/state.cpp states/visualizer-state.cpp states/main-menu-state.cpp states/about-state.cpp tools/gui.cpp"
INC="-I/usr/local/Cellar/sfml/2.5.1_1/include"
LIB="-L/usr/local/Cellar/sfml/2.5.1_1/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio"

g++ -std=c++17 $OBJS $INC -o prog $LIB && ./prog