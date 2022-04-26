# @file install-dependencies.command
# @author Robert Oganesian (roganesian@uri.edu)
# @version 1.0
# @date 2022-04-26

#! /bin/bash

if ! command -v brew ; then
    # Install Homebrew
    printf "Homebrew was not found... beginning installation!\n"
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
else
    echo "Brew was found... updating!\n"
    brew update
fi

# Install make, cmake gcc, git, python3, and termcolor
brew install make
brew install cmake
brew install gcc
brew install git
brew install python3
pip3 install termcolor

# Install google benchmark for benchmarking support
brew install google-benchmark

# Install google test for testing support
brew install googletest

# Install SFML and Graphviz for SFML GUI support
brew install sfml
brew install graphviz

printf "\n--- Dependencies installed! ---"
printf "\nFor Uninstall directions, visit: https://mac.install.guide/homebrew/5.html"