//red-black-tree.h
#pragma once
#include "red-black-tree-node.h"
#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <optional>
#include <utility>

template <typename T>
class RedBlackTree {
    friend Node<T>;
public:
	RedBlackTree();
    ~RedBlackTree();
	Node<T>* rotate(Node<T>* root, bool side);
	Node<T>* left_rotate(Node<T>* root);
	Node<T>* right_rotate(Node<T>* root);
    Node<T>* find(T key);
    Node<T>* successor_node(T key);
    Node<T>* successor_node(Node<T>* node);
    Node<T>* predecessor_node(T key);
    Node<T>* predecessor_node(Node<T>* node);

    std::optional<T> predecessor(T key);
	std::optional<T> successor(T key);

    RedBlackTree<T>* merge(RedBlackTree<T>* tree1, RedBlackTree<T>* tree2);

    std::vector<RedBlackTree<T>*> split(T key);
    std::pair<RedBlackTree<T>,RedBlackTree<T>> bisect();

    std::vector<Node<T>*> get_layer(int layer);

    std::vector<Node<T>*> nodes();

    std::vector<std::vector<Node<T>*>> make_2d_vector();

	void recolor_tree();
	void insert_check(Node<T>* node);
	void insert(T key);
    void insert(Node<T>* node);
	void remove_check(Node<T>* parent, bool color, bool dir);
	void remove(T key);
    void clear();
    void show(Node<T>* node, int depth = 0);
    void show();
    void show_layers();
    void merge(RedBlackTree<T>* tree2);

    bool contains(T key);
    bool correct_black_height();
    bool no_consecutive_reds();

    int size();
    int height() {return std::ceil(std::log2(size_));};
    
    T limit() {return -1;};
    std::optional<T> max();
    std::optional<T> min();
    T median();

    Node<T>* root();
    void root(Node<T>* node);

    
private:
    Node<T>* root_;
	int size_;

};

template <typename T> RedBlackTree<T>::RedBlackTree() {
    root_ = nullptr;
    size_ = 0;
};

template <typename T> RedBlackTree<T>::~RedBlackTree() {
    clear();
};

template <typename T> Node<T>* RedBlackTree<T>::rotate(Node<T>* root, bool side) {
    if (root->parent_ == nullptr) {
        Node<T>* parent = root->children_[side ^ 1];

        Node<T>* grandchild = root->children_[side ^ 1]->children_[side];

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
        Node<T>* parent = root->parent_;

        Node<T>* grandchild = root->children_[side ^ 1]->children_[side];

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

template <typename T> Node<T>* RedBlackTree<T>::left_rotate(Node<T>* root) {
    return rotate(root, 0);
};

template <typename T> Node<T>* RedBlackTree<T>::right_rotate(Node<T>* root) {
    return rotate(root, 1);
};

template <typename T> void RedBlackTree<T>::recolor_tree() {
    std::vector<std::vector<Node<T>*>>layers;
    layers.push_back(std::vector<Node<T>*>({ root_ }));
    unsigned int layer = 0;
    while (layers[layer].size() > 0) {
        std::vector<Node<T>*> new_layer;
        new_layer.reserve(1 << layer);
        for (Node<T>* node : layers[layer]) {

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

    for (int i = 0; i < layers.size() - 1; i++) {
        for (Node<T>* node : layers[i]) {
            node->color_ = 0;
        }
    }
    for (Node<T>* node : layers.back()) {
        node->color_ = 1;
    }
};

template <typename T> void RedBlackTree<T>::clear() {
    for (Node<T>* node : nodes()) {
        delete node;
    }
    root_ = nullptr;
    size_ = 0;

    /*
    std::vector<Node<T>*> layer1{root_};
    std::vector<Node<T>*> layer2;
    int width = 1;
    while (width < size_){
        width *= 2;
        layer2.reserve(width);
        
        for (Node<T>* node: layer1) {
            if (node->children_[0] != nullptr) {
                layer2.push_back(node->children_[0]);
            }
            if (node->children_[1] != nullptr) {
                layer2.push_back(node->children_[1]);
            }
            delete node;
        }
        layer1 = layer2; 
    }
    for (Node<T>* node: layer1) {
        delete node;
    }
    */
};

template <typename T> void RedBlackTree<T>::insert_check(Node<T>* node) {
    Node<T>* parent = node->parent_;
    if (parent == nullptr || !parent->color_) {
        return;
    }
    else {
        bool parent_dir = parent->parent_->children_[1] == parent;
        bool target_dir = parent->children_[1] == node;
        Node<T>* sibling = parent->parent_->children_[parent_dir ^ 1];
        bool black_sibling = (sibling == nullptr || sibling->color_ == 0);

        if (black_sibling) {
            Node<T>* first_rotate_targets[2] = {parent, parent->parent_};

            Node<T>* target_node = rotate(first_rotate_targets[parent_dir == target_dir], target_dir ^ 1);
            if (parent_dir != target_dir) {
                parent = node->parent_;
                target_node = rotate(parent, target_dir);
            }
            bool color = target_node->color_;
            target_node->color_ = color^1;
            if (target_node->children_[0] != nullptr) {
                target_node->children_[0]->color_ = color;
            }
            if (target_node->children_[1] != nullptr) {
                target_node->children_[1]->color_ = color;
            }
        }
        else {
            if (parent->parent_->children_[0] != nullptr) {
                parent->parent_->children_[0]->color_ = parent->parent_->children_[0]->color_ ^ 1;
            }
            if (parent->parent_->children_[1] != nullptr) {
                parent->parent_->children_[1]->color_ = parent->parent_->children_[1]->color_ ^ 1;
            }
            if (parent->parent_ != root_) {
                parent->parent_->color_ ^= 1;
                insert_check(parent->parent_);
                if (root_->color_ == 1) {
                    recolor_tree();
                }
            }
        }
    }
};

template <typename T> void RedBlackTree<T>::insert(T key) {
    if (root_ == nullptr) {
        root_ = new Node<T>(key, 0);
        root_->parent_ = nullptr;
        size_++;
    }
    else {
        Node<T>* target_node = root_;
        while (target_node != nullptr && target_node->key_ != key) {
            bool dir = key > target_node->key_;
            if (target_node->children_[dir] == nullptr) {
                target_node->children_[dir] = new Node<T>(key, 1, nullptr, nullptr, target_node);
                size_++;
                insert_check(target_node->children_[dir]);
                return;
            }
            else {
                target_node = target_node->children_[dir];
            }
        }
    }
};

template <typename T> void RedBlackTree<T>::insert(Node<T>* node) {
    if (node == nullptr) {
        return;
    }
    node->children_[0] = nullptr;
    node->children_[1] = nullptr;
    node->parent_ = nullptr;

    if (root_ == nullptr) {
        root_ = node;
        root_->color_ = 0;
        size_++;
    }
    else {
        node->color_ = 1;
        T key = node->key_;
        Node<T>* target_node = root_;
        while (target_node != nullptr && target_node->key_ != key) {
            bool dir = key > target_node->key_;
            if (target_node->children_[dir] == nullptr) {
                target_node->children_[dir] = node;
                node->parent_ = target_node;
                size_++;
                insert_check(node);
                return;
            }
            else {
                target_node = target_node->children_[dir];
            }
        }
    }
};

template <typename T> Node<T>* RedBlackTree<T>::find(T key) {
    Node<T>* target_node = root_;
    while (target_node != nullptr && target_node->key_ != key) {
        target_node = target_node->children_[key > target_node->key_];
    }
    return target_node;
};

template <typename T> std::optional<T> RedBlackTree<T>::max() {
    if (root_ == nullptr) return std::nullopt;
    Node<T>* target_node = root_;
    while (target_node != nullptr && target_node->children_[1] != nullptr) {
        target_node = target_node->children_[1];
    }
    return std::optional<T>(target_node->key_);
};

template <typename T> std::optional<T> RedBlackTree<T>::min() {
    if (root_ == nullptr) return std::nullopt;
    Node<T>* target_node = root_;
    while (target_node != nullptr && target_node->children_[0] != nullptr) {
        target_node = target_node->children_[0];
    }
    return std::optional<T>(target_node->key_);
};

template <typename T> Node<T>* RedBlackTree<T>::successor_node(T key) {
    Node<T>* node = find(key);
    if (node == nullptr) {
        auto succ = root_;
        auto cur = root_;
        while (cur) {
            if (key < cur->key_ ) {
                succ = cur;
                cur = cur->children_[0];
            } else {
                cur = cur ->children_[1];
            }
        }
        return succ;
    }
    return successor_node(node);
};

template <typename T> Node<T>* RedBlackTree<T>::successor_node(Node<T>* node) {
    if (node == nullptr) return nullptr;

    auto succ = node->children_[1];
    if (succ) {
        while (succ->children_[0] != nullptr) {
            succ = succ->children_[0];
        }
        return succ;
    }

    auto parent = node->parent_;
    while (parent) {
        if (node != parent->children_[1]) break;
        node = parent;
        parent = parent->parent_;
    }

    return parent;
};

template <typename T> Node<T>* RedBlackTree<T>::predecessor_node(T key) {
    Node<T>* node = find(key);
    if (node == nullptr) {
        auto pred = root_;
        auto cur = root_;
        while (cur) {
            if (key < cur->key_ ) {
                cur = cur->children_[0];
            } else {
                pred = cur;
                cur = cur ->children_[1];
            }
        }
        return pred;
    }
    return predecessor_node(node);
};

template <typename T> Node<T>* RedBlackTree<T>::predecessor_node(Node<T>* node) {
    if (node == nullptr) return nullptr;

    auto pred = node->children_[0];
    if (pred) {
        while (pred->children_[1] != nullptr) {
            pred = pred->children_[1];
        }
        return pred;
    }

    auto parent = node->parent_;
    while (parent) {
        if (node != parent->children_[0]) break;
        node = parent;
        parent = parent->parent_;
    }

    return parent;
};

template <typename T> std::optional<T> RedBlackTree<T>::predecessor(T key) {
	Node<T>* node = predecessor_node(key);
	if (node) return std::optional<T>(node->key_);
	return std::nullopt;
};

template <typename T> std::optional<T> RedBlackTree<T>::successor(T key) {
	Node<T>* node = successor_node(key);
	if (node) return std::optional<T>(node->key_);
	return std::nullopt;
};

template <typename T> bool RedBlackTree<T>::contains(T key) {
    return find(key) != nullptr;
};

template <typename T> void RedBlackTree<T>::remove(T key) {
    Node<T>* node = find(key);
    bool dir;
    Node<T>* parent = nullptr;
    bool color;
    if (node == nullptr) {
        return;
    }
    if (node != root_) {
        dir = node->parent_->children_[1] == node;
        parent = node->parent_;
        bool child_dir = parent->children_[1] == node;
        if (node->children_[0] == nullptr) {
            if (node->children_[1] == nullptr) {
                parent->children_[child_dir] = nullptr;
                color = node->color_;
                delete node;
            }
            else {
                parent->children_[child_dir] = node->children_[1];
                node->children_[1]->parent_ = parent;
                color = node->color_;
                delete node;
            }

        }
        else {
            if (node->children_[1] == nullptr) {
                parent->children_[child_dir] = node->children_[0];
                node->children_[0]->parent_ = parent;
                color = node->color_;
                delete node;
            }
            else {
                Node<T>* succ = node->children_[1];
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
        remove_check(parent, color, dir);
    }
    else {
        Node<T>* succ = successor_node(node);
        if (succ == nullptr) {
            root_ = node->children_[0];
            if (root_ != nullptr) {
                root_->parent_ = nullptr;
                root_->color_ = 0;
            }
            parent = node->parent_;
            color = node->color_;
            delete node;
        }
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
            remove_check(parent, color, dir);
        }
    }
    size_--;
};

template <typename T> void RedBlackTree<T>::remove_check(Node<T>* parent, bool color, bool dir) {
    if (color == 0) {
        if (parent->children_[dir ^ 1] != nullptr) {
            parent->children_[dir ^ 1]->color_ = 1;
        }
        if (parent->color_ == 0) {
            if (parent == root_) {
                return;
            }
            if (parent->children_[!dir] != nullptr && parent->children_[!dir]->color_ == 1) {
                bool temp_color = parent->children_[!dir]->color_;
                parent->children_[!dir]->color_ = parent->color_;
                parent->color_ = temp_color;
                Node<T>* new_node = rotate(parent, dir);
                if (new_node->parent_ == nullptr) {
                    root_ = new_node;
                }
                remove_check(parent, color, dir);
            }
            else {
                bool parent_dir = parent->parent_->children_[1] == parent;
                Node<T>* parent_sibling = parent->parent_->children_[!parent_dir];
                if(parent_sibling != nullptr) {
                    if ((parent_sibling->children_[parent_dir] != nullptr && parent_sibling->children_[parent_dir]->color_ == 1) 
                    && (parent_sibling->children_[!parent_dir] == nullptr || parent_sibling->children_[!parent_dir]->color_ == 0)) {
                        bool temp_color = parent_sibling->color_;
                        parent_sibling->color_ = parent_sibling->children_[parent_dir]->color_;
                        parent_sibling->children_[parent_dir]->color_ = temp_color;
                        Node<T>* new_node = rotate(parent_sibling, !parent_dir);
                        if (new_node->parent_ == nullptr) {
                            root_ = new_node;
                        }
                    }
                    if (parent_sibling->color_ == 0) {
                        bool temp_color = parent_sibling->color_;
                        parent_sibling->color_ = parent_sibling->parent_->color_;
                        parent_sibling->parent_->color_ = temp_color;
                        rotate(parent_sibling->parent_, parent_dir);
                        if (parent_sibling->children_[!parent_dir] != nullptr){
                            parent_sibling->children_[!parent_dir]->color_ = 0;
                        }
                        
                    }
                }
                
            }

        }
        else {
            parent->color_ = 0;
        }
    }
};

template <typename T> void RedBlackTree<T>::show(Node<T>* node, int depth) {
    std::string colors[2] = { "Black","Red"};
    std::string lines = "";
    for (int i = 0; i < depth; i++) {
        lines += "--";
    }
    if (node) {
        std::cout << std::to_string(depth) << lines << ' ' << colors[node->color_] << ' ' << std::to_string(node->key_) << '\n';
        show(node->left(),depth + 1);
        show(node->right(),depth + 1);
    }
    
    
};

template <typename T> void RedBlackTree<T>::show() {
    show(root_);
};

template <typename T> void RedBlackTree<T>::root(Node<T>* node) {
    root_ = node;
};

template <typename T> Node<T>* RedBlackTree<T>::root() {
    return root_;
};

template <typename T> int RedBlackTree<T>::size() {
    return size_;
};

template <typename T> std::vector<std::vector<Node<T>*>> RedBlackTree<T>::make_2d_vector() {
    std::vector<std::vector<Node<T>*>>layers;
    layers.push_back(std::vector<Node<T>*>({ root_ }));
    unsigned int layer = 0;
    while (layers[layer].size() > 0) {
        std::vector<Node<T>*> new_layer;
        new_layer.reserve(1 << layer);
        for (Node<T>* node : layers[layer]) {
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
};

template <typename T> std::vector<Node<T>*> RedBlackTree<T>::nodes() {
    std::vector<Node<T>*> current_nodes{root_};
    std::vector<Node<T>*> nodes(1);
    nodes[0] = root_;

    unsigned int layer = 1;
    while (nodes.size() < size_) {
        std::vector<Node<T>*> new_layer;
        new_layer.reserve(1 << layer);
        for (Node<T>* node : current_nodes) {
            if (node == nullptr) continue;
            if (node->left() != nullptr) {
                new_layer.push_back(node->left());
            }
            if (node->right() != nullptr) {
                new_layer.push_back(node->right());
            }

        }
        current_nodes = new_layer;

        for (Node<T>* node : new_layer) {
            nodes.push_back(node);
        }
        layer += 1;
    }
    return nodes;
};

template <typename T>
T RedBlackTree<T>::median() {
    std::vector<T> vec;
    for (auto node : this->nodes()) {
        vec.push_back(node->key_);
    }
    std::sort(std::begin(vec), std::end(vec));
    return vec[vec.size() / 2];
}

template <typename T> std::vector<Node<T>*> RedBlackTree<T>::get_layer(int layer) {
    std::vector<Node<T>*> layer1({root_});
    std::vector<Node<T>*> layer2();
    for (int layer_num = 0; layer_num < layer; layer++) {
        layer2.reserve(1 << (layer_num + 1));
        
        for (Node<T>* node: layer1) {
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
};

//Returns two trees that have a size difference of at most 1 from the nodes of the tree.
template <typename T> std::pair<RedBlackTree<T>,RedBlackTree<T>>  RedBlackTree<T>::bisect() {
    RedBlackTree<T> tree1 = RedBlackTree<T>();
    RedBlackTree<T> tree2 = RedBlackTree<T>();
    std::vector<Node<T>*> nodes = nodes();
    
    for (int i = 0; i < nodes.size()/2; i++) {
        tree1->insert(nodes[i]);
    }

    for (int i = nodes.size()/2; i < nodes.size(); i++) {
        tree2->insert(nodes[i]);
    }

    return std::pair<RedBlackTree<T>,RedBlackTree<T>>(tree1, tree2); 
};

//The first tree that is returned is all nodes less than <key>, and the other tree is all nodes greater than it.
template <typename T> std::vector<RedBlackTree<T>*>  RedBlackTree<T>::split(T key) {
    RedBlackTree<T>* tree1 = new RedBlackTree<T>();
    RedBlackTree<T>* tree2 = new RedBlackTree<T>();
    
    for (Node<T>* node : nodes()) {
        if (node->key_ <= key){
            tree1->insert(node);
        }
        else {
            tree2->insert(node);
        }
    }

    root_ = nullptr;
    return std::vector{tree1, tree2}; 
};

template <typename T> RedBlackTree<T>* RedBlackTree<T>::merge(RedBlackTree<T>* tree1, RedBlackTree<T>* tree2) {
    std::vector<Node<T>*> nodes1 = tree1->nodes();
    std::vector<Node<T>*> nodes2 = tree2->nodes();
    RedBlackTree<T>* merged = new RedBlackTree();

    for (Node<T>* node: nodes1){
        merged->insert(node);
    }

    for (Node<T>* node: nodes2){
        merged->insert(node);
    }

    tree1->root_ = nullptr; 
    tree2->root_ = nullptr;
    return merged;
};

template <typename T> void RedBlackTree<T>::merge(RedBlackTree<T>* tree2) {
    std::vector<Node<T>*> nodes = tree2->nodes();

    for (Node<T>* node: nodes){
        insert(node);
    }

    tree2->root_ = nullptr;
};

template <typename T> void RedBlackTree<T>::show_layers() {
    std::vector<std::vector<Node<T>*>> layers = make_2d_vector();
    
    for (std::vector<Node<T>*> layer : layers) {
        for (Node<T>* node : layer) {
            std::cout << std::to_string(node->key_) << ' ';
        }
        std::cout << '\n';
    }
};

/*
template <typename T> bool RedBlackTree<T>::correct_black_height() {
    std::vector<std::vector<Node<T>*>> layers = make_2d_vector();
    for 
}
*/

template <typename T> bool RedBlackTree<T>::no_consecutive_reds() {
    for (Node<T>* node : nodes()){
        if ( node->color_){
            if(node->parent_->color_ ||
            (node->children_[0] && node->children_[0]->color_) ||
            (node->children_[1] && node->children_[1]->color_)
            )
            {
                return false;
            }
        }
    }
    return true;
};