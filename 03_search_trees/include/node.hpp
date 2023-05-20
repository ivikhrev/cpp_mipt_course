#pragma once
#include <algorithm>
#include <iostream>

template<class T>
struct Node {
    const T key; // ?

    Node(T key, Node* parent = nullptr, Node* left = nullptr, Node* right = nullptr) : key(key), parent(parent),
        left(left), right(right) {
        if (left != nullptr && right != nullptr) {
            height = std::max(left->height, right->height) + 1;
            subtree_nodes_count = left->subtree_nodes_count + right->subtree_nodes_count + 2;
            left->parent = this;
            right->parent = this;
        } else if (left != nullptr) {
            height = left->height + 1;
            subtree_nodes_count = left->subtree_nodes_count + 1;
            left->parent = this;
        } else if (right != nullptr){
            height = right->height + 1;
            subtree_nodes_count = right->subtree_nodes_count + 1;
            right->parent = this;
        } else {
            height = 0;
            subtree_nodes_count = 0;
        }
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

    Node* parent;
    Node* left;
    Node* right;

    const int NULL_NODE_HEIGHT = -1;
    int height;  // AVL invariant
    int subtree_nodes_count;  // Efficient k-th minimum search
};

template<class T>
int Node<T>::balance_factor() const {
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