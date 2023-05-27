#pragma once
#include <vector>
#include <stack>

template<class Node>
std::vector<Node> inorder_traversal_(Node root) {
    std::stack<Node> s;
    std::vector<Node> inorder_nodes;
    Node curr = root;
    while (curr != nullptr || !s.empty()) {
        while(curr) {
            s.push(curr);
            curr = curr->left;
        }
        curr = s.top();
        s.pop();
        inorder_nodes.push_back(curr);
        curr = curr->right;
    }

    return inorder_nodes;
}

template<class Node>
std::vector<Node*> preorder_traversal(const Node*& root) {
    std::stack<Node*> s;
    std::vector<Node*> preorder_nodes;
    Node* curr = root;
    while (!s.empty()) {
        auto* curr = s.top();
        s.pop();
        preorder_nodes.push_back(curr);
        if (curr->right != nullptr) {
            s.push(curr->right);
        }
        if (curr->left != nullptr) {
            s.push(curr->left);
        }
    }

    return preorder_nodes;
}

template<class Node>
std::vector<Node*> postorder_traversal(Node*& root) {
    std::stack<Node*> s1, s2;
    if (root) {
        s1.push(root);
    }
    // postorder traversal (left, right, root)
    while (!s1.empty()) {
        auto* parent = s1.top();
        s1.pop();
        s2.push(parent);
        if (parent->left) {
            s1.push(parent->left);
        }

        if (parent->right) {
            s1.push(parent->right);
        }
    }

    std::vector<Node*> postorder_nodes(s2.size());
    std::generate(postorder_nodes.begin(), postorder_nodes.end(), [&s2] () {
        auto res = s2.top();
        s2.pop();
        return res;
    });
    return postorder_nodes;
}
