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
    void fix_double_red(RBNode<T>* node);
    void fix_double_black(RBNode<T>* node);
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
    fix_double_red(curr_node);
}

template<class T>
void RBTree<T>::erase(T key) {
    RBNode<T>* to_delete = find(key);
    if (to_delete == nullptr) {
        return;
    }

    // find successor - leaf node in leftmost node in the right subtree
    RBNode<T>* curr = to_delete;
    if (to_delete->right != nullptr) {
        curr = to_delete->right;
        while (curr->left != nullptr) {
            curr = curr->left;
        }
    }

    std::swap(to_delete->key, curr->key);

    if (curr->is_black() && (curr->left != nullptr && curr->left->is_red())) {
        curr->left->colour = Colour::BLACK;
    } else if (curr->is_black() && (curr->left == nullptr || curr->left->is_black())) {
        fix_double_black(curr);
    }


    RBNode<T>* parent = curr->parent;
    if (curr->is_root()) {
        root = curr->left;
        if (curr->left != nullptr) {
            curr->left->parent = nullptr;
        }
    }
    else if (curr->is_right() && curr->left != nullptr) {
        parent->right = curr->left;
        curr->left->parent = parent;
    }
    else if (curr->is_right() && curr->right != nullptr) {
        parent->right = curr->right;
        curr->right->parent = parent;
    }
     else if (curr->is_left() && curr->left != nullptr) {
        parent->left = curr->left;
        curr->left->parent = parent;
    }
    else if (curr->is_left() && curr->right != nullptr) {
        parent->left = curr->right;
        curr->right->parent = parent;
    } else if (curr->is_left()) {
        parent->left = nullptr;
    } else {
        parent->right = nullptr;
    }

    update_nodes_subtree_count(parent);
    delete curr;
}

template<class T>
void RBTree<T>::fix_double_black(RBNode<T>* curr) {
        if (curr == nullptr) {
            return;
        }

        RBNode<T>* parent = curr->parent;
        auto* sibling = curr->sibling();
        if (sibling == nullptr) {
            fix_double_black(parent);
        }
        else if (sibling->is_black() && sibling->has_red_child()) {
            if (sibling->is_left()) {
                if (sibling->left !=nullptr && sibling->left->is_red()) {
                    sibling->left->colour = Colour::BLACK;
                    right_rotate(sibling->parent);
                } else {
                    sibling->right->colour = Colour::BLACK;
                    left_rotate(sibling);
                    right_rotate(sibling->parent);
                }
            } else {
                if (sibling->right !=nullptr && sibling->right->is_red()) {
                    sibling->right->colour = Colour::BLACK;
                    left_rotate(sibling->parent);
                } else {
                    sibling->left->colour = Colour::BLACK;
                    right_rotate(sibling);
                    left_rotate(sibling->parent);
                }
            }
        } else if (sibling->is_black()) {
            sibling->colour = Colour::RED;
            if (parent->is_red()) {
                parent->colour = Colour::BLACK;
            } else {
                fix_double_black(parent);
            }
        } else if (sibling->is_red()) {
            std::swap(parent->colour, sibling->colour);
            if (sibling->is_left()) {
                right_rotate(parent);
            } else {
                left_rotate(parent);
            }
            fix_double_black(curr);
        }
}

template<class T>
void RBTree<T>::fix_double_red(RBNode<T>* node) {
    RBNode<T>* curr_node = node;
    while (curr_node != nullptr  && curr_node->parent != nullptr && curr_node->parent->parent != nullptr) {
        if (curr_node->colour == Colour::RED && curr_node->parent->colour == Colour::RED) {
            auto* parent = curr_node->parent;
            auto* grandfather = curr_node->parent->parent;
            auto* uncle = curr_node->uncle();
            if (uncle != nullptr && uncle->colour == Colour::RED) {
                uncle->recolour();
                parent->recolour();
                if (grandfather->parent != nullptr) {  // if not root
                    grandfather->recolour();
                }
            } else {
                auto* parent = curr_node->parent;
                auto* grandfather = curr_node->parent->parent;
                if (parent->is_left() && curr_node->is_left()) {
                    std::swap(grandfather->colour, parent->colour);
                    right_rotate(grandfather);
                } else if (parent->is_left() && curr_node->is_right()) {
                    std::swap(grandfather->colour, curr_node->colour);
                    left_rotate(parent);
                    right_rotate(grandfather);
                } else if (parent->is_right() && curr_node->is_right()) {
                    std::swap(grandfather->colour, parent->colour);
                    left_rotate(grandfather);
                } else if (parent->is_right() && curr_node->is_left()) {
                    std::swap(grandfather->colour, curr_node->colour);
                    right_rotate(parent);
                    left_rotate(grandfather);
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

        update_nodes_subtree_count(old_parent);

        root = new_parent;
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
        update_nodes_subtree_count(node);
    } else {
        // make old right new root
        auto* old_parent = node;
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

        update_nodes_subtree_count(old_parent);

        root = new_parent;
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
