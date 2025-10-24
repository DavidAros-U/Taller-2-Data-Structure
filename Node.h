#pragma once
#include <iostream>
using namespace std;

struct Node {
    int x;
    int y;
    int value;
    Node* next;

    Node(int xPos, int yPos, int val) {
        x = xPos;
        y = yPos;
        value = val;
        next = nullptr;
    }

    ~Node() {
       cout << "Node removed at position (" << x << ", " << y << ")" << endl;
    }
};
