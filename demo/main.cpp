#include "visualizer.h"

Visualizer visualizer;

int main() {    
    visualizer.run();

    return 0;
}

//TODO: Redesign button colors, make them more consistent and change fonts
    // Courier New
//TODO: Fix uint_64t random number generation and make other random number generators more accurate... check uniform_int_distribution
//TODO: Change backgrounds to Images then textures not rectangles (like in visualizerstate updateBackground())
//TODO: Commet everything in GUI
//TODO: Comment everything in BST
//TODO: Reformat isVisualizerState and getStateID to use short unsigned and const
//TODO: Try to put all cpp files in header files
//TODO: Clean up unnecessary code and files
//TODO: Zoom feature??