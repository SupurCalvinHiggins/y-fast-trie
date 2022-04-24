/**
 * @file augmented-pointer.example.cpp
 * @author Calvin Higgins (calvin_higgins2@uri.edu)
 * @brief Example program using AugmentedPointer.
 * @version 1.0
 * @date 2022-04-24
 * 
 */

#include "../src/augmented-pointer/augmented-pointer.h"
#include <iostream>

int main() {
    int val = 9;
    AugmentedPointer<int*, 2> aug_ptr(&val);

    aug_ptr.set_bit(0);
    if (aug_ptr.is_set_bit(0))
        std::cout << "The 0th bit is set." << std::endl;

    aug_ptr.unset_bit(0);
    if (aug_ptr.is_set_bit(0))
        std::cout << "The 0th bit is not set." << std::endl;
    
    aug_ptr.set_bit(1);
    if (aug_ptr.get_ptr() == &val)
        std::cout << "get_ptr returns the pointer without the data bits." << std::endl;

    int second_val = 10;
    aug_ptr.set_ptr(&second_val);
    if (!aug_ptr.is_set_bit(1))
        std::cout << "Resetting the raw pointer clears the data bits." << std::endl;
}