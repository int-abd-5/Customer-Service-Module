#pragma once
#include <iostream>
#include <algorithm>
#include "list.h"
#include "user.h"
using namespace std;
template <typename T>
//template<typename t>
class avl {
private:
    struct Node {
        T data;
        list<user>::mynode* pointer;
        Node* right;
        Node* left;
        int height;
        Node(T d, list<user>::mynode * ptr) : data(d), right(nullptr), left(nullptr), height(1), pointer(ptr){
            
        }
    };

    Node* root;

    int height(Node* ptr) {
        if (ptr == nullptr) return 0;
        return ptr->height;
    }

    /*int height(Node* nodeptr) {
        if (nodeptr == nullptr) {
            return -1;
        }
        int left = -1, right = -1;
        if (nodeptr->left) {
            left = nodeptr->left->height;
        }
        if (nodeptr->right) {
            right = nodeptr->right->height;
        }
        return max(left, right) + 1;
    }*/

    int getBalanceFactor(Node* ptr) {
        if (ptr == nullptr) return 0;
        return height(ptr->right) - height(ptr->left);
    }

    /*int getBalanceFactor(Node* ptr) {
        if (ptr == nullptr) return -1;
        return height(ptr->right) - height(ptr->left);
    }*/

    void rotateRight(Node*& x) {
        Node* y = x->left;
        Node* orphan = y->right;

        y->right = x;
        x->left = orphan;

        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        x = y;
    }

    void rotateLeft(Node*& x) {
        Node* y = x->right;
        Node* orphan = y->left;

        y->left = x;
        x->right = orphan;

        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        x = y;
    }

    void balance(Node*& x) {
        if (x == nullptr) return;

        x->height = 1 + max(height(x->left), height(x->right));

        int bf = getBalanceFactor(x);

        // Left heavy
        if (bf < -1) {
            if (getBalanceFactor(x->left) <= 0) {
                // Left-Left case
                rotateRight(x);
            }
            else {
                // Left-Right case
                rotateLeft(x->left);
                rotateRight(x);
            }
        }
        // Right heavy
        else if (bf > 1) {
            if (getBalanceFactor(x->right) >= 0) {
                // Right-Right case
                rotateLeft(x);
            }
            else {
                // Right-Left case
                rotateRight(x->right);
                rotateLeft(x);
            }
        }
    }

    //void balance(Node*& x) {
    //    if (x == nullptr) return;

    //    x->height = 1 + max(height(x->left), height(x->right));

    //    int bf = getBalanceFactor(x);

    //    // Left heavy
    //    if (bf < -1) {
    //        if (getBalanceFactor(x->left) <= 1) {
    //            // Left-Left case
    //            rotateRight(x);
    //        }
    //        else {
    //            // Left-Right case
    //            rotateLeft(x->left);
    //            rotateRight(x);
    //        }
    //    }
    //    // Right heavy
    //    else if (bf > 1) {
    //        if (getBalanceFactor(x->right) >= 1) {
    //            // Right-Right case
    //            rotateLeft(x);
    //        }
    //        else {
    //            // Right-Left case
    //            rotateRight(x->right);
    //            rotateLeft(x);
    //        }
    //    }
    //}
    
    bool insert(Node*& node, T &value, list<user>::mynode*& t) {
        if (node == nullptr) {
            node = new Node(value,t);
            return true;
        }

        if (value< node->data) {
            insert(node->left, value,t);
        }
        else if (value > node->data) {
            insert(node->right, value,t);
        }
        else {
            return false; // Duplicate `userid`
        }

        balance(node);
        return true;
    }


    Node* remove(Node* root, int key,string& name,string &email, string& type, string& country) {
        if (root == nullptr)
            return root;

        // Use the user class's methods for comparison
        if (root->data < key) {
            root->right = remove(root->right, key,name,email,type,country);
        }
        else if (root->data > key) {
            root->left = remove(root->left, key,name,email,type,country);
        }
        else {
            // Node to be deleted is found
            Node* v = root;
            if ((root->left == nullptr) || (root->right == nullptr)) {
                Node* temp = root->left ? root->left : root->right;

                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                }
                else {
                    *root = *temp; // Copy the contents of the non-empty child
                }
                
                //delete temp->pointer;
                delete temp;
            }
            else {
                v->pointer->prev->next = v->pointer->next;
                v->pointer->next->prev = v->pointer->prev;
                v->pointer->data.updatename(name);
                v->pointer->data.updateemail(email);
                v->pointer->data.updatetype(type);
                v->pointer->data.updatecountry(country);
                Node* temp = findMin(root->right);
                root->data = temp->data; // Replace data with minNode's data
                root->pointer = temp->pointer;
                root->right = remove(root->right, temp->data,name,email,type,country);
            }
        }

        if (root == nullptr)
            return root;

        // Update height of the current node
        root->height = 1 + max(height(root->left), height(root->right));

        // Balance the tree using your custom balance function
        balance(root);

        return root;
    }

    Node* removes(Node*& root, string key) {
        if (root == nullptr)
            return root;

        // Use the user class's methods for comparison
        if (root->data < key) {
            root->right = removes(root->right, key);
        }
        else if (root->data > key) {
            root->left = removes(root->left, key);
        }
        else {
            // Node to be deleted is found
            Node* v = root;
            if ((root->left == nullptr) || (root->right == nullptr)) {
                Node* temp = root->left ? root->left : root->right;

                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                }
                else {
                    *root = *temp; // Copy the contents of the non-empty child
                }
                delete temp;
            }
            else {
                Node* temp = findMin(root->right);
                root->data = temp->data; // Replace data with minNode's data
                root->pointer = temp->pointer;
                root->right = removes(root->right, temp->data);
            }
        }

        if (root == nullptr)
            return root;

        // Update height of the current node
        root->height = 1 + max(height(root->left), height(root->right));

        // Balance the tree using your custom balance function
        balance(root);

        return root;
    }


    

    Node* findMin(Node* t) const {
        if (t == nullptr) {
            return nullptr;
        }
        while (t->left != nullptr) {
            t = t->left;
        }
        return t;
    }

    Node* search(Node* node, int useri) {
        if (node == nullptr || node->data==useri) {
            //cout << "\n" << node->pointer->data<< "\n";
            return node;
        }

        if (useri < node->data) {
            return search(node->left, useri);
        }
        return search(node->right, useri);
    }

    Node* search(Node* node, string useri) {
        if (node == nullptr || node->data == useri) {
            //cout << "\n" << node->pointer->data << "\n";
            return node;
        }

        if (useri < node->data) {
            return search(node->left, useri);
        }
        return search(node->right, useri);
    }

    void inorderTraversal(Node* node) {
        if (node == nullptr) return;

        inorderTraversal(node->left);
        cout <<(node->pointer->data);
        inorderTraversal(node->right);
    }

    Node* copyTree(const Node* source) {
        if (source == nullptr) return nullptr;

        Node* newNode = new Node(source->data);
        newNode->left = copyTree(source->left);
        newNode->right = copyTree(source->right);
        newNode->height = source->height;
        return newNode;
    }

    void deleteTree(Node*& node) {
        if (node) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
            node = nullptr;
        }
    }
    bool printnode(Node* tmp) {
        if (tmp) {
            cout << (tmp->pointer->data) << endl; // Ensure the data is printed correctly
            return true;
        }
        return false;
    }
public:
    avl() : root(nullptr) {}

    avl(const avl& other) {
        root = copyTree(other.root);
    }

    avl& operator=(const avl& other) {
        if (this != &other) {
            deleteTree(root);
            root = copyTree(other.root);
        }
        return *this;
    }

    bool search(int userid) {
        Node* result = search(root, userid);
        return printnode(result);
    }
    bool search(string str) {
        Node* result = search(root, str);
        return printnode(result);
    }
    bool insert(T value, list<user>::mynode*& tm) {
        return insert(root, value,tm);
    }

    void printInorder() {
        inorderTraversal(root);
        cout << endl;
    }

    int getHeight() {
        return height(root);
    }

    bool isEmpty() const {
        return root == nullptr;
    }
    bool remove(int userid,string &name,string &email, string& type, string& country) {
        return remove(root, userid,name,email,type,country);
    }
    bool remove(string str) {
        return removes(root, str);
    }
    ~avl() {
        deleteTree(root);
    }
};
