#pragma once
#include "red-black-tree-node.h"
#include <vector>
#include <string>
#include <iostream>

template <typename T>
class RedBlackTree {
    friend Node<T>;
public:
	RedBlackTree();
	Node<T>* rotate(Node<T>* root, bool side);
	Node<T>* left_rotate(Node<T>* root);
	Node<T>* right_rotate(Node<T>* root);
	void recolor_tree();
	void insert_check(Node<T>* node);
	void insert(T key);
	Node<T>* find(T key);
    bool contains(T key);
    void remove_check(Node<T>* parent, bool color, bool dir);
	void remove(T key);
	std::string show_color(Node<T>* node);
    void show(Node<T>* node, int depth = 0);
    bool check_balance();

    Node<T>* root();
    void root(Node<T>* node);

    int size();
private:
    Node<T>* root_;
	int size_;

};

template <typename T> RedBlackTree<T>::RedBlackTree() {
    root_ = nullptr;
    size_ = 0;
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
    bool layer_color = 0;
    for (int i = 0; i < layers.size() - 1; i++) {
        for (Node<T>* node : layers[i]) {
            node->color_ = 0;
        }
    }
    for (Node<T>* node : layers.back()) {
        node->color_ = 1;
    }
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
}

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

template <typename T> Node<T>* RedBlackTree<T>::find(T key) {
    Node<T>* target_node = root_;
    while (target_node != nullptr && target_node->key_ != key) {
        target_node = target_node->children_[key > target_node->key_];
    }
    return target_node;
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
        Node<T>* succ = node->children_[1];
        if (succ == nullptr) {
            root_ = node->children_[0];
            if (root_ != nullptr) {
                root_->parent_ = nullptr;
            }
            parent = node->parent_;
            color = node->color_;
            delete node;
        }
        else {
            while (succ->children_[0] != nullptr) {
                succ = succ->children_[0];
            }
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
            if (parent->children_[dir ^ 1] != nullptr && parent->children_[dir ^ 1]->color_ == 1) {
                bool temp_color = parent->children_[dir ^ 1]->color_;
                parent->children_[dir ^ 1]->color_ = parent->color_;
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
                    parent_sibling->children_[!parent_dir]->color_ = 0;
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
    if (node != nullptr) {
        std::cout << std::to_string(depth) << lines << ' ' << colors[node->color_] << ' ' << std::to_string(node->key_) << '\n';
        show(node->left(),depth + 1);
        show(node->right(), depth + 1);
    }
    else {
        std::cout << std::to_string(depth) << lines << " Null" << '\n';
    }
}

template <typename T> void RedBlackTree<T>::root(Node<T>* node) {
    root_ = node;
}

template <typename T> Node<T>* RedBlackTree<T>::root() {
    return root_;
}

template <typename T> int RedBlackTree<T>::size() {
    return size_;
};

template <typename T> bool RedBlackTree<T>::check_balance() {
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

    if (layers[layers.size() -2].size() != 1 << (layers.size() - 2) && layers[layers.size() - 1].size() != 0) {
        return false;
    }
    return true;
};