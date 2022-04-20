#! /bin/bash

which -s brew
if [[ $help != 0 ]] ; then
    # Install Homebrew
    -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
else
    brew update
fi

# Install gcc, git, python3, and termcolor
brew install gcc
brew install git
brew install python3
brew install termcolor

# Install google benchmark for benchmarking support
brew install google-benchmark

# Install google test for testing support
brew install googletest

# Install SFML and Graphviz for SFML GUI support
brew install sfml
brew install graphviz

# Do some research on this file type / take notes of what commands i run when downloading benchmarks/gtest
# Use bootcamp to test if it works

#brew info sfml to find sfml directory
# mdfind kind:folder "sfml"
#command line (WORKS): g++ -std=c++17 main.cpp visualizer.cpp states/state.cpp states/visualizer-state.cpp -I/usr/local/Cellar/sfml/2.5.1_1/include -o prog -L/usr/local/Cellar/sfml/2.5.1_1/lib -lsfml-graphics -lsfml-window -lsfml-system && ./prog
#command line (WORKS): g++ main.cpp visualizer.cpp states/state.cpp states/visualizer-state.cpp -I/usr/local/Cellar/sfml/2.5.1_1/include -o prog -L/usr/local/Cellar/sfml/2.5.1_1/lib -lsfml-graphics -lsfml-window -lsfml-system && ./prog