#pragma once
#include <iostream>
#include <string>
#include "list.h"
#include "user.h"
using namespace std;

template <typename T>
class avlgroup {
private:
    struct Node {
        T key;
        list<user*> users;  // List of pointers to users
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

    Node* insert(Node*& node, T key, user* usr) {
        if (!node) {
            node = new Node(key);
            node->users.addatend(usr);
            return node;
        }

        if (key < node->key) {
            node->left = insert(node->left, key, usr);
        }
        else if (key > node->key) {
            node->right = insert(node->right, key, usr);
        }
        else {
            node->users.addatend(usr);  // Add user to existing group
        }

        balance(node);
        return node;
    }

    Node* remove(Node*& node, T key) {
        if (!node) return node;

        if (key < node->key) {
            node->left = remove(node->left, key);
        }
        else if (key > node->key) {
            node->right = remove(node->right, key);
        }
        else {
            if (node->left == nullptr || node->right == nullptr) {
                Node* temp = node->left ? node->left : node->right;
                if (!temp) {
                    temp = node;
                    node = nullptr;
                }
                else {
                    *node = *temp;
                }
                delete temp;
            }
            else {
                Node* temp = findMin(node->right);
                node->key = temp->key;
                node->users = temp->users;
                node->right = remove(node->right, temp->key);
            }
        }

        if (!node) return node;

        node->height = 1 + max(height(node->left), height(node->right));
        balance(node);

        return node;
    }

    Node* findMin(Node* node) const {
        while (node && node->left) {
            node = node->left;
        }
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
        cout << "Group: " << node->key << endl;
        for (auto usr : node->users) {
            cout << *usr << endl;
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
    avlgroup() : root(nullptr) {}

    void insert(T key, user* usr) {
        root = insert(root, key, usr);
    }

    void remove(T key) {
        root = remove(root, key);
    }

    void listUsers(T key) const {
        Node* node = search(root, key);
        if (node) {
            for (auto usr : node->users) {
                cout << *usr << endl;
            }
        }
        else {
            cout << "No users found for group: " << key << endl;
        }
    }
    void removeGroup(T key) {
        root = remove(root, key);
    }
    void display() const {
        inorderTraversal(root);
    }
    list<user*>* getUsers(T key) {
        Node* node = search(root, key);
        return node ? &node->users : nullptr;
    }
    ~avlgroup() {
        deleteTree(root);
    }
};
