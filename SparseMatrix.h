#pragma once
#include "Node.h"

class SparseMatrix {
private:
    Node* start;

public:
    SparseMatrix();
    ~SparseMatrix();

    void add(int value, int xPos, int yPos);
    int get(int xPos, int yPos);
    int getNonZeroCount();
    int getMaxColumn();
    void remove(int xPos, int yPos);
    void printStoredValues();
    double density();
    SparseMatrix* multiply(SparseMatrix* other);
};
