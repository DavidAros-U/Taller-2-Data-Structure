#include <iostream>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <chrono>
#include <iomanip>
#include "SparseMatrix.h"

using namespace std;

int readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        } else {
            cout << "Invalid input. Please enter an integer." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

void runAutomaticTestsBySize(SparseMatrix& matrix) {
    const int sizes[] = {50, 250, 500, 1000, 5000};
    cout << "\n=== Automatic tests by data size ===" << endl;
    cout << "Choose the number of values:" << endl;
    for (int i = 0; i < 5; i++)
        cout << i + 1 << ". " << sizes[i] << " values" << endl;
    cout << "0. Back to menu" << endl;

    int choice = readInt("Option: ");
    if (choice < 1 || choice > 5) return;

    int numValues = sizes[choice - 1];

    // Preguntar porcentaje de densidad
    cout << "Choose density range:" << endl;
    cout << "1. Less than 40%" << endl;
    cout << "2. More than 70%" << endl;
    int densityChoice = readInt("Option: ");
    double densityFactor = (densityChoice == 1) ? 0.35 : 0.75;

    int rows = 100;
    int cols = 100;
    int maxElements = rows * cols;
    if (numValues > static_cast<int>(maxElements * densityFactor))
        numValues = static_cast<int>(maxElements * densityFactor);

    srand(time(0));
    matrix = SparseMatrix(); // limpiar matriz

    // =============================
    // Inserción
    // =============================
    auto startInsert = chrono::high_resolution_clock::now();
    for (int i = 0; i < numValues; i++) {
        int x = rand() % rows;
        int y = rand() % cols;
        int value = rand() % 100 + 1;
        while (matrix.get(x, y) != 0) {
            x = rand() % rows;
            y = rand() % cols;
        }
        matrix.add(value, x, y);
    }
    auto endInsert = chrono::high_resolution_clock::now();
    chrono::duration<double> insertTime = endInsert - startInsert;

    // =============================
    // Obtención
    // =============================
    auto startGet = chrono::high_resolution_clock::now();
    for (int i = 0; i < numValues; i++) {
        int x = rand() % rows;
        int y = rand() % cols;
        volatile int temp = matrix.get(x, y);
    }
    auto endGet = chrono::high_resolution_clock::now();
    chrono::duration<double> getTime = endGet - startGet;

    // =============================
    // Multiplicación con matriz B
    // =============================
    SparseMatrix B;
    int columnsA = matrix.getMaxColumn() + 1;
    int rowsB = columnsA;
    int colsB = 5;
    int numValuesB = 10;

    for (int i = 0; i < numValuesB; i++) {
        int x, y;
        do {
            x = rand() % rowsB;
            y = rand() % colsB;
        } while (B.get(x, y) != 0);
        int value = rand() % 100 + 1;
        B.add(value, x, y);
    }

    auto startMult = chrono::high_resolution_clock::now();
    SparseMatrix* result = matrix.multiply(&B);
    auto endMult = chrono::high_resolution_clock::now();
    chrono::duration<double> multTime = endMult - startMult;

    // =============================
    // Mostrar matriz resultante
    // =============================
    cout << "\nResulting matrix positions after multiplication:" << endl;
    result->printStoredValues();
    delete result;

    // =============================
    // Mostrar tiempos con decimales
    // =============================
    cout << fixed << setprecision(6); // 6 decimales
    cout << "\n=== RESULTS ===" << endl;
    cout << "Number of values inserted: " << numValues << endl;
    cout << "Density factor used: " << densityFactor * 100 << "%" << endl;
    cout << "Insertion time: " << insertTime.count() << " seconds." << endl;
    cout << "Retrieval time: " << getTime.count() << " seconds." << endl;
    cout << "Multiplication time: " << multTime.count() << " seconds." << endl;
}

void multiplyWithAutomaticB(SparseMatrix& A) {
    if (A.getNonZeroCount() == 0) {
        cout << "Matrix A is empty. Cannot perform multiplication." << endl;
        return;
    }

    int columnsA = A.getMaxColumn() + 1;
    int rowsB = columnsA;
    int colsB = 5;
    int numValuesB = 10;

    SparseMatrix B;

    srand(time(0));
    for (int i = 0; i < numValuesB; i++) {
        int x, y;
        do {
            x = rand() % rowsB;
            y = rand() % colsB;
        } while (B.get(x, y) != 0);

        int value = rand() % 100 + 1;
        B.add(value, x, y);
    }

    cout << "\nMatrix A positions:" << endl;
    A.printStoredValues();
    cout << "\nMatrix B positions:" << endl;
    B.printStoredValues();

    SparseMatrix* result = A.multiply(&B);
    cout << "\nResulting matrix positions:" << endl;
    result->printStoredValues();

    delete result;
}

int main() {
    SparseMatrix matrixA;

    while (true) {
        cout << "\n=== Sparse Matrix Menu ===" << endl;
        cout << "1. Insert a value into matrix A" << endl;
        cout << "2. Get a value from matrix A" << endl;
        cout << "3. Remove a value from matrix A" << endl;
        cout << "4. Print matrix A" << endl;
        cout << "5. Show density of matrix A" << endl;
        cout << "6. Multiply matrix A with automatic matrix B" << endl;
        cout << "7. Run automatic tests by data size" << endl;
        cout << "0. Exit" << endl;

        int option = readInt("Select an option: ");

        switch (option) {
            case 1: {
                int x = readInt("Enter row (X): ");
                int y = readInt("Enter column (Y): ");
                int value = readInt("Enter value: ");
                matrixA.add(value, x, y);
                break;
            }
            case 2: {
                int x = readInt("Enter row (X) to get value: ");
                int y = readInt("Enter column (Y) to get value: ");
                cout << "Value at (" << x << "," << y << "): " << matrixA.get(x, y) << endl;
                break;
            }
            case 3: {
                int x = readInt("Enter row (X) to remove: ");
                int y = readInt("Enter column (Y) to remove: ");
                matrixA.remove(x, y);
                break;
            }
            case 4:
                matrixA.printStoredValues();
                break;
            case 5:
                cout << "Density: " << matrixA.density() << endl;
                break;
            case 6:
                multiplyWithAutomaticB(matrixA);
                break;
            case 7:
                runAutomaticTestsBySize(matrixA);
                break;
            case 0:
                cout << "Exiting..." << endl;
                return 0;
            default:
                cout << "Invalid option. Try again." << endl;
        }
    }
    return 0;
}
