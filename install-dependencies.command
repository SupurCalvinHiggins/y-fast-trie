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