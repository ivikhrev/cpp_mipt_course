#pragma once
#include <algorithm>
#include <iostream>

enum class Colour {
    RED,
    BLACK
};

template<class T>
struct RBNode {

    const T key; // ?

    RBNode(T key, RBNode* parent = nullptr, RBNode* left = nullptr, RBNode* right = nullptr, Colour colour = Colour::RED,
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
        // update_height();
        update_subtree_count();
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

// template<class T>
// int RBNode<T>::balance_factor() const {
//     if (left != nullptr && right != nullptr) {
//         return left->height - right->height;
//     } else if (left != nullptr) {
//         return left->height - NULL_NODE_HEIGHT;
//     } else if (right != nullptr){
//         return NULL_NODE_HEIGHT - right->height;
//     } else {
//         return height;
//     }
// }
