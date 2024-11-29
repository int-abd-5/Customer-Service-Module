#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "complaint.h"
using namespace std;

template <typename T>
class AVLGroup {
private:
    struct Node {
        T key;
        vector<Complaint> complaints;  // Vector to store complaints
        Node* left;
        Node* right;
        int height;

        Node(T k) : key(k), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;

    int height(Node* node) {
        return node ? node->height : 0;
    }

    int getBalanceFactor(Node* node) {
        return node ? height(node->right) - height(node->left) : 0;
    }

    void rotateRight(Node*& y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        y = x;
    }

    void rotateLeft(Node*& x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        x = y;
    }

    void balance(Node*& node) {
        if (!node) return;

        node->height = 1 + max(height(node->left), height(node->right));

        int bf = getBalanceFactor(node);

        if (bf < -1) {
            if (getBalanceFactor(node->left) <= 0) {
                rotateRight(node);
            }
            else {
                rotateLeft(node->left);
                rotateRight(node);
            }
        }
        else if (bf > 1) {
            if (getBalanceFactor(node->right) >= 0) {
                rotateLeft(node);
            }
            else {
                rotateRight(node->right);
                rotateLeft(node);
            }
        }
    }

    Node* insert(Node*& node, T key, const Complaint& complaint) {
        if (!node) {
            node = new Node(key);
            node->complaints.push_back(complaint);
            return node;
        }

        if (key < node->key) {
            node->left = insert(node->left, key, complaint);
        }
        else if (key > node->key) {
            node->right = insert(node->right, key, complaint);
        }
        else {
            node->complaints.push_back(complaint);  // Add complaint to existing group
        }

        balance(node);
        return node;
    }

    Node* search(Node* node, T key) const {
        if (!node || node->key == key) {
            return node;
        }

        if (key < node->key) {
            return search(node->left, key);
        }
        return search(node->right, key);
    }

    void inorderTraversal(Node* node) const {
        if (!node) return;

        inorderTraversal(node->left);
        cout << "User ID: " << node->key << endl;
        for (const auto& comp : node->complaints) {
            cout << "  Complaint ID: " << comp.complaintID
                << ", Text: " << comp.complaintText
                << ", Type: " << comp.userType << endl;
        }
        inorderTraversal(node->right);
    }

    void deleteTree(Node*& node) {
        if (node) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
            node = nullptr;
        }
    }

public:
    AVLGroup() : root(nullptr) {}

    void insert(T key, const Complaint& complaint) {
        root = insert(root, key, complaint);
    }

    void listComplaints(T key) const {
        Node* node = search(root, key);
        if (node) {
            for (const auto& comp : node->complaints) {
                cout << "  Complaint ID: " << comp.complaintID
                    << ", Text: " << comp.complaintText
                    << ", Type: " << comp.userType << endl;
            }
        }
        else {
            cout << "No complaints found for User ID: " << key << endl;
        }
    }
    vector<Complaint>* getComplaints(T key) {
        Node* node = search(root, key);
        return node ? &node->complaints : nullptr;
    }
    void display() const {
        inorderTraversal(root);
    }

    ~AVLGroup() {
        deleteTree(root);
    }
};