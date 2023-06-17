#pragma once
#include "rb_tree/rb_node.hpp"
#include "common.hpp"

#include <cassert>
#include <memory>
#include <vector>
#include <iostream>
#include <stack>

template<class T>
class RBTree {
public:
    RBTree() = default;
    RBTree(const std::vector<T>& data);
    RBTree(const RBTree<T>& other);
    ~RBTree();

    void insert(T key);
    void erase(T key);
    RBNode<T>* find(T key) const;

    int kmin(int k) const;
    int less_count(int k) const;

    std::vector<T> inorder_keys() const;

private:
    RBNode<T>* root = nullptr;
    void rebalance(RBNode<T>* node);
    void recolour_nodes(RBNode<T>* node);
    void update_nodes_subtree_count(RBNode<T>* node);
    void left_rotate(RBNode<T>* node);
    void right_rotate(RBNode<T>* node);
    void delete_nodes();
};

template<class T>
RBTree<T>::RBTree(const std::vector<T>& data) {
    for (auto i : data) {
        insert(i);
    }
}

template<class T>
RBTree<T>::RBTree(const RBTree<T>& other) {
    // preorder traversal (root, left, right)
    if (other.root != nullptr) {
        std::stack<RBNode<T>*> s_orig, s_copy;
        root = new RBNode<T>(other.root->key, nullptr, nullptr, nullptr, other.root->colour, other.root->subtree_nodes_count);
        s_copy.push(root);
        s_orig.push(other.root);
        while (!s_orig.empty()) {
            auto* ocurr = s_orig.top();
            auto* ccurr = s_copy.top();
            s_orig.pop();
            s_copy.pop();
            if (ocurr->right != nullptr) {
                s_orig.push(ocurr->right);
                ccurr->right = new RBNode<T>(ocurr->right->key, ccurr, nullptr, nullptr,
                    ocurr->right->colour, ocurr->right->subtree_nodes_count);
                s_copy.push(ccurr->right);
            }
            if (ocurr->left != nullptr) {
                s_orig.push(ocurr->left);
                ccurr->left = new RBNode<T>(ocurr->left->key, ccurr, nullptr, nullptr,
                    ocurr->left->colour, ocurr->left->subtree_nodes_count);
                s_copy.push(ccurr->left);
            }
        }
    }
}

template<class T>
void RBTree<T>::delete_nodes() {
    auto nodes = postorder_traversal(root);
    for (auto& n : nodes) {
        delete n;
    }
}

template<class T>
RBTree<T>::~RBTree() {
    delete_nodes();
}

template<class T>
void RBTree<T>::recolour_nodes(RBNode<T>* node) {
    auto* parent = node;
    // while (parent->parent != nullptr) {
    //     if
    // }
}

template<class T>
void RBTree<T>::update_nodes_subtree_count(RBNode<T>* node) {
    auto* parent = node;
    while (parent != nullptr) {
        parent->update_subtree_count();
        parent = parent->parent;
    }
}

template<class T>
RBNode<T>* RBTree<T>::find(T key) const {
    RBNode<T>* curr_node = root;
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
void RBTree<T>::insert(T key) {
    if (root == nullptr) {
        root = new RBNode<T>(key, nullptr, nullptr, nullptr, Colour::BLACK);
        return;
    }

    RBNode<T>* curr_node = root;
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
        //auto new_node = new RBNode<T>(key, curr_node, nullptr, nullptr);
        curr_node->right = new RBNode<T>(key, curr_node, nullptr, nullptr);
        // recolour_nodes(curr_node);
        update_nodes_subtree_count(curr_node);
        curr_node = curr_node->right;
    } else {
        curr_node->left = new RBNode<T>(key, curr_node, nullptr, nullptr);
        // recolour_nodes(curr_node);
        update_nodes_subtree_count(curr_node);
        curr_node = curr_node->left;
    }
    rebalance(curr_node);
}

template<class T>
void RBTree<T>::erase(T key) {
    RBNode<T>* to_delete = find(key);
    if (to_delete == nullptr) {
        return;
    }

    RBNode<T>* curr = to_delete;
    if (to_delete->right != nullptr) {
        curr = to_delete->right;
        while (curr->left != nullptr) {
            curr = curr->left;
        }
        if (curr == to_delete->right) {
            curr->parent->right = nullptr;
        } else {
            curr->parent->left = nullptr;
        }
    }

    RBNode<T>* parent = to_delete->parent;
    //delete root
    if (parent == nullptr) {
        if (curr != nullptr && curr != to_delete) {
            curr->parent = nullptr;
            curr->left = to_delete->left;
            if (to_delete->left != nullptr) {
                to_delete->left->parent = curr;
            }

            if (curr != to_delete->right) {
                auto* old_right = curr->right;
                curr->right = to_delete->right;
                if (to_delete->right != nullptr) {
                    to_delete->right->left = old_right;
                    to_delete->right->parent = curr;
                }
            } else {
                curr->right = nullptr;
            }
            root = curr;
        } else {
            curr = root = to_delete->left;
            if (to_delete->left != nullptr) {
                to_delete->left ->parent = nullptr;
            }
        }
    }
    else if (to_delete == parent->right && curr == to_delete) {
        parent->right = curr->left;
        if (curr->left != nullptr) {
            curr->left->parent = parent;
            curr = curr->left;
        } else {
            curr = parent;
        }
    }
    else if (to_delete == parent->right) {
        parent->right = curr;
        curr->parent = parent;

        curr->left = to_delete->left;
        if (curr->left != nullptr) {
            curr->left->parent = curr;
        }

        curr->right = to_delete->right;
        if (curr->right != nullptr && curr != curr->right) {
            curr->right->parent = curr;
        } else {
            curr->right = nullptr;
        }
    }
    else if (to_delete == parent->left && curr == to_delete) {
        parent->left = curr->left;
        if (curr->left != nullptr) {
            curr->left->parent = parent;
            curr = curr->left;
        } else {
            curr = parent;
        }

    }
    else if (to_delete == parent->left) {
        parent->left = curr;
        curr->parent = parent;

        curr->left = to_delete->left;
        if (curr->left != nullptr) {
            curr->left->parent = curr;
        }

        curr->right = to_delete->right;
        if (curr->right != nullptr && curr != curr->right) {
            curr->right->parent = curr;
        } else {
            curr->right = nullptr;
        }
    }

    // update_nodes_heights(curr);
    update_nodes_subtree_count(curr);
    delete to_delete;
    rebalance(curr);
}

template<class T>
void RBTree<T>::rebalance(RBNode<T>* node) {
    RBNode<T>* curr_node = node;
    while (curr_node != nullptr  && curr_node->parent != nullptr && curr_node->parent->parent != nullptr) {
        if (curr_node->colour == Colour::RED && curr_node->parent->colour == Colour::RED) {
            auto* parent = curr_node->parent;
            auto* grandfather = curr_node->parent->parent;
            RBNode<T>* uncle;
            if (grandfather->left == parent) {
                uncle = grandfather->right;
            } else {
                uncle = grandfather->left;
            }
            if (uncle != nullptr && uncle->colour == Colour::RED) {
                uncle->recolour();
                parent->recolour();
                if (grandfather->parent != nullptr) {  // if not root
                    grandfather->recolour();
                }
            } else {
                auto* parent = curr_node->parent;
                auto* grandfather = curr_node->parent->parent;
                RBNode<T>* uncle;
                if (grandfather->left == parent && parent->left == curr_node) {
                    // uncle = grandfather->right;
                    right_rotate(grandfather);
                    std::swap(grandfather->colour, parent->colour);
                } else if (grandfather->left == parent && parent->right == curr_node) {
                    left_rotate(parent);
                    right_rotate(grandfather);
                    std::swap(grandfather->colour, curr_node->colour);
                } else if (grandfather->right == parent && parent->right == curr_node) {
                    left_rotate(grandfather);
                    std::swap(grandfather->colour, parent->colour);
                } else if (grandfather->right == parent && parent->left == curr_node) {
                    right_rotate(parent);
                    left_rotate(grandfather);
                    std::swap(grandfather->colour, curr_node->colour);
                }
            }
        }
        curr_node = curr_node->parent;
    }
}

template<class T>
void RBTree<T>::right_rotate(RBNode<T>* node) {
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
        // update_nodes_heights(node);
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

        // update_nodes_heights(old_parent);
        update_nodes_subtree_count(old_parent);

        root = new_parent;
        // update_nodes_heights(root);
        update_nodes_subtree_count(root);
    }
}

template<class T>
void RBTree<T>::left_rotate(RBNode<T>* node) {
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
        // update_nodes_heights(node);
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

        // update_nodes_heights(old_parent);
        update_nodes_subtree_count(old_parent);

        root = new_parent;
        // update_nodes_heights(root);
        update_nodes_subtree_count(root);
    }
}

template<class T>
std::vector<T> RBTree<T>::inorder_keys() const {
    std::vector<T> inorder_data;
    auto inorder_nodes = inorder_traversal(root);
    std::transform(inorder_nodes.cbegin(), inorder_nodes.cend(),
        std::back_inserter(inorder_data), [](const RBNode<T>* n) {return n->key;});
    return inorder_data;
}

template<class T>
int RBTree<T>::kmin(int k) const {
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
int RBTree<T>::less_count(int k) const {
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
