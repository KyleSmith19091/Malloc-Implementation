#include <cstddef>
#include <string>
#include <iostream>
#include <iomanip>

#include "../include/MemoryManager.hpp"

using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;
};

int main() {

    MemoryManager manager;
    int* myNumber = (int*)manager.alloc(sizeof(int)); // 3
    *myNumber = 99;

    int* myNumber2 = (int*)manager.alloc(sizeof(int)); // 3
    *myNumber2 = 87;

    Node* root = (Node*)manager.alloc(sizeof(Node)); // 5
    root->data = 123;
    root->left = new Node{ 1, nullptr, nullptr };
    root->right = nullptr;

    //manager.free(new int);
    manager.free(myNumber);

    manager.printHeap();
    manager.dumpHeap();

    return 0;
}
