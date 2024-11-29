#pragma once
#include <iostream>
#include <cstring>
#include <stdexcept>
using namespace std;



template <typename t>
class list {
private:
    struct node {
        t data;
        node* next;
        node* prev;
        node() : next(nullptr), prev(nullptr) {}
        node(t val, node* nextp = nullptr, node* prevp = nullptr) : data(val), next(nextp), prev(prevp) {}
    };

    class iterator {
    public:
        iterator(node* ptr = nullptr) { iptr = ptr; }
        iterator& operator ++() {
            if (iptr) { iptr = iptr->next; }
            return(*this);
        }

        iterator operator++(int) {
            node* tmp = iptr;
            if (iptr) {
                iptr = iptr->next;
            }
            return tmp;
        }

        iterator& operator --() {
            if (iptr) { iptr = iptr->prev; }
            return(*this);
        }

        iterator operator--(int) {
            node* tmp = iptr;
            if (iptr) {
                iptr = iptr->prev;
            }
            return tmp;
        }

        t& operator* () const {
            return iptr->data;
        }

        bool operator==(const iterator& obj) const {
            return (iptr == obj.iptr);
        }

        bool operator!=(const iterator& obj) const {
            return !(iptr == obj.iptr);
        }

        ~iterator() {
            iptr = nullptr;

        }
        friend class list;

    private:
        node* iptr;
    };
    void copyFrom(const list& other) {
        node* current = other.head->next;
        while (current != other.tail) {
            addatend(current->data);
            current = current->next;
        }
    }
    node* head;
    node* tail;
    int size = 0;

public:
    typedef node mynode;
    typedef iterator itr;
    itr begin() { return head->next; }
    itr end() { return tail; }


    list() {
        head = new node();
        tail = new node();
        head->next = tail;
        tail->prev = head;
    }

    ~list() {
        node* tmp = head;
        while (tmp != nullptr) {
            node* next = tmp->next;
            delete tmp;
            tmp = next;
        }
    }

    list(const list& other) : list() {
        copyFrom(other);
    }

    void clear() {
        node* current = head->next;
        while (current != tail) {
            node* next = current->next;
            delete current;
            current = next;
        }
        head->next = tail;
        tail->prev = head;
        size = 0;
    }

    list& operator=(const list& other) {
        if (this != &other) {
            clear();
            copyFrom(other);
        }
        return *this;
    }
    t* ends(){
        if (size > 0) {
            return &(tail->prev->data);
        }
        return nullptr;
    }
    void printforward() const {
        node* temp = head->next;
        while (temp != tail) {
            cout << temp->data << "\t";
            temp = temp->next;
        }
        cout << endl;
    }

    void printreverse() const {
        node* temp = tail->prev;
        while (temp != head) {
            cout << temp->data << "\t";
            temp = temp->prev;
        }
        cout << endl;
    }

    /*void addatend(t element) {
        node* temp = new node(element, tail, tail->prev);
        tail->prev->next = temp;
        tail->prev = temp;
        size++;
    }*/

    node* addatend(t user1) {
        // Create a new node for the linked list
        node* newNode = new node(user1, tail, tail->prev);

        // Adjust the pointers of the surrounding nodes
        tail->prev->next = newNode;
        tail->prev = newNode;

        // Increment the size of the list
        size++;

        // Return the pointer to the newly created node
        return newNode;
    }

    void insertstart(t  element) {
        node* temp = new node(element, head->next, head);
        head->next->prev = temp;
        head->next = temp;
        size++;
    }

    void deletefromstart() {
        if (head->next != tail) {
            node* temp = head->next;
            head->next = temp->next;
            temp->next->prev = head;
            delete temp;
            size--;
        }
    }

    t deletefromstartt() {
        if (head->next != tail) {
            node* temp = head->next;
            t val = temp->data;
            head->next = temp->next;
            temp->next->prev = head;
            delete temp;
            size--;
            return val;
        }
        return t();
    }

    void deletefromend() {
        if (tail->prev != head) {
            node* temp = tail->prev;
            tail->prev = temp->prev;
            temp->prev->next = tail;
            delete temp;
            size--;
        }
    }

    t deletefromendt() {
        if (tail->prev != head) {
            node* temp = tail->prev;
            t val = temp->data;
            tail->prev = temp->prev;
            temp->prev->next = tail;
            delete temp;
            size--;
            return val;
        }
        return t();
    }

    int getsize() const { return size; }

    node* ReturnMiddle() {
        node* slow = head->next;
        node* fast = head->next;
        while (fast != tail && fast->next != tail) {
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow;
    }

    bool IsEmpty() { return (head->next == tail); }

    void InsertAfter(t val, t key) {
        node* tmp = head->next;
        while (tmp != tail) {
            if (tmp->data == val) {
                node* nodetoadd = new node(key, tmp->next, tmp);
                tmp->next->prev = nodetoadd;
                tmp->next = nodetoadd;
                size++;
                return;
            }
            tmp = tmp->next;
        }
        addatend(key);
    }

    void Insertbefore(t val, t key) {
        node* tmp = head->next;
        while (tmp != tail) {
            if (tmp->data == val) {
                node* nodetoadd = new node(key, tmp, tmp->prev);
                tmp->prev->next = nodetoadd;
                tmp->prev = nodetoadd;
                size++;
                return;
            }
            tmp = tmp->next;
        }
        insertstart(key);
    }

    bool Swap(int leftIndex, int rightIndex) {
        if (leftIndex >= rightIndex || leftIndex < 0 || rightIndex >= size) {
            return false;
        }

        node* tmp1 = head->next;
        node* tmp2 = head->next;

        for (int i = 0; i < leftIndex; i++) {
            tmp1 = tmp1->next;
        }
        for (int i = 0; i < rightIndex; i++) {
            tmp2 = tmp2->next;
        }

        if (tmp1 == nullptr || tmp2 == nullptr) {
            return false;
        }

        node* tmp1prev = tmp1->prev;
        node* tmp1next = tmp1->next;
        node* tmp2prev = tmp2->prev;
        node* tmp2next = tmp2->next;

        if (tmp1next == tmp2) {
            tmp1->next = tmp2next;
            tmp2->prev = tmp1prev;
            tmp1->prev = tmp2;
            tmp2->next = tmp1;
            if (tmp1prev) tmp1prev->next = tmp2;
            if (tmp2next) tmp2next->prev = tmp1;
        }
        else {
            tmp1prev->next = tmp2;
            tmp1next->prev = tmp2;
            tmp2prev->next = tmp1;
            tmp2next->prev = tmp1;

            tmp1->next = tmp2next;
            tmp1->prev = tmp2prev;
            tmp2->next = tmp1next;
            tmp2->prev = tmp1prev;
        }

        return true;
    }

    t& topval() {
        if (tail->prev != head) {
            node* temp = tail->prev;
            return temp->data;
        }
        throw std::out_of_range("List is empty");
    }

    t getnfromtail(int n) {
        if (n <= 0) {
            throw std::invalid_argument("n must be greater than 0");
        }

        node* temp = tail->prev;
        for (int i = 1; i < n; ++i) {
            if (temp->prev == head) {
                throw std::out_of_range("List does not have enough elements");
            }
            temp = temp->prev;
        }
        return temp->data;
    }
    bool deleteuserid(int userid) {
        node* current = head->next;
        while (current != tail) {
            if (current->data.isequal(userid)) {
                // Found the node, now delete it
                cout << "\n\n";
                current->prev->next = current->next;
                current->next->prev = current->prev;
                delete current;
                --size;
                return true; // Successfully deleted
            }
            current = current->next;
        }
        return false; // UserID not found
    }
    iterator erase(iterator pos) {
        if (pos == end()) {
            throw std::out_of_range("Cannot erase end iterator");
        }

        node* temp = pos.iptr;
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
        iterator nextPos(temp->next);
        delete temp;
        --size;
        return nextPos;
    }

};

