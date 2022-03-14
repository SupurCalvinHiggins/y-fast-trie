#pragma once
#include "Node.h"
#include <vector>
#include <string>
#include <iostream>

template <typename T>
class RBTree {
    friend Node<T>;
public:
	RBTree();
	Node<T>* rotate(Node<T>* root, bool side);
	Node<T>* left_rotate(Node<T>* root);
	Node<T>* right_rotate(Node<T>* root);
	void recolor_tree();
	void check(Node<T>* node);
	void insert(T key);
	Node<T>* find(T key);
    bool contains(T key);
    void destroy(Node<T>* node);
	void remove(T key);
	std::string show_color(Node<T>* node);
    void show(Node<T>* node, int depth = 0);

    Node<T>* root();
    void root(Node<T>* node);

    int size();
private:
    Node<T>* root_;
	int size_;

};

template <typename T> RBTree<T>::RBTree() {
    root_ = nullptr;
    size_ = 0;
};

template <typename T> Node<T>* RBTree<T>::rotate(Node<T>* root, bool side) {
    if (root->parent_ == nullptr) {
        Node<T>* parent = root->children_[side ^ 1];

        Node<T>* grandchild = root->children_[side ^ 1]->children_[side];

            root->children_[side ^ 1]->children_[side] = root;
            root->parent_ = root->children_[side ^ 1];

        if (grandchild != nullptr) {
            grandchild->parent_ = root;
            root->children_[side ^ 1] = grandchild;
        }
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
            root->children_[side ^ 1] = grandchild;
        }
        parent->children_[parent->children_[1] == root] = root->parent_;

        root->parent_->parent_ = parent;
        return root->parent_;
    }
};

template <typename T> Node<T>* RBTree<T>::left_rotate(Node<T>* root) {
    return rotate(root, 0);
};

template <typename T> Node<T>* RBTree<T>::right_rotate(Node<T>* root) {
    return rotate(root, 1);
};

template <typename T> void RBTree<T>::recolor_tree() {
    std::vector<std::vector<Node<T>*>>layers;
    layers.back().push_back(root_);
    unsigned int layer = 0;
    while (layers[layer].size() > 0) {
        std::vector<Node<T>*> new_layer;
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
    bool layer_color = 0;
    for (std::vector<Node<T>*> row : layers) {
        for (Node<T>* node : row) {
            node->color_ = layer_color;
        }
        layer_color = !layer_color;
    }
};

template <typename T> void RBTree<T>::check(Node<T>* node) {
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
                check(parent->parent_);
                if (root_->color_ == 1) {
                    recolor_tree();
                }
            }
        }
    }
}

template <typename T> void RBTree<T>::insert(T key) {
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
                check(target_node->children_[dir]);
                return;
            }
            else {
                target_node = target_node->children_[dir];
            }
        }
    }
};

template <typename T> Node<T>* RBTree<T>::find(T key) {
    Node<T>* target_node = root_;
    while (target_node != nullptr && target_node->key_ != key) {
        target_node = target_node->children_[key > target_node->key_];
    }
    return target_node;
};

template <typename T> bool RBTree<T>::contains(T key) {
    return find(key) != nullptr;
};

template <typename T> void RBTree<T>::destroy(Node<T>* node) {
    delete node;
};

template <typename T> void RBTree<T>::remove(T key) {
    Node<T>* node = find(key);
    if (node == nullptr) {
        return;
    }
    Node<T>* parent = node->parent_;
    if (node == nullptr) {
        return;
    }
    else{
        bool child_dir = parent->children_[1] == node;
        if (node->children_[0] == nullptr) {
            parent->children_[child_dir] = node->children_[1];
            delete node;
        }
        else {
            if (node->children_[1] == nullptr) {
                parent->children_[child_dir] = node->children_[1];
                delete node;
            }
            else {
                parent->children_[child_dir] = rotate(node->children_[1], 0);
                parent->children_[child_dir] = rotate(node->children_[0], 1);
                node->children_[1]->parent_ = parent->children_[child_dir];
                parent->children_[child_dir]->children_[1] = node->children_[1];
                delete node;
            }
        }
        recolor_tree();
    }
};

template <typename T> void RBTree<T>::show(Node<T>* node, int depth) {
    std::string colors[2] = { "Black","Red"};
    std::string tabs = "";
    for (int i = 0; i < depth; i++) {
        tabs += "\t";
    }
    if (node != nullptr) {
        std::cout << tabs << "Null";
    }
    else {
        std::cout << ' ' << colors[node->color_] << ' ' << std::to_string(node->key_) << '\n';
        show(node->left());
        show(node->right());

    }
}

template <typename T> void RBTree<T>::root(Node<T>* node) {
    root_ = node;
}

template <typename T> Node<T>* RBTree<T>::root() {
    return root_;
}

template <typename T> int RBTree<T>::size() {
    return size_;
};