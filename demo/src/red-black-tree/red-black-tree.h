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
#include "../../gui.h"

template <typename Key_>
class RedBlackTree {
    template <typename> friend class Node;

//Implicit types
public:

    using key_type = Key_;
    using some_key_type = std::optional<Key_>;

    using node_type = Node<key_type>;
    using node_ptr = node_type*;
    using size_type = size_t;

//Aliases
public:
    static constexpr bool black_ = 0;
    static constexpr bool red_ = 1;

//Contructor and destructor
public:
    bool animate_;

    void set_animate(bool animate) {
        animate_ = animate;
    }

    /**
     * @brief ruct a new RedBlackTree object
     * 
     */
	RedBlackTree() {
        root_ = nullptr;
        max_ = nullptr;
        min_ = nullptr;
        size_ = 0;
        animate_ = true;
        std::vector<node_ptr> node_path_;
    };

    /**
     * @brief Destroy the RedBlackTree object
     * 
     */
    virtual ~RedBlackTree() {
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
        CLEAN();
    };

    /**
     * @brief Remove the node that has the input key. Will do nothing if the key isn't in the tree.
     * 
     * @param key The value of the node that is to be deleted.
     */
	void remove(key_type key) {
        auto node = find(key);
        if (node == nullptr) {
            CLEAN();
            return;
        }
        if (max_ && key == max_->key_){
            min_ = nullptr;  
        }
        if (min_ && key == min_->key_){
            min_ = nullptr;            
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
                        MARK(succ);
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
        CLEAN();
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
                MARK(target_node);
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

//Visualization
public:
    void MARK(node_ptr node){
        if (animate_) {
            node_path_.push_back(node);
            UPDATE_GUI();          
        }
    }

    void CLEAN() noexcept{
        // node_path_.clear();
        // if (animate_)
        //     UPDATE_GUI();
    }

    void CLEAN_NO_UPDATE() noexcept{
        node_path_.clear();
    }

    /**
     * @brief Prints out a text based representation of the tree by calling the recursive function to do so.
     * Each node has its key, color and memory address shown.
     * 
     */
    void show(){
        show(root_,0);
    };
    
    /**
     * @brief Prints out the keys of each layer on a seperate line, from the first layer to the last layer.
     * This method will crash if not enough contiguous memory is present to store each layer in an array.
     * 
     */
    void show_layers() {
        std::vector<std::vector<node_ptr>> layers = make_2d_vector();
        
        for (std::vector<node_ptr> layer : layers) {
            for (auto node : layer) {
                std::cout << std::to_string(node->key_) << ' ';
            }
            std::cout << '\n';
        }
    };

    /**
     * @brief Generates a DOT file (a format for GraphViz) which can be used to visualize the tree.
     * 
     * @param out_fpath The full path of the created destination file, which includes the directory and the file name.
     */
    std::string to_dot()  noexcept {
        std::string text;
        if (!root_){
            text += "digraph RBTree0x00000000 {\n\n\n}";
        }
        else{
            std::stringstream sstream;
            sstream << root_;
            std::string root_ptr = sstream.str();
            sstream.str("");

            std::string colors[2] = {"black","red"};

            text += "digraph RBTree" + root_ptr + " {\n\n";
            bool do_color = false;
            for (int i = 0; i < node_path_.size(); i++){
                if (node_path_[i] == root_){
                    do_color = true;
                }
            }
            if (do_color){
                text += "\tn" + root_ptr + " [label=\"" + std::to_string(root_->key_) + "\", color=black, fillcolor=\"#90ee90\", style=filled];\n";
            }
            else{
                text += "\tn" + root_ptr + " [label=\"" + std::to_string(root_->key_) + "\", color=black, fillcolor=\"#bebebe\", style=filled];\n";
            }
            //Most of this method is done in a recursive submethod, so see that for details on the implementation.
            text = write_dot(root_,text);
            text += "\n}";
        }
        return text;        
    }

//Accessors
public:

    /**
     * @brief Returns the median key of the tree. If the tree has an even number of nodes, it gives the greater key of the two middle keys.
     * If the tree has no nodes, std::nullopt is returned.
     * 
     * @return some_key_type 
     */
    key_type median() {
        assert(root_ && "Use of median on an empty tree is not supported");
        //Gets vector with all keys
        std::vector<key_type> keys;
        for (auto node : nodes()) {
            keys.push_back(node->key_);
        }
        //Sets the "middle" of the vector to be the median
        std::nth_element(keys.begin(), keys.begin() + keys.size()/2, keys.end());

        return keys[keys.size() / 2];
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
            MARK(target_node);
            target_node = target_node->children_[key > target_node->key_];
        }
        MARK(target_node);
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
        bool has = find(key) != nullptr;
        return has;
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
        node_ptr target = root_;
        while (target->children_[key > target->key_] != nullptr) {
            MARK(target);
            target = target->children_[key > target->key_];
            //If the input key is in the tree, it used that as the target instead.
            if (target->key_ == key) {
                CLEAN();
                break;
            }
        }
        
        if (target->key_ < key){
            CLEAN();
            return some_key_type(target->key_);
        }

        auto pred = target->children_[0];

        //Case 1: Target node has left child, so find rightmost node from the subtree of the target node's left child
        if(pred){
            MARK(pred);
            while(pred->children_[1]) {
                pred = pred->children_[1];
                MARK(pred);
            }
            CLEAN();
            return some_key_type(pred->key_);
        }
        
        //Case 2: The target node has no left child, so find the parent with a value closest to but less than the value.
        auto parent = target->parent_;

        while (parent) {
            MARK(parent);
            if (target == parent->children_[1]) break;
            target = parent;
            parent = parent->parent_;
        }
        CLEAN();
        if (parent) return some_key_type(parent->key_);
    
        return std::nullopt;
    };

    /**
     * @brief Finds the inclusive predecessor of the value, which is the value closest to and less than or equal to the input value.
     * If there is no lesser or equal value, it returns nullopt.
     * 
     * @param node The value to find the inclusive predecessor of, which is less than or equal to the value.
     * @return some_key_type The inclusive predecessor of the value.
     * Returns nullopt if the teee is empty, or there is no value lesser than or equal to the input value.
     * 
     */
    some_key_type inclusive_predecessor(key_type key) {
        
        if(!root_) return std::nullopt;

        //Finds the parent of the key if it were to be inserted into the tree.
        //The predecessor will be the same for this target node as the key.
        auto target = root_;
        while (target->children_[key > target->key_] != nullptr) {
            target = target->children_[key > target->key_];
            //If the input key is in the tree, it used that as the target instead.
            if (target->key_ == key) break;
        }
        
        if (target->key_ <= key){
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
            MARK(target);
            target = target->children_[key > target->key_];
            //If the input key is in the tree, it used that as the target instead.
            if (target->key_ == key) {
                CLEAN();
                break;
            }
        }
        
        if (target->key_ > key){
            CLEAN();
            return some_key_type(target->key_);
        }

        auto pred = target->children_[1];

        //Case 1: Target node has right child, so find leftmost node from the subtree of the target node's right child
        if(pred){
            MARK(pred);
            while(pred->children_[0]) {
                pred = pred->children_[0];
                MARK(pred);
            }
            CLEAN();
            return some_key_type(pred->key_);
        }
        
        //Case 2: The target node has no right child, so find the parent with a value closest to but greater than the value.
        auto parent = target->parent_;

        while (parent) {
            MARK(pred);
            if (target == parent->children_[0]) break;
            target = parent;
            parent = parent->parent_;
        }

        CLEAN();

        if (parent) return some_key_type(parent->key_);
    
        return std::nullopt;
    };

    /**
     * @brief Finds the inclusive successor of the value, which is the value closest to and greater than the input value.
     * If there is no greater than or equal to the value, returns nullopt.
     * 
     * @param node The value to find the inclusive successor of.
     * @return some_key_type The inclusive succecessor of the value, which might be itself.
     * Returns nullopt if the tree is empty, or there is no value greater than or equal to the input value.
     * 
     */
    some_key_type inclusive_successor(key_type key) {
        
        if(!root_) return std::nullopt;

        //Finds the parent of the key if it were to be inserted into the tree.
        //The successor will be the same for this target node as the key.
        auto target = root_;
        while (target->children_[key > target->key_] != nullptr) {
            target = target->children_[key > target->key_];
            //If the input key is in the tree, it used that as the target instead.
            if (target->key_ == key) break;
        }
        
        if (target->key_ >= key){
            return some_key_type(target->key_);
        }

        auto pred = target->children_[1];

        //Case 1: Target node has right child, so find leftmost node from the subtree of the target node's right child
        if(pred){
            while(pred->children_[0]) {
                pred = pred->children_[0];
            }
            return some_key_type(pred->key_);
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
     * @author Calvin Higgins
     * 
     * @return some_key_type The maximum of the tree, which may be none if the tree is empty.
     */
    some_key_type max() {
        if (max_) return some_key_type(max_->key());
        if (!root_) return some_key_type();
        auto max_ = root_;
        while (max_->children_[1]) max_ = max_->children_[1];
        return some_key_type(max_->key());
    }  

    /**
     * @brief Returns the minimum value of the tree. Returns nullopt if the tree is empty
     * 
     * @author Calvin Higgins
     * 
     * @return some_key_type The minimum of the tree, which may be none if the tree is empty.
     */
    some_key_type min() {
        if (min_) return some_key_type(min_->key());
        if (!root_) return some_key_type();
        auto min_ = root_;
        while (min_->children_[0]) min_ = min_->children_[0];
        return some_key_type(min_->key());
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
                MARK(succ);
                succ = succ->children_[0];
            }
        }
        
        return succ;
    };

//Split/merge
public:

    /**
     * @brief Split the current tree into two new trees.
     * 
     * @author Calvin Higgins (calvin_higgins2@uri.edu)
     * 
     * @return left tree with values less than the median and a right tree with all other values.
     */
    std::array<RedBlackTree<key_type>*,2>  split() {
        assert(root_ && (size() > 1) && 
               "Cannot split a tree with size 0 or 1");

        // Collect all the nodes in the tree into a vector.
        std::vector<node_ptr> tree_nodes;
        tree_nodes.reserve(size());
        nodes(root_, tree_nodes);

        // Split the nodes into two new vectors.
        auto mid = tree_nodes.size() / 2;
        std::vector<node_ptr> left_tree_nodes(tree_nodes.begin(), tree_nodes.begin() + mid);
        std::vector<node_ptr> right_tree_nodes(tree_nodes.begin() + mid, tree_nodes.end());

        // Create the new trees from the node vectors.
        auto left_tree = nodes_to_balanced_tree(left_tree_nodes);
        auto right_tree = nodes_to_balanced_tree(right_tree_nodes);
        assert(left_tree->max().value() < right_tree->min().value());

        // Ensure that the current tree does not have any associated data.
        root_ = nullptr;
        max_ = nullptr;
        min_ = nullptr;
        size_ = 0;

        return std::array<RedBlackTree<key_type>*, 2>{left_tree, right_tree};
    };

    /**
     * @brief Merge two red-black trees into an single red-black tree.
     * 
     * @author Calvin Higgins (calvin_higgins2@uri.edu)
     * 
     * @param left_tree with values strictly less than the right tree.
     * @param right_tree with values strictly greater than the left tree.
     * @return the merged tree.
     */
    RedBlackTree<key_type>* merge(RedBlackTree<key_type>* left_tree, RedBlackTree<key_type>* right_tree) {
        assert(left_tree->root_ && right_tree->root_ && (left_tree->size() > 0) && (right_tree->size() > 0) &&
               "Cannot merge empty trees.");
        assert(left_tree->max().value() < right_tree->min().value() &&
               "The left tree must contain values strictly less than the right tree.");

        // Collect all the nodes into a vector.
        std::vector<node_ptr> tree_nodes;
        tree_nodes.reserve(left_tree->size() + right_tree->size());
        //bug must be sorted
        nodes(left_tree->root_, tree_nodes);
        nodes(right_tree->root_, tree_nodes);

        // Create a new tree from the nodes.
        auto merged_tree = nodes_to_balanced_tree(tree_nodes);

        // Ensure that the current trees do not have any associated data.
        left_tree->root_ = nullptr;
        left_tree->max_ = nullptr;
        left_tree->min_ = nullptr;
        left_tree->size_ = 0;

        right_tree->root_ = nullptr;
        right_tree->max_ = nullptr;
        right_tree->min_ = nullptr;
        right_tree->size_ = 0;

        return merged_tree;
    };

private:
    /**
     * @brief Convert a sorted vector of nodes to a balanced red-black tree.
     * 
     * @author Calvin Higgins (calvin_higgins2@uri.edu)
     * 
     * @param tree_nodes to construct a red-black tree from.
     * @return the red black tree.
     */
    RedBlackTree<key_type>* nodes_to_balanced_tree(std::vector<node_ptr>& tree_nodes) {
        assert(!tree_nodes.empty() && "Cannot build a tree from 0 nodes.");

        // Create a vector to store nodes that should be marked red.
        std::vector<node_ptr> mark_red;
        mark_red.reserve(tree_nodes.size());

        // Construct a balanced tree without correct coloring.
        int max_depth = 0;
        auto merged_root = construct_all_black_balanced_tree(tree_nodes, mark_red, nullptr, 0, tree_nodes.size() - 1, 0, max_depth);

        // Correct the coloring.
        for (auto node : mark_red)
            node->color_ = red_;
        
        // Initialize the merged tree.
        auto merged_tree = new RedBlackTree<key_type>();
        merged_tree->root_ = merged_root;
        merged_tree->size_ = tree_nodes.size();
        merged_tree->min_ = *tree_nodes.begin();
        merged_tree->max_ = *(--tree_nodes.end());
        
        // Check some post conditions.
        assert(!merged_tree->empty());
        assert(merged_tree->max().value() == merged_tree->max_->key_);
        assert(merged_tree->min().value() == merged_tree->min_->key_);

        return merged_tree;
    }  

    /**
     * @brief Construct an all black balanced binary tree from a vector of nodes.
     * 
     * @author Calvin Higgins (calvin_higgins2@uri.edu)
     * 
     * @param tree_nodes is the nodes to construct the tree from.
     * @param mark_red is will contain all nodes that should be colored red.
     * @param parent is the parent of the current node.
     * @param start is the left bound of tree_nodes.
     * @param end is the right bound of tree_nodes.
     * @param depth is the current depth.
     * @param max_depth is the maximum depth seen so far.
     * @return node_ptr to an all black balanced tree.
     */
    node_ptr construct_all_black_balanced_tree(std::vector<node_ptr>& tree_nodes, std::vector<node_ptr>& mark_red, node_ptr parent, int start, int end, int depth, int& max_depth) {

        // Compute the size of the slice.
        auto size = end - start + 1;

        // If the slice does not exist, return nullptr.
        if (size <= 0)
            return nullptr;
        
        // Check that the slice is valid.
        assert(start >= 0 && start < tree_nodes.size());
        assert(end >= 0 && end < tree_nodes.size());
        
        // Compute the current root.
        auto mid = start + (size / 2);
        auto root = tree_nodes.at(mid);

        // Since we need to mark only the deepest nodes red, keep track of the deepest level and
        // flag any nodes red that are on the deepest current level.
        if (depth > max_depth) {
            max_depth = depth;
            mark_red.clear();
            mark_red.push_back(root);
        }
        else if (depth == max_depth) {
            mark_red.push_back(root);
        }

        // Compute the left and right subtrees from the left and right sections of the node vector.
        root->children_[0] = construct_all_black_balanced_tree(tree_nodes, mark_red, root, start, mid - 1, depth + 1, max_depth);
        root->children_[1] = construct_all_black_balanced_tree(tree_nodes, mark_red, root, mid + 1, end, depth + 1, max_depth);

        // Update the parent and color of the current node.
        root->parent_ = parent;
        root->color_ = black_;

        return root;   
    }


//Data members
private:
    node_ptr root_;
	size_type size_;
    node_ptr max_;
    node_ptr min_;
    std::vector<node_ptr> node_path_;

//Class ants
private:
    //The maximum value for the type that Key_ is using.
    static constexpr key_type upper_bound_ = std::numeric_limits<key_type>::max();

    //The minimum value for the type that Key_ is using.
	static constexpr key_type lower_bound_ = std::numeric_limits<key_type>::min();

//Recursive submethods
public:

    /**
     * @brief Recursively prints out a text based representation of the tree. Each node has its key, color and memory address shown.
     * 
     * @param node The node of the current recursive call
     * @param depth The layer of the node of the current recursive call.
     */
    void show(node_ptr node, int depth = 0) {
        std::string colors[2] = { "Black","Red"};
        std::string lines = "";
        for (int i = 0; i < depth; i++) {
            lines += "--";
        }
        if (node) {
            //Format: (layer) + (lines based on depth) + (color) + (key) + (memory address)
            std::cout << std::to_string(depth) << lines << ' ' << colors[node->color_] << ' ' << std::to_string(node->key_) << ' ' << node << '\n';
            show(node->left(),depth + 1);
            show(node->right(),depth + 1);
        }
        else{
            std::cout << std::to_string(depth) << lines << " Null\n";
        }
        
        
    };

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
                if (parent->children_[!dir] != nullptr && parent->children_[!dir]->color_ == red_) {
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
     * @brief Recursively generates the text for the DOT file
     * 
     * @param node The node of the current recursive call.
     * @param text The return text as it is during the current recursive call.
     * @return std::string 
     */
    std::string write_dot(node_ptr node, std::string text = "")  noexcept {
        if (node){
            //Gets the text representation of the node's memory address
            std::stringstream sstream;
            sstream << node;
            //The id of each node is the memory address preceeded by 'n'.
            std::string node_id = 'n' + sstream.str();
            sstream.str("");

            //For converting color booleans to color words.
            std::string colors[2] = {"black","red"};
            std::string fill_colors[3] = {"\"#bebebe\"","\"#ff6a6a\"","\"#90ee90\""};

            if (node->children_[0]||node->children_[1]){
                //Director is the string that represents the relationship to the node's children.
                std::string director = "";
                std::string left_id = "";
                std::string right_id = "";
                char left_color,right_color;
                
                //If there's a left child, make the definition for the left child, and add it to the parent's relationship.
                if(node->children_[0]) {
                    
                    sstream << node->children_[0];
                    left_id = 'n' + sstream.str();
                    sstream.str("");

                    left_color = node->children_[0]->color_;
                    for (int i = 0; i < node_path_.size(); i++){
                        if (node_path_[i] == node->children_[0]){
                            left_color = 2;
                        }
                    }

                    //Format: (tab) (left node id) [label="(left node's key)", color=(left node's color)];

                    text += '\t' + left_id + " [label=\"" + std::to_string(node->children_[0]->key_)  + "\", color=" + colors[node->children_[0]->color_] +  ", fillcolor=" + fill_colors[left_color] + ", style=filled];\n";
                    director += " -> {" + left_id;

                    //If there's also a right child, define it and add it to the parent's relationship as well.
                    if (node->children_[1]){
                        sstream << node->children_[1];
                        right_id = 'n' + sstream.str();
                        sstream.str("");

                        right_color = node->children_[1]->color_;
                        for (int i = 0; i < node_path_.size(); i++){
                            if (node_path_[i] == node->children_[1]){
                                right_color = 2;
                            }
                        }
                        
                        //Format: (tab) (right node id) [label="(right node's key)", color=(right node's color)];
                        text += '\t' + right_id + " [label=\"" + std::to_string(node->children_[1]->key_) + "\", color=" + colors[node->children_[1]->color_] + ", fillcolor=" + fill_colors[right_color] + ", style=filled];\n";
                        director += ", " + right_id;
                    }

                }
                //If there's only a right child define it and add it to the parent's relationship.
                else{
                    sstream << node->children_[1];
                    right_id = 'n' + sstream.str();
                    sstream.str("");

                    right_color = node->children_[1]->color_;
                    for (int i = 0; i < node_path_.size(); i++){
                        if (node_path_[i] == node->children_[1]){
                            right_color = 2;
                        }
                    }

                    //Format: (tab) (right node id) [label="(right node's key)", color=(right node's color)];
                    text += '\t' + right_id + " [label=\"" + std::to_string(node->children_[1]->key_) + "\", color=" + colors[node->children_[1]->color_] + ", fillcolor=" + fill_colors[right_color] + ", style=filled];\n";
                    director += " -> {"+ right_id;
                }
                text += '\t' + node_id + director + "};\n";
            }
            //Do the same process for each child
            if(node->children_[0]) {
                text = write_dot(node->children_[0],text);
            }
            if(node->children_[1]) {
                text = write_dot(node->children_[1],text);
            }
            
        }
        return text;
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
     * @brief Gives the root of the tree
     * 
     * @return node_ptr The root of the tree
     */
    node_ptr root() {
        return root_;
    };

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
    static constexpr key_type upper_bound(){
        return upper_bound_;
    };

    /**
     * @brief Returns the upper bound of the data type of the tree.
     * 
     * @return key_type The upper bound.
     */
    static constexpr key_type lower_bound(){
        return lower_bound_;
    };

    

//Member setters
public:
    /**
     * @brief Sets the root of the tree. Clears the tree if a root alredy exists.
     * 
     * @param node The new root of the tree.
     */
    void root(node_ptr node) {
        if (root_){
            clear();
        }
        root_ = node;

    };

//Node accessors
public:

    /**
     * @brief Tries to make a vector with the nodes of a given layer of the tree. If the input layer is too large, 
     * the last layer is returned. If the vector can't be made, it will return an empty vector and report the error.
     * 
     * @param layer The layer to get the nodes from.
     * @return std::vector<node_ptr> The vector of nodes of the layer. Empty if the allocation fails.
     */
    std::vector<node_ptr> get_layer(int layer) {
        try{
            std::vector<node_ptr> layer1({root_});
            std::vector<node_ptr> layer2;
            //Puts the children of the nodes from the first layer into the second layer, and the process is repeated using
            //The second layer as the first layer. This stops when there are no more children or if the target layer is reached.
            for (int layer_num = 0; layer_num < layer; layer++) {
                
                for (auto node: layer1) {
                    if (node->children_[0] != nullptr) {
                        layer2.push_back(node->children_[0]);
                    }
                    if (node->children_[1] != nullptr) {
                        layer2.push_back(node->children_[1]);
                    }
                }
                if (layer2.size() == 0){
                    return layer1;
                }
                layer1 = layer2; 
            }

            return layer2;
        } catch (...) {
            std::cout << "Sorry, but there were too many nodes to be stored into an array. Returning an empty vector";
            return (std::vector<node_ptr>());
        }
        
    };

    /**
     * @brief Tries to put all of the nodes from the tree into a vector in preorder order. Returns an empty vector upon failure, and
     * states that an error has occured.
     * 
     * @return std::vector<node_ptr> The vector of the tree's nodes. Empty if the allocation fails.
     */
    std::vector<node_ptr> nodes() {
        try{
            std::vector<node_ptr> node_vector = std::vector<node_ptr>();
            nodes(root_,&node_vector);
            return node_vector;
        } catch (...) {
            std::cout << "Sorry, but there were too many nodes to be stored into an array. Returning an empty vector";
            return (std::vector<node_ptr>());
        }
        
    };

    /**
     * @brief Tries to store the vector representation of each of the tree's layers in a 2d vetcor. Will report the failure of this,
     * and return an empty 2D vector.
     * 
     * @return std::vector<node_ptr> The 2D vector of the tree's node layers. Empty if the allocation fails.
     */
    std::vector<std::vector<node_ptr>> make_2d_vector() {
        try{
            std::vector<std::vector<node_ptr>> layers;
            layers.push_back(std::vector<node_ptr>({ root_ }));
            unsigned int layer = 0;
            while (layers[layer].size() > 0) {
                std::vector<node_ptr> new_layer;
                new_layer.reserve(1 << layer);
                for (auto node : layers[layer]) {
                    if (node->left() != nullptr) {
                        new_layer.push_back(node->left());
                    }
                    if (node->right() != nullptr) {
                        new_layer.push_back(node->right());
                    }

                }
                layers.push_back(new_layer);
                layer += 1;
            }
            layers.pop_back();
            return layers;
        } catch (...) {
            std::cout << "Sorry, but there were too many nodes to be stored into an array. Returning an empty 2D vector";
            return (std::vector<std::vector<node_ptr>>());
        }
        
    };

//Debugging
public:
    /**
     * @brief Prints true if there are no consequetive red nodes, and false if there are.
     * 
     * @return true if there are no consequetive red nodes.
     * @return false if there are consequetive red nodes.
     */
    bool no_consecutive_reds() {
        for (auto node : nodes()){
            if ( node->color_ == red_){
                if(node->parent_->color_ == red_ ||
                (node->children_[0] && node->children_[0]->color_ == red_) ||
                (node->children_[1] && node->children_[1]->color_ == red_)
                )
                {
                    return false;
                }
            }
        }
        return true;
    };
    
    /**
     * @brief Checks if tree is empty.
     * 
     * @return true if tree is empty.
     * @return false if tree has one or more nodes.
     */
    bool empty(){
        return !root_;
    }

//Tree attribute accessors
public:
    /**
     * @brief Gets the height of the subtree about the input node, which is the number of nodes in the longest path of that subtree.
     * 
     * @param node The root of the subtree to find the height of.
     * @return int The height of the subtree.
     */
    int height(node_ptr node) {
        //If theres a left child...
        if (node->children_[0]){
            //Keep finding the height of the left path
            int height1 = height(node->children_[0]);
            //If there is both a left and right child, also keep finding the right path's height and return the height of the longest path.
            if (node->children_[1]){
                //Keep finding the height of the right path
                int height2 = height(node->children_[1]);
                if (height2 > height1) {
                    return height2 + 1;
                }
                return height1 + 1;
            }
            //If there's only a left child, return the left paths's height.
            else {
                return height1 + 1;
            }
        }
        //If there's only a right child, keep finding the right path's height.
        else if (node->children_[1]) {
            return height(node->children_[1]) + 1;
        }
        //Base case.
        else {
            return 1;
        }
    };
    
    /**
     * @brief Returns the upper limit of the unsigned type of the tree's keys, which can be represented as -1;
     * 
     * @return key_type The upper limit of the unsigned type of the tree's keys (-1).
     */
    key_type limit() {return -1;};
};