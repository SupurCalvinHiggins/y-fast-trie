/**
 * @file red-black-tree.h
 * @author Ethan Carlson
 * @brief This is the header for a red black tree class template (RedBlackTree), which is a self balancing binary search tree.
 * @version 1.0
 * @date 2022-04-09
 * 
 */
#pragma once
#include "red-black-tree-node.h"
#include <cassert>
#include <filesystem>
#include <ctime>
#include <array>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cmath>
#include <optional>
#include <algorithm>

template <typename Key_>
class RedBlackTree {
//Implicit types
public:
    using key_type = Key_;
    using some_key_type = std::optional<Key_>;

private:
    using node_type = Node<key_type>;
    using node_ptr = node_type*;
    using size_type = size_t;

//Aliases
private:
    static constexpr bool black_ = 0;
    static constexpr bool red_ = 1;

//Contructor and destructor
public:

    /**
     * @brief Construct a new RedBlackTree object
     * 
     */
	RedBlackTree() {
        root_ = nullptr;
        max_ = nullptr;
        min_ = nullptr;
        size_ = 0;
    };

    /**
     * @brief Destroy the RedBlackTree object
     * 
     */
    ~RedBlackTree() {
        clear();
    };

//Core functions:
public:

    /**
     * @brief Inserts a node into the tree with the input key
     * 
     * @param key The key of the new node to be inserted into the tree
     */
    void insert(key_type key) {
        //See insert by node ptr for the implementation
        insert(new Node<key_type>(key,1));
    };

    /**
     * @brief Remove the node that has the input key. Will do nothing if the key isn't in the tree.
     * 
     * @param key The value of the node that is to be deleted.
     */
	void remove(key_type key) {
        auto node = find(key);
        if (node == nullptr) {return;}
        if (max_ && key == max_->key_){
            if (!max_->parent_){
                max_ = max_->children_[0];
            }
            else{
                max_ = max_->parent_;
            }
        }
        if (min_ && key == min_->key_){
            if (!min_->parent_){
                min_ = min_->children_[1];
            }
            else{
                min_ = min_->parent_;
            }
        }
        //<dir> carries the direction of the node relative to its parent
        bool dir;
        node_ptr parent = nullptr;
        bool color;
        
        if (node != root_) {
            //"node->parent_->children_[1] == node" will always return the direction of the node relative to its parent, so is used
            //frequently in place of if else statements for determining direction.
            dir = node->parent_->children_[1] == node;
            parent = node->parent_;
            bool child_dir = parent->children_[1] == node;
            if (node->children_[0] == nullptr) {
                //Case 1: Node is a leaf (has no children), so can be deleted without precaution.
                if (node->children_[1] == nullptr) {
                    parent->children_[child_dir] = nullptr;
                    color = node->color_;
                    delete node;
                }
                //Case 2: Node only has a right child, so the node gets replaced by its right child before deletion.
                else {
                    parent->children_[child_dir] = node->children_[1];
                    node->children_[1]->parent_ = parent;
                    color = node->color_;
                    delete node;
                }

            }
            else {
                //Case 3: Node only has a left child, so the node gets replaced by its left child before deletion.
                if (node->children_[1] == nullptr) {
                    parent->children_[child_dir] = node->children_[0];
                    node->children_[0]->parent_ = parent;
                    color = node->color_;
                    delete node;
                }
                //Case 4: Node has two children, so the node's key is replaced by its successor and the succesor node gets deleted.
                else {
                    node_ptr succ = node->children_[1];
                    while (succ->children_[0] != nullptr) {
                        succ = succ->children_[0];
                    }
                    node->key_ = succ->key_;

                    succ->parent_->children_[succ->parent_->children_[1] == succ] = succ->children_[1];
                    if (succ->children_[1] != nullptr) {
                        succ->children_[1]->parent_ = succ->parent_;
                    }

                    parent = succ->parent_;
                    color = succ->color_;
                    delete succ;
                    
                }
            }
            //Maintains the RedBlackTree invariant
            remove_check(parent, color, dir);
            
        }
        //Cases 5&6: The root is being deleted, so replace the root with either its successor or left child
        else {
            auto succ = successor_node(node);
            //Case 5: Root has no successor, so replace it with its left child
            if (succ == nullptr) {
                root_ = node->children_[0];
                if (root_ != nullptr) {
                    root_->parent_ = nullptr;
                    root_->color_ = black_;
                }
                //Case 5.5: Root has no children, so just delete the root.
                else {
                    parent = node->parent_;
                    color = node->color_;
                    delete node;
                }
                
            }
            //Case 6: Root has successor, so replace the root's key with the successor's key and delete the successor
            else {
                
                node->key_ = succ->key_;

                succ->parent_->children_[succ->parent_->children_[1] == succ] = succ->children_[1];
                if (succ->children_[1] != nullptr) {
                    succ->children_[1]->parent_ = succ->parent_;
                }

                parent = succ->parent_;
                dir = succ->parent_->children_[1] == succ;
                color = succ->color_;
                delete succ;
                //Maintains the RedBlackTree invariant
                remove_check(parent, color, dir);
            }
        }
        size_--;
    };

    /**
     * @brief Calls the recursive clear, which will delete all nodes in the tree.
     * 
     * @tparam key_type 
     */
    void clear() {
        //See this recursive submethod to see the actual process
        clear(root_);
    };

private:
    /**
     * @brief Insert a node into a tree. This is used to reuse nodes from an otherwise deleted tree during merging
     * 
     * @param node The preexisting node to insert into the tree
     */
	void insert(node_ptr node){
        if (!node) return;
        key_type key = node->key_;
        //Resets node relations. These relations will be updated during insertion.
        node->children_[0] = nullptr;
        node->children_[1] = nullptr;
        node->parent_ = nullptr;

        //Case 1: Tree has no root, so replace root with node.
        if (root_ == nullptr) {
            root_ = node;
            root_->color_ = black_;
            size_++;
            if (!max_ || key > max_->key_){
                max_ = node;
            }
            if (!min_ || key < min_->key_){
                min_ = node;
            }
        }
        //Case 2: Tree has root, so binary search for the right spot to insert the node into.
        else {
            node->color_ = red_;
            
            node_ptr target_node = root_;
            while (target_node != nullptr && target_node->key_ != key) {
                bool dir = key > target_node->key_;
                if (target_node->children_[dir] == nullptr) {
                    target_node->children_[dir] = node;
                    node->parent_ = target_node;
                    size_++;
                    insert_check(node);

                    if (!max_ || key > max_->key_){
                        max_ = node;
                    }
                    if (!min_ || key < min_->key_){
                        min_ = node;
                    }
                    return;
                }
                else {
                    target_node = target_node->children_[dir];
                }
            }
            if (target_node->key_ == key){
                delete node;
            }
        }
       
    };

//Accessors
public:

    /**
     * @brief Finds the nth element in the set of the RedBlackTree.
     * 
     * @param n The index of the desired element.
     * @return some_key_type 
     */
    some_key_type get(size_type n) {
        if (n >= size_){
            if(max_){
                return some_key_type(max_->key_);
            }else{
                return std::nullopt;
            }
        }
        node_ptr target = min_;
    
        for (int i = 0; i < n; i++){
            auto succ = target->children_[1];

            //Case 1: Target node has right child, so find leftmost node from the subtree of the target node's right child
            if(succ){
                while(succ->children_[0]) {
                    succ = succ->children_[0];
                    target = succ;
                }
            }
            else{
                //Case 2: The target node has no right child, so find the parent with a value closest to but greater than the value.
                auto parent = target->parent_;
                while (parent) {
                    if (target == parent->children_[0]) break;
                    target = parent;
                    parent = parent->parent_;
                }
            }
        }
        return some_key_type(target->key_);
        
    };

    /**
     * @brief Returns the median key of the tree. If the tree has an even number of nodes, it gives the greater key of the two middle keys.
     * If the tree has no nodes, std::nullopt is returned.
     * 
     * @return some_key_type 
     */
    key_type median() {
        assert(root_ && "Use of median on an empty tree is not supported");
        return get(size_/2).value();
    };

    /**
     * @brief Returns a node of the input value if it is in the tree, returns <nullptr> if else.
     * 
     * @param key The key of the node that is being searched for.
     * @return node_ptr The node that had the input key, or nullptr if that does not exist.
     */
    node_ptr find(key_type key){
        auto target_node = root_;
        while (target_node != nullptr && target_node->key_ != key) {
            target_node = target_node->children_[key > target_node->key_];
        }
        return target_node;
    };

    /**
     * @brief Checks if a given key is inside of the tree, returning true or false.
     * 
     * @param key 
     * @return true if the key is in the tree.
     * @return false if the key is not in the tree.
     */
    bool contains(key_type key) {
        return find(key) != nullptr;
    };

    /**
     * @brief Finds the value closest to the target value that is strictly less than it.
     * 
     * @param node The value to find the exclusive predecessor of, which is strictly less than the value
     * @return some_key_type The exclusive predecessor of the value.
     * Returns nullopt if <key> is not in the tree, or there is no value less than the input value in the tree.
     * 
     */
    some_key_type predecessor(key_type key) {

        if(!root_) return std::nullopt;

        //Finds the parent of the key if it were to be inserted into the tree.
        //The predecessor will be the same for this target node as the key.
        auto target = root_;
        while (target->children_[key > target->key_] != nullptr) {
            target = target->children_[key > target->key_];
            //If the input key is in the tree, it used that as the target instead.
            if (target->key_ == key) break;
        }
        
        if (target->key_ < key){
            return some_key_type(target->key_);
        }

        auto pred = target->children_[0];

        //Case 1: Target node has left child, so find rightmost node from the subtree of the target node's left child
        if(pred){
            while(pred->children_[1]) {
                pred = pred->children_[1];
            }
            return some_key_type(pred->key_);
        }
        
        //Case 2: The target node has no left child, so find the parent with a value closest to but less than the value.
        auto parent = target->parent_;

        while (parent) {
            if (target == parent->children_[1]) break;
            target = parent;
            parent = parent->parent_;
        }

        if (parent) return some_key_type(parent->key_);
    
        return std::nullopt;
    };

    /**
     * @brief Finds the inclusive successor of the value, which is the value closest to and strictly greater than the input value.
     * 
     * @param node The value to find the exclusive successor of, which is strictly greater than the value
     * @return some_key_type The exclusive succecessor of the value.
     * Returns nullopt if <key> is not in the tree, or there is no value greater than the input value.
     * 
     */
    some_key_type successor(key_type key) {
        
        if(!root_) return std::nullopt;

        //Finds the parent of the key if it were to be inserted into the tree.
        //The successor will be the same for this target node as the key.
        auto target = root_;
        while (target->children_[key > target->key_] != nullptr) {
            target = target->children_[key > target->key_];
            //If the input key is in the tree, it used that as the target instead.
            if (target->key_ == key) break;
        }
        
        if (target->key_ > key){
            return some_key_type(target->key_);
        }

        auto succ = target->children_[1];

        //Case 1: Target node has right child, so find leftmost node from the subtree of the target node's right child
        if(succ){
            while(succ->children_[0]) {
                succ = succ->children_[0];
            }
            return some_key_type(succ->key_);
        }
        
        //Case 2: The target node has no right child, so find the parent with a value closest to but greater than the value.
        auto parent = target->parent_;

        while (parent) {
            if (target == parent->children_[0]) break;
            target = parent;
            parent = parent->parent_;
        }

        if (parent) return some_key_type(parent->key_);
    
        return std::nullopt;
    };

    /**
     * @brief Returns the maximum value of the tree. Returns nullopt if the tree is empty
     * 
     * @return some_key_type The maximum of the tree, which may be none if the tree is empty.
     */
    some_key_type max() {
        if (!max_) return std::nullopt;
        return some_key_type(max_->key_);
    }

    /**
     * @brief Returns the minimum value of the tree. Returns nullopt if the tree is empty
     * 
     * @return some_key_type The minimum of the tree, which may be none if the tree is empty.
     */
    some_key_type min() {
        if (!min_) return std::nullopt;
        return some_key_type(min_->key_);
    };

private:
    /**
     * @brief Finds the node with the closest key to the target node's key that is greater it.
     * The value has to be in the the tree. Used in deletion.
     * 
     * @param node The node to find the successor of.
     * @return node_ptr The successor of the node, which may be the node itself. The node must be in the tree
     */
    node_ptr successor_node(node_ptr node) {
        if (!node) {return nullptr;}

        node_ptr succ = node->children_[1];
        if (succ){
            while (succ->children_[0] != nullptr) {
                succ = succ->children_[0];
            }
        }
        
        return succ;
    };

//Split/merge
public:
    /**
     * @brief Gets all nodes from <tree2> and puts them into <tree1>, deleting <tree2> afterwards.
     * 
     * @param tree1 The first tree, which will have all nodes from <tree2> added to it.
     * @param tree2 The second tree, which will have all of its nodes inserted into tree1, Is deleted after merging.
     * @return RedBlackTree<key_type>* The pointer to the merged tree.
     */
    RedBlackTree<key_type>* merge(RedBlackTree<key_type>* tree1, RedBlackTree<key_type>* tree2){
        //The bulk of this function is done in the recursive submethod, so see that function for a better understanding
        tree1->merge(tree2->root_,tree1);
        //Because the nodes of <tree2> are recycled, the root is set to nullptr so all of the nodes don't get deleted during deletion.
        tree2->root_ = nullptr;
        delete tree2;
        return tree1;
    };

    /**
     * @brief Splits the tree into two trees about the input key. The original is practically cleared after the split.
     * The first tree that is returned is all nodes less than <key>, and the other tree is all nodes greater than it.
     * 
     * @param pivot The pivot for the tree to split about.
     * @return std::array<RedBlackTree<key_type>*,2> The first tree that is returned is all 
     * nodes less than <key>, and the other tree is all nodes greater than it.
     */
    std::array<RedBlackTree<key_type>*,2>  split(key_type pivot) {
        RedBlackTree<key_type>* tree1 = new RedBlackTree<key_type>();
        RedBlackTree<key_type>* tree2 = new RedBlackTree<key_type>();
        
        //The majority of this function is done with the recursive submethod, so see that for more information.
        split(root_, pivot, tree1, tree2);

        root_ = nullptr;
        size_ = 0;
        return std::array<RedBlackTree<key_type>*,2>{tree1, tree2}; 
    };

    /**
     * @brief Merges <tree2> into the tree that merge is called from. Deletes <tree2> afterwards.
     * 
     * @param tree2 The tree being merged into the tree merge is called from. Is deleted after merging.
     */
    void merge(RedBlackTree<key_type>* tree2) {
        //The bulk of this function is done in the recursive merge, so see that function for a better understanding
        merge(tree2->root_,this);
        //Because the nodes of <tree2> are recycled, the root is set to nullptr so all of the nodes don't get deleted.
        tree2->root_ = nullptr;
        delete tree2;
    };

//Data members
private:
    node_ptr root_;
	size_type size_;
    node_ptr max_;
    node_ptr min_;

//Class constants
private:
    //The maximum value for the type that Key_ is using.
    static constexpr key_type upper_bound_ = std::numeric_limits<key_type>::max();

    //The minimum value for the type that Key_ is using.
	static constexpr key_type lower_bound_ = std::numeric_limits<key_type>::min();


    /**
     * @brief Recursively fills the referenced input vector with all nodes from a tree of the input root.
     * 
     * @param root The root of the tree to get the nodes from. 
     * @param nodes The vector to store the tree's nodes in.
     */
    void nodes(node_ptr root, std::vector<node_ptr>* node_vector){
        //If the current root exists...
        if (root){
            node_vector->push_back(root);
            nodes(root->children_[0], node_vector);
            nodes(root->children_[1], node_vector);
        }
    };

private:
    /**
     * @brief Maintains the RedBlackTree invariant after standard binary search tree insertion. May call itself if needed.
     * 
     * @param node The newly inserted node.
     */
    void insert_check(Node<Key_>* node) {
        auto parent = node->parent_;
        //Case 1:  Inserted node is the new root, so do nothing.
        if (parent == nullptr || !(parent->color_ == red_) ) {
            return;
        }
        //Cases 2-4:...
        else {
            //Gets the parent direction relative to the node's grandparent, instead of using an if else clause.
            bool parent_dir = parent->parent_->children_[1] == parent;
            bool target_dir = parent->children_[1] == node;
            auto sibling = parent->parent_->children_[parent_dir ^ 1];
            //Checks if the sibling of the node is black.
            bool black_sibling = (sibling == nullptr || sibling->color_ == black_);

            //Case 2: If the sibling of the inserted node is black, perform a rotation dependant on the node's direction and the parent's direction relative to their 
            //parents, and swap the color of the node and its children.
            if (black_sibling) {
                node_ptr first_rotate_targets[2] = {parent, parent->parent_};
                //Does the first rotation on the the parent if the node and parent lean different ways.
                //Does the first rotation on the the grandparent if the node and parent lean the same way.
                //The first rotation is in the direction opposite to the one the node is leaning towards.
                auto target_node = rotate(first_rotate_targets[parent_dir == target_dir], target_dir ^ 1);
                //Does the second rotation on the parent if the parent and node lean diffrent ways.
                //The second rotation is done in the direction the node leans towards.
                if (parent_dir != target_dir) {
                    parent = node->parent_;
                    target_node = rotate(parent, target_dir);
                }
                //Swaps the colors between the node and its children.
                bool color = target_node->color_;
                target_node->color_ = color^1;
                if (target_node->children_[0] != nullptr) {
                    target_node->children_[0]->color_ = color;
                }
                if (target_node->children_[1] != nullptr) {
                    target_node->children_[1]->color_ = color;
                }
            }
            //Case 3: Node has a red sibling, so change the color of the cousins of the node.
            else {
                if (parent->parent_->children_[0] != nullptr) {
                    parent->parent_->children_[0]->color_ = parent->parent_->children_[0]->color_ ^ 1;
                }
                if (parent->parent_->children_[1] != nullptr) {
                    parent->parent_->children_[1]->color_ = parent->parent_->children_[1]->color_ ^ 1;
                }
                //Case 4: Ontop of case 3, if the parent is not the root, then inverse the color of the grandparent and do another
                //insertion check on the grandparent.
                if (parent->parent_ != root_) {
                    parent->parent_->color_ ^= 1;
                    insert_check(parent->parent_);
                    if (root_->color_ == 1) {
                        root_->color_ = 0;
                    }
                }
            }
        }
    };
    
    /**
     * @brief Maintains the RedBlackTree invariant after binary search tree deletion is performed. May call itself if needed.
     * 
     * @param parent The parent of the deleted node.
     * @param color The color of the deleted node.
     * @param dir The direction of the deleted node relative to its parent.
     */
    void remove_check(node_ptr parent, bool color, bool dir) {
        //Case 1: The color of the deleted node is red, so just color the new node black
        if (color == red_) {
            parent->color_ = black_;
        }
        //Cases 2-6: The color of the deleted node is black, which could require the parent node to be "double black"
        //due to the color of the node and its nil children. Thus, special cases are needed.
        else {
            //Cases 2-6: If the deleted node's sibling is not null, change it to red.
            if (parent->children_[dir ^ 1] != nullptr) {
                parent->children_[dir ^ 1]->color_ = red_;
            }
            //Cases 2-5: Parent color is black.
            if (parent->color_ == black_) {
                //Case 3: If the parent is the root, then the double black doesn't matter, so do nothing.
                if (parent == root_) {
                    return;
                }
                //Case 4: The deleted node's sibling is red, so swap the colors of the sibling and parent, and do
                //a rotation in the direction of the deleted node on the parent. Then, redo the cases on the parent.
                if (parent->children_[!dir] != nullptr) {
                    bool temp_color = parent->children_[!dir]->color_;
                    parent->children_[!dir]->color_ = parent->color_;
                    parent->color_ = temp_color;
                    auto new_node = rotate(parent, dir);
                    if (new_node->parent_ == nullptr) {
                        root_ = new_node;
                    }
                    remove_check(parent, color, dir);
                }
                //Cases 5-6: The deleted node's sibling is either null or black
                else {
                    //Gets the parent direction using a conditional, avoiding the use of branching if else statements.
                    bool parent_dir = parent->parent_->children_[1] == parent;
                    auto parent_sibling = parent->parent_->children_[!parent_dir];
                    if(parent_sibling != nullptr) {
                        //Case 5: The nephew of the parent in the direction of the parent is red and the nephew in the opposite direction is
                        //black or null, then swap the color of the parent's sibling and its red sibling, and do a rotation inf the direction
                        //opposite of the parent on the parent's sibling.
                        if ((parent_sibling->children_[parent_dir] != nullptr && parent_sibling->children_[parent_dir]->color_ == red_) 
                        && (parent_sibling->children_[!parent_dir] == nullptr || parent_sibling->children_[!parent_dir]->color_ == black_)) {
                            bool temp_color = parent_sibling->color_;
                            parent_sibling->color_ = red_;
                            parent_sibling->children_[parent_dir]->color_ = temp_color;
                            auto new_node = rotate(parent_sibling, !parent_dir);
                            if (new_node->parent_ == nullptr) {
                                root_ = new_node;
                            }
                        }
                        //Case 6: The parent's sibling is black, so swap colors between the parent's sibling and the parent's parent. Then,
                        //do a rotation in the parent's direction on the deleted node's grandparent.
                        if (parent_sibling->color_ == black_) {
                            bool temp_color = parent_sibling->color_;
                            parent_sibling->color_ = parent_sibling->parent_->color_;
                            parent_sibling->parent_->color_ = temp_color;
                            rotate(parent_sibling->parent_, parent_dir);
                            if (parent_sibling->children_[!parent_dir] != nullptr){
                                parent_sibling->children_[!parent_dir]->color_ = black_;
                            }      
                        }
                    }
                    
                }
            }
        }
        //Case 2: The parent is not black, so there can't be a double black problem.
    };

    /**
     * @brief Recursively deletes the decendants of the input node.
     * 
     * @param node The node of the current recursive call, and the node to delete all of the decendants of.
     */
    void clear(node_ptr node) {
        if (node) {
            clear(node->children_[0]);
            clear(node->children_[1]);
            delete node;
        }
    };

    /**
     * @brief Recursively inserts all nodes into <destination> from the subtree starting at the first <node> and 
     * traversing to the end of each path inserting each node.
     *
     * @param node The node of the current recursive call.
     * @param destination The tree that nodes are put into.
     * 
     */
    static void merge(node_ptr node, RedBlackTree<key_type>* destination) {
        if (node){
            merge(node->children_[0],destination);
            merge(node->children_[1],destination);
            destination->insert(node);
        }
            
    }

    /**
     * @brief Recursively inserts all nodes into either <tree1> or <tree2> from the subtree starting at the first <node> and 
     * traversing to the end of each path inserting each node. <tree1> has the nodes less than <pivot>, and <tree2> has the rest.
     * 
     * @param node The targeted node of the current recursive call.
     * @param pivot The value which the tree is being split about.
     * @param tree1 The tree that contains all nodes less than the pivot.
     * @param tree2 The tree that contains all nodes greater than or equal to the pivot.
     */
    static void split(node_ptr node, key_type pivot, RedBlackTree<key_type>* tree1, RedBlackTree<key_type>* tree2) {
        if (node){
            split(node->children_[0], pivot, tree1, tree2);
            split(node->children_[1], pivot, tree1, tree2);
            if (node->key_ < pivot){
                tree1->insert(node);
            }
            else{
                tree2->insert(node);
            }
        }
    };

//Rotation
private:

    /**
     * @brief Performs a binary search tree rotation on the given node in the given direction. Returns the new root of the rotated subtree.
     * 
     * @param root The root of the subtree being rotated
     * @param side The direction of the rotation
     * @return node_ptr The new root of the rotated subtree.
     */
    node_ptr rotate(node_ptr root, bool side) {
        //The result of the rotation is shown below in ascii art.
        //The left shows the result of left rotation, and the right is the result of right rotation
        //Whichever is the original tree is up for you to decide, lol.
        //     A                      B
        //    / \                    / \
        //   B   C                  D   A
        //  / \         <-->           / \
        // D   E                      E   C
        //

        if (root->parent_ == nullptr) {
            auto parent = root->children_[side ^ 1];

            auto grandchild = root->children_[side ^ 1]->children_[side];

            root->children_[side ^ 1]->children_[side] = root;
            root->parent_ = root->children_[side ^ 1];

            if (grandchild != nullptr) {
                grandchild->parent_ = root;
            }
            root->children_[side ^ 1] = grandchild;

            root_ = root->parent_;
            root->parent_->parent_ = nullptr;

            return root_;
        }
        else {
            auto parent = root->parent_;

            
            node_ptr grandchild = nullptr;
            if(root->children_[side^1]){
                grandchild = root->children_[side ^ 1]->children_[side];
            }
            
            root->children_[side ^ 1]->children_[side] = root;
            root->parent_ = root->children_[side ^ 1];

            if (grandchild != nullptr) {
                grandchild->parent_ = root;
            }
            root->children_[side ^ 1] = grandchild;

            parent->children_[parent->children_[1] == root] = root->parent_;

            root->parent_->parent_ = parent;
            return root->parent_;
        }
    };

//Member getters
public:
    /**
     * @brief Gives the size of the tree
     * 
     * @return node_ptr The size of the tree
     */
    size_type size() {
        return size_;
    };

    /**
     * @brief Returns the lower bound of the data type of the tree.
     * 
     * @return key_type The lower bound.
     */
    static constexpr key_type upper_bound() {
        return upper_bound_;
    };

    /**
     * @brief Returns the upper bound of the data type of the tree.
     * 
     * @return key_type The upper bound.
     */
    static constexpr key_type lower_bound() {
        return lower_bound_;
    };

//Node accessors
public:
    
    /**
     * @brief Checks if tree is empty.
     * 
     * @return true if tree is empty.
     * @return false if tree has one or more nodes.
     */
    bool empty() {
        return !root_;
    }

public:
    template <typename> friend class Node;
};