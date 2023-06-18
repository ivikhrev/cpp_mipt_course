#pragma once
#include <algorithm>
#include <iostream>

template<class T>
struct AVLNode {
    const T key; // ?

    AVLNode(T key = 0, AVLNode* parent = nullptr, AVLNode* left = nullptr, AVLNode* right = nullptr, int height = 0,
        int subtree_nodes_count = 0) : key(key), parent(parent),
        left(left), right(right), height(height), subtree_nodes_count(subtree_nodes_count) {
        if (left != nullptr && right != nullptr) {
            left->parent = this;
            right->parent = this;
        } else if (left != nullptr) {
            left->parent = this;
        } else if (right != nullptr){
            right->parent = this;
        }
        update_height();
        update_subtree_count();
    }

    int nodes_count() const {
        return subtree_nodes_count;
    }

    void update_height() {
        if (left != nullptr && right != nullptr) {
            height = std::max(left->height, right->height) + 1;
        } else if (left != nullptr) {
            height = left->height + 1;
        } else if (right != nullptr){
            height = right->height + 1;
        } else {
            height = 0;
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

    int balance_factor() const;

    AVLNode* parent;
    AVLNode* left;
    AVLNode* right;

    const int NULL_NODE_HEIGHT = -1;
    int height;  // AVL invariant
    int subtree_nodes_count;  // Efficient k-th minimum search
};

template<class T>
int AVLNode<T>::balance_factor() const {
    if (left != nullptr && right != nullptr) {
        return left->height - right->height;
    } else if (left != nullptr) {
        return left->height - NULL_NODE_HEIGHT;
    } else if (right != nullptr){
        return NULL_NODE_HEIGHT - right->height;
    } else {
        return height;
    }
}