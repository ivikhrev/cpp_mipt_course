#pragma once
#include "avl_node.hpp"
#include "common.hpp"

#include <cassert>
#include <memory>
#include <vector>
#include <iostream>
#include <stack>

template<class T>
class AVLTree {
public:
    AVLTree() = default;
    AVLTree(const std::vector<T>& data);
    AVLTree(const AVLTree<T>& other);
    ~AVLTree();

    void insert(T key);
    void erase(T key);
    AVLNode<T>* find(T key) const;

    int kmin(int k) const;
    int less_count(int k) const;

    std::vector<T> inorder_keys() const;
private:
    AVLNode<T>* root = nullptr;
    void rebalance(AVLNode<T>* node);
    void update_nodes_heights(AVLNode<T>* node);
    void update_nodes_subtree_count(AVLNode<T>* node);
    void left_rotate(AVLNode<T>* node);
    void right_rotate(AVLNode<T>* node);
    void left_right_rotate(AVLNode<T>* node);
    void right_left_rotate(AVLNode<T>* node);
    void delete_nodes();
};

template<class T>
AVLTree<T>::AVLTree(const std::vector<T>& data) {
    for (auto i : data) {
        insert(i);
    }
}

template<class T>
AVLTree<T>::AVLTree(const AVLTree<T>& other) {
    // preorder traversal (root, left, right)
    if (other.root != nullptr) {
        std::stack<AVLNode<T>*> s_orig, s_copy;
        root = new AVLNode<T>(other.root->key, nullptr, nullptr, nullptr, other.root->height, other.root->subtree_nodes_count);
        s_copy.push(root);
        s_orig.push(other.root);
        while (!s_orig.empty()) {
            auto* ocurr = s_orig.top();
            auto* ccurr = s_copy.top();
            s_orig.pop();
            s_copy.pop();
            if (ocurr->right != nullptr) {
                s_orig.push(ocurr->right);
                ccurr->right = new AVLNode<T>(ocurr->right->key, ccurr, nullptr, nullptr,
                    ocurr->right->height, ocurr->right->subtree_nodes_count);
                s_copy.push(ccurr->right);
            }
            if (ocurr->left != nullptr) {
                s_orig.push(ocurr->left);
                ccurr->left = new AVLNode<T>(ocurr->left->key, ccurr, nullptr, nullptr,
                    ocurr->left->height, ocurr->left->subtree_nodes_count);
                s_copy.push(ccurr->left);
            }
        }
    }
}

template<class T>
void AVLTree<T>::delete_nodes() {
    auto nodes = postorder_traversal(root);
    for (auto& n : nodes) {
        delete n;
    }
}

template<class T>
AVLTree<T>::~AVLTree() {
    delete_nodes();
}

template<class T>
void AVLTree<T>::update_nodes_heights(AVLNode<T>* node) {
    auto* parent = node;
    while (parent != nullptr) {
        parent->update_height();
        parent = parent->parent;
    }
}

template<class T>
void AVLTree<T>::update_nodes_subtree_count(AVLNode<T>* node) {
    auto* parent = node;
    while (parent != nullptr) {
        parent->update_subtree_count();
        parent = parent->parent;
    }
}

template<class T>
AVLNode<T>* AVLTree<T>::find(T key) const {
    AVLNode<T>* curr_node = root;
    while(curr_node != nullptr) {
        if (curr_node->key == key) {
            return curr_node;
        }
        if (key > curr_node->key) {
            curr_node = curr_node->right;
        } else {
            curr_node = curr_node->left;
        }
    }

    return nullptr;
}

template<class T>
void AVLTree<T>::insert(T key) {
    if (root == nullptr) {
        root = new AVLNode<T>(key);
        return;
    }

    AVLNode<T>* curr_node = root;
    while(curr_node->left != nullptr || curr_node->right != nullptr) {
        if (key >= curr_node->key) {
            if (curr_node->right == nullptr) break;
            curr_node = curr_node->right;
        } else {
            if (curr_node->left == nullptr) break;
            curr_node = curr_node->left;
        }
    }

    if (key >= curr_node->key) {
        //auto new_node = new AVLNode<T>(key, curr_node, nullptr, nullptr);
        curr_node->right = new AVLNode<T>(key, curr_node, nullptr, nullptr);
        update_nodes_heights(curr_node);
        update_nodes_subtree_count(curr_node);
        curr_node = curr_node->right;
    } else {
        curr_node->left = new AVLNode<T>(key, curr_node, nullptr, nullptr);
        update_nodes_heights(curr_node);
        update_nodes_subtree_count(curr_node);
        curr_node = curr_node->left;
    }
    rebalance(curr_node);
}

template<class T>
void AVLTree<T>::erase(T key) {
    AVLNode<T>* to_delete = find(key);
    if (to_delete == nullptr) {
        return;
    }

    rebalance(to_delete);
}

template<class T>
void AVLTree<T>::rebalance(AVLNode<T>* node) {
    AVLNode<T>* curr_node = node;
    while (curr_node != nullptr) {
        if (curr_node->balance_factor() < -1) {  // right heavy
            if (curr_node->right->balance_factor() == 1) { // left heavy
                right_rotate(curr_node->right);
                left_rotate(curr_node);
            } else if (curr_node->right->balance_factor() == -1) { // right heavy
                left_rotate(curr_node);
            }
        } else if (curr_node->balance_factor() > 1) {  // left heavy
            if (curr_node->left->balance_factor() == 1) { // left heavy
                right_rotate(curr_node);
            } else if (curr_node->left->balance_factor() == -1) { // right heavy
                left_rotate(curr_node->left);
                right_rotate(curr_node);
            }
        } else {
            curr_node = curr_node->parent;
        }
    }
}

template<class T>
void AVLTree<T>::right_rotate(AVLNode<T>* node) {
    auto* parent = node->parent;
    if (parent != nullptr) {
        // make old left new paret
        if (node == parent->left) {
            parent->left = node->left;
            node->left->parent = parent;
            // rotate old left right node
            auto* old_left_right = parent->left->right;
            node->left = old_left_right;
            if (old_left_right != nullptr) {
                old_left_right->parent = node;
            }
            // make old parent new left
            parent->left->right = node;
            node->parent = parent->left;
        } else {
            parent->right = node->left;
            node->left->parent = parent;

            // rotate old left right node
            auto* old_left_right = node->left->right;
            node->left = old_left_right;
            if (old_left_right != nullptr) {
                old_left_right->parent = node;
            }
            // make old parent new left
            parent->right->right = node;
            node->parent = parent->right;
        }
        update_nodes_heights(node);
        update_nodes_subtree_count(node);
    } else {
        // make old left new root
        auto* old_parent = node;
        // auto* old_parent_right = node->right;
        old_parent->left->parent = nullptr;

        auto* new_parent = old_parent->left;
        auto* old_left_right = new_parent->right;

        // rotate old parent to the right
        new_parent->right = old_parent;
        old_parent->parent = new_parent;

        // rotate old left right node
        old_parent->left = old_left_right;
        if (old_left_right != nullptr) {
            old_left_right->parent = old_parent;
        }

        update_nodes_heights(old_parent);
        update_nodes_subtree_count(old_parent);

        root = new_parent;
        update_nodes_heights(root);
        update_nodes_subtree_count(root);
    }
}

template<class T>
void AVLTree<T>::left_rotate(AVLNode<T>* node) {
    auto* parent = node->parent;
    if (parent != nullptr) {
        // make old right new paret
        if (node == parent->right) {
            parent->right = node->right;
            node->right->parent = parent;
            // rotate old right left node
            auto* old_right_left = parent->right->left;
            node->right = old_right_left;
            if (old_right_left != nullptr) {
                old_right_left->parent = node;
            }
            // make old parent new left
            parent->right->left = node;
            node->parent = parent->right;
        } else {
            parent->left = node->right;
            node->right->parent = parent;

            // rotate old right left node
            auto* old_right_left = node->right->left;
            node->right = old_right_left;
            if (old_right_left != nullptr) {
                old_right_left->parent = node;
            }

            // make old parent new left
            parent->left->left = node;
            node->parent = parent->left;
        }
        update_nodes_heights(node);
        update_nodes_subtree_count(node);
    } else {
        // make old right new root
        auto* old_parent = node;
        // auto* old_parent_left = node->left;
        old_parent->right->parent = nullptr;

        auto* new_parent = old_parent->right;
        auto* old_right_left = new_parent->left;

        // rotate old parent to the left
        new_parent->left = old_parent;
        old_parent->parent = new_parent;

        // rotate old right left node
        old_parent->right = old_right_left;
        if (old_right_left != nullptr) {
            old_right_left->parent = old_parent;
        }

        update_nodes_heights(old_parent);
        update_nodes_subtree_count(old_parent);

        root = new_parent;
        update_nodes_heights(root);
        update_nodes_subtree_count(root);
    }
}

template<class T>
std::vector<T> AVLTree<T>::inorder_keys() const {
    std::vector<T> inorder_data;
    auto inorder_nodes = inorder_traversal(root);
    std::transform(inorder_nodes.cbegin(), inorder_nodes.cend(),
        std::back_inserter(inorder_data), [](const AVLNode<T>* n) {return n->key;});
    return inorder_data;
}

template<class T>
int AVLTree<T>::kmin(int k) const {
    assert(k > 0);
    if (root == nullptr) {
        throw std::logic_error("Empty tree");
    }
    auto* curr = root;
    while (true) {
        int left_subtree_count = curr->left ? curr->left->subtree_nodes_count + 1 : 0;
        if (k <= left_subtree_count) {
            curr = curr->left;
        }
        else if (k > left_subtree_count + 1) {
            k -= left_subtree_count + 1;
            curr = curr->right;
        }
        else {
            break;
        }
    }

    return curr->key;
}

template<class T>
int AVLTree<T>::less_count(int k) const {
    int count = 0;
    auto* curr = root;
    while (curr) {
        if (k == curr->key) {
            count += curr->left ? curr->left->subtree_nodes_count + 1 : 0;
            break;
        }
        else if (k > curr->key) {
            count += curr->left ? curr->left->subtree_nodes_count + 2 : 1;
            curr = curr->right;
        }
        else {
            curr = curr->left;
        }
    }
    return count;
}