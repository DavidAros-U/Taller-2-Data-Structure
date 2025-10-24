#include "SparseMatrix.h"
#include "Node.h"
#include <iostream>
#include <algorithm>
using namespace std;

SparseMatrix::SparseMatrix() {
    start = nullptr;
}

SparseMatrix::~SparseMatrix() {
    Node* current = start;
    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
}

void SparseMatrix::add(int value, int xPos, int yPos) {
    if (value == 0) return;

    Node* prev = nullptr;
    Node* current = start;

    while (current != nullptr && (current->x < xPos || (current->x == xPos && current->y < yPos))) {
        prev = current;
        current = current->next;
    }

    if (current != nullptr && current->x == xPos && current->y == yPos) {
        current->value = value;
        return;
    }

    Node* newNode = new Node(xPos, yPos, value);

    if (prev == nullptr) {
        newNode->next = start;
        start = newNode;
    } else {
        prev->next = newNode;
        newNode->next = current;
    }
}

int SparseMatrix::get(int xPos, int yPos) {
    Node* current = start;
    while (current != nullptr) {
        if (current->x == xPos && current->y == yPos)
            return current->value;
        current = current->next;
    }
    return 0;
}

int SparseMatrix::getNonZeroCount() {
    int count = 0;
    Node* current = start;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}

int SparseMatrix::getMaxColumn() {
    int maxCol = 0;
    Node* current = start;
    while (current != nullptr) {
        if (current->y > maxCol)
            maxCol = current->y;
        current = current->next;
    }
    return maxCol;
}


void SparseMatrix::remove(int xPos, int yPos) {
    Node* prev = nullptr;
    Node* current = start;

    while (current != nullptr && (current->x < xPos || (current->x == xPos && current->y < yPos))) {
        prev = current;
        current = current->next;
    }
    if (current != nullptr && current->x == xPos && current->y == yPos) {
        if (prev == nullptr) {
            start = current->next;
        } else {
            prev->next = current->next;
        }
        delete current;
    }
}

void SparseMatrix::printStoredValues() {
    Node* current = start;
    while (current != nullptr) {
        cout << "(" << current->x << ", " << current->y << ") --> " << current->value << endl;
        current = current->next;
    }
}

double SparseMatrix::density() {
    if (start == nullptr) return 0.0;

    int count = 0;
    int maxX = 0, maxY = 0;
    Node* current = start;

    while (current != nullptr) {
        count++;
        maxX = max(maxX, current->x);
        maxY = max(maxY, current->y);
        current = current->next;
    }
    int totalSize = (maxX + 1) * (maxY + 1);
    return (double)count / totalSize;
}

SparseMatrix* SparseMatrix::multiply(SparseMatrix* other) {
    SparseMatrix* result = new SparseMatrix();

    for (Node* a = start; a != nullptr; a = a->next) {
        for (Node* b = other->start; b != nullptr; b = b->next) {
            if (a->y == b->x) {
                int existing = result->get(a->x, b->y);
                result->add(existing + a->value * b->value, a->x, b->y);
            }
        }
    }
    return result;
}
