<<com
/**
 * @file compile.command
 * @author Robert Oganesian (roganesian@uri.edu)
 * @brief macOS script to compile the y-fast-trie GUI.
 * @version 1.0
 * @date 2022-04-25
 * 
 */
com

#! /bin/bash

OBJS="main.cpp"
INC="-I/usr/local/Cellar/sfml/2.5.1_1/include"
LIB="-L/usr/local/Cellar/sfml/2.5.1_1/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio"

g++ -O3 -std=c++17 $OBJS $INC -o prog $LIB && ./prog