#pragma once
#include <algorithm>
#include <iostream>

enum class Colour {
    RED,
    BLACK
};

template<class T>
struct RBNode {

    T key; // ?


    RBNode(T key = 0, RBNode* parent = nullptr, RBNode* left = nullptr, RBNode* right = nullptr, Colour colour = Colour::RED,
        int subtree_nodes_count = 0) : key(key), parent(parent),
        left(left), right(right), colour(colour), subtree_nodes_count(subtree_nodes_count) {
        if (left != nullptr && right != nullptr) {
            left->parent = this;
            right->parent = this;
        } else if (left != nullptr) {
            left->parent = this;
        } else if (right != nullptr){
            right->parent = this;
        }

        update_subtree_count();
    }

    bool is_left() const {
        return parent != nullptr ? this == parent->left : false;
    }

    bool is_right() const {
        return parent != nullptr ? this == parent->right : false;
    }

    bool is_root() const {
        return parent == nullptr;
    }

    bool is_red() const {
        return colour == Colour::RED;
    }

    bool is_black() const {
        return colour == Colour::BLACK;
    }

    int nodes_count() const {
        return subtree_nodes_count;
    }

    void recolour() {
        if (colour == Colour::BLACK) {
            colour = Colour::RED;
        }
        else if (colour == Colour::RED) {
            colour = Colour::BLACK;
        }
    }

    RBNode* uncle() const {
        if (parent == nullptr || parent->parent == nullptr) {
            return nullptr;
        }
        if (parent->is_left()) {
            return parent->parent->right;
        }
        return parent->parent->left;
    }

    RBNode* sibling() const {
        if (parent == nullptr) {
            return nullptr;
        }
        if (is_left()) {
            return parent->right;
        }
        return parent->left;
    }

    bool has_red_child() const {
        return (left != nullptr && left->is_red()) ||
            (right != nullptr && right->is_red());
    }

    void update_subtree_count() {
        if (left != nullptr && right != nullptr) {
            subtree_nodes_count = left->subtree_nodes_count + right->subtree_nodes_count + 2;
        } else if (left != nullptr) {
            subtree_nodes_count = left->subtree_nodes_count + 1;
        } else if (right != nullptr){
            subtree_nodes_count = right->subtree_nodes_count + 1;
        } else {
            subtree_nodes_count = 0;
        }
    }

    // int balance_factor() const;

    RBNode* parent;
    RBNode* left;
    RBNode* right;

    const Colour NULL_NODE_COLOUR = Colour::BLACK;
    Colour colour;
    int subtree_nodes_count;  // Efficient k-th minimum search
};
