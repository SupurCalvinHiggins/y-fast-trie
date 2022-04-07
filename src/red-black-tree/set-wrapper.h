//set-wrapper.h
#pragma once
#include "red-black-tree-node.h"
#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <optional>
#include <set>

template <typename T>
class RedBlackTree {
    friend Node<T>;
public:
	RedBlackTree();
    ~RedBlackTree();

    std::optional<T> predecessor(T key);
	std::optional<T> successor(T key);

    RedBlackTree<T>* merge(RedBlackTree<T> tree1, RedBlackTree<T> tree2);

    RedBlackTree<T>** split(RedBlackTree<T> tree);

	void insert(T key);
	void remove(T key);
    void clear();

    bool contains(T key);

    int size();
    int height() {return std::ceil(std::log2(size_));};
    
    T limit() {return -1;};
    
private:
	int size_;
    std::set<T> root_;
};

template <typename T> RedBlackTree<T>::RedBlackTree() {
    root_ = std::set<T>();
    size_ = 0;
};

template <typename T> RedBlackTree<T>::~RedBlackTree() {
    clear();
};

template <typename T>
void RedBlackTree<T>::insert(T key){
    root_.insert(key);
    size_++;
};

template <typename T>
void RedBlackTree<T>::remove(T key){
    root_.erase(key);
    size_--;
};

template <typename T>
void RedBlackTree<T>::clear(){
    root_.clear();
    size_ = 0;
};

template <typename T>
RedBlackTree<T>* RedBlackTree<T>::merge(RedBlackTree<T> tree1, RedBlackTree<T> tree2){
    RedBlackTree<T>* merged = new RedBlackTree<T>();
    merged->root_.merge(tree1->root_);
    merged->root_.merge(tree2->root_);
    return merged;
};

template <typename T>
std::optional<T> RedBlackTree<T>::predecessor(T key) {
    return std::nullopt;
    /*T target = *root_.begin();
    T next = *root_.upper_bound(target);
    while (next < key) {
        T temp = next;
        next = *root_.upper_bound(target);
        target = temp;
    }
    if(target) return std::optional<T>(target);
    return std::nullopt;
    */
};

template <typename T>
std::optional<T> RedBlackTree<T>::successor(T key) {
    T next = *root_.upper_bound(key);
    if(next) return std::optional<T>(next);
    return std::nullopt; 
};

template <typename T>
bool RedBlackTree<T>::contains(T key){
    return root_.find(key) != root_.end();
};