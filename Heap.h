#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


template <typename T>
class MinHeap {
private:
    vector<T> heap;

    int parent(int index) { return (index - 1) / 2; }
    int leftChild(int index) { return (2 * index) + 1; }
    int rightChild(int index) { return (2 * index) + 2; }

    void heapifyUp(int index) {
        while (index > 0 && heap[parent(index)] > heap[index]) {
            swap(heap[parent(index)], heap[index]);
            index = parent(index);
        }
    }

    void heapifyDown(int index) {
        int smallest = index;
        int left = leftChild(index);
        int right = rightChild(index);

        if (left < heap.size() && heap[left] < heap[smallest]) {
            smallest = left;
        }
        if (right < heap.size() && heap[right] < heap[smallest]) {
            smallest = right;
        }
        if (smallest != index) {
            swap(heap[index], heap[smallest]);
            heapifyDown(smallest);
        }
    }

public:
    MinHeap() {}

    MinHeap(T* array, int size) {
        for (int i = 0; i < size; ++i) {
            heap.push_back(array[i]);
        }
        for (int i = parent(heap.size() - 1); i >= 0; --i) {
            heapifyDown(i);
        }
    }

    void insert(const T& element) {
        heap.push_back(element);
        heapifyUp(heap.size() - 1);
    }

    T getMin() const {
        if (isEmpty()) {
            throw runtime_error("Heap is empty.");
        }
        return heap[0];
    }

    void deleteMin() {
        if (isEmpty()) {
            throw runtime_error("Heap is empty.");
        }
        heap[0] = heap.back();
        heap.pop_back();
        heapifyDown(0);
    }

    bool isEmpty() const {
        return heap.empty();
    }

    int getSize() const {
        return heap.size();
    }

    void display() const {
        for (const T& element : heap) {
            cout << element.getUserType() << " - " << element.getComplaintCount() << " complaints" << endl;
        }
    }
};