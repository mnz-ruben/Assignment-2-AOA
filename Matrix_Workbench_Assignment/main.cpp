#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <limits>
using namespace std;

const int MAX_SIZE = 10;
const int MAX_IDS = 100;

struct Matrix {
    int values[MAX_SIZE][MAX_SIZE];
    int original[MAX_SIZE][MAX_SIZE];
    int rows;
    int cols;
    int id;
    char name;
    bool exists;
};

struct MultiplyResult {
    int result[MAX_SIZE][MAX_SIZE];
    int rows;
    int cols;
    long long scalarMultiplications;
    long long additions;
};

struct ScalarResult {
    long long scalarMultiplications;
    long long writes;
};

int usedIDs[MAX_IDS];
int usedIDCount = 0;

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void pauseProgram() {
    cout << "Press any key to return to Main Menu!";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

int getIntInRange(const string &prompt, int minValue, int maxValue) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;

        if (!cin.fail() && value >= minValue && value <= maxValue) {
            clearInput();
            return value;
        }

        cout << "Invalid input. Please enter a number from " << minValue << " to " << maxValue << ".\n";
        clearInput();
    }
}

int getInt(const string &prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;

        if (!cin.fail()) {
            clearInput();
            return value;
        }

        cout << "Invalid input. Please enter an integer.\n";
        clearInput();
    }
}

bool idAlreadyUsed(int id) {
    for (int i = 0; i < usedIDCount; i++) {
        if (usedIDs[i] == id) {
            return true;
        }
    }
    return false;
}

int generateUniqueID() {
    int id;
    do {
        id = 10000 + rand() % 90000;
    } while (idAlreadyUsed(id));

    if (usedIDCount < MAX_IDS) {
        usedIDs[usedIDCount] = id;
        usedIDCount++;
    }

    return id;
}

void copyMatrix(int dest[][MAX_SIZE], const int src[][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dest[i][j] = src[i][j];
        }
    }
}

void displayMatrix(const int mat[][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << setw(6) << mat[i][j];
        }
        cout << endl;
    }
}

void createMatrix(Matrix &m, char name) {
    m.name = name;
    m.rows = getIntInRange("Enter number of rows for Matrix " + string(1, name) + " (1-10): ", 1, 10);
    m.cols = getIntInRange("Enter number of columns for Matrix " + string(1, name) + " (1-10): ", 1, 10);

    cout << "Choose input method:\n";
    cout << "1- Manual\n";
    cout << "2- Random\n";
    int choice = getIntInRange("Choice: ", 1, 2);

    if (choice == 1) {
        for (int i = 0; i < m.rows; i++) {
            for (int j = 0; j < m.cols; j++) {
                m.values[i][j] = getInt("Enter " + string(1, name) + "[" + to_string(i) + "][" + to_string(j) + "]: ");
            }
        }
    } else {
        int minValue = getInt("Enter minimum random value: ");
        int maxValue = getInt("Enter maximum random value: ");

        while (minValue > maxValue) {
            cout << "Minimum value cannot be greater than maximum value. Try again.\n";
            minValue = getInt("Enter minimum random value: ");
            maxValue = getInt("Enter maximum random value: ");
        }

        for (int i = 0; i < m.rows; i++) {
            for (int j = 0; j < m.cols; j++) {
                m.values[i][j] = minValue + rand() % (maxValue - minValue + 1);
            }
        }
    }

    copyMatrix(m.original, m.values, m.rows, m.cols);
    m.id = generateUniqueID();
    m.exists = true;

    cout << "Great! Matrix " << name << " has been created.\n";
    cout << "Matrix " << name << " ID: " << m.id << endl;
    cout << "Order: " << m.rows << " x " << m.cols << endl;
    displayMatrix(m.values, m.rows, m.cols);
}

bool canMultiply(const Matrix &A, const Matrix &B) {
    return A.exists && B.exists && A.cols == B.rows;
}

MultiplyResult multiplyMatrices(const Matrix &A, const Matrix &B) {
    MultiplyResult output;
    output.rows = A.rows;
    output.cols = B.cols;
    output.scalarMultiplications = 0;
    output.additions = 0;

    for (int i = 0; i < A.rows; i++) {
        for (int j = 0; j < B.cols; j++) {
            int sum = 0;
            for (int k = 0; k < A.cols; k++) {
                int product = A.values[i][k] * B.values[k][j];
                output.scalarMultiplications++;

                if (k == 0) {
                    sum = product;
                } else {
                    sum += product;
                    output.additions++;
                }
            }
            output.result[i][j] = sum;
        }
    }

    return output;
}

ScalarResult scalarMultiply(Matrix &m, int k) {
    ScalarResult result;
    result.scalarMultiplications = 0;
    result.writes = 0;

    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            m.values[i][j] = m.values[i][j] * k;
            result.scalarMultiplications++;
            result.writes++;
        }
    }

    return result;
}

void restoreMatrix(Matrix &m) {
    if (m.exists) {
        copyMatrix(m.values, m.original, m.rows, m.cols);
    }
}

void showMenu() {
    cout << "\n*************** Matrix Workbench ***************\n";
    cout << "1- Create Matrix A\n";
    cout << "2- Create Matrix B\n";
    cout << "3- Display current matrices\n";
    cout << "4- Check multiplication compatibility\n";
    cout << "5- Perform A x B\n";
    cout << "6- Perform B x A\n";
    cout << "7- Multiply a matrix by a scalar\n";
    cout << "8- Restore original matrices\n";
    cout << "0- Exit\n";
}

void displayCurrentMatrices(const Matrix &A, const Matrix &B) {
    if (!A.exists) {
        cout << "Matrix A has not yet been created.\n";
    } else {
        cout << "Matrix A ID: " << A.id << endl;
        cout << "Order: " << A.rows << " x " << A.cols << endl;
        displayMatrix(A.values, A.rows, A.cols);
    }

    cout << endl;

    if (!B.exists) {
        cout << "Matrix B has not yet been created.\n";
    } else {
        cout << "Matrix B ID: " << B.id << endl;
        cout << "Order: " << B.rows << " x " << B.cols << endl;
        displayMatrix(B.values, B.rows, B.cols);
    }
}

void compatibilityReport(const Matrix &A, const Matrix &B) {
    cout << "------ Compatibility Report ------\n";

    if (!A.exists || !B.exists) {
        if (!A.exists) cout << "Matrix A is missing.\n";
        if (!B.exists) cout << "Matrix B is missing.\n";
        return;
    }

    cout << "Matrix A Order: " << A.rows << " x " << A.cols << endl;
    cout << "Matrix B Order: " << B.rows << " x " << B.cols << endl;

    if (A.cols == B.rows) {
        cout << "A x B: Valid\n";
        cout << "Result Order: " << A.rows << " x " << B.cols << endl;
    } else {
        cout << "A x B: Not defined\n";
    }

    if (B.cols == A.rows) {
        cout << "B x A: Valid\n";
        cout << "Result Order: " << B.rows << " x " << A.cols << endl;
    } else {
        cout << "B x A: Not defined\n";
    }

    cout << "Note: Even when both products are defined, matrix multiplication is generally not commutative.\n";
    cout << "Theoretical Complexity:\n";
    cout << "Time Complexity: O(1)\n";
    cout << "Auxiliary Space: O(1)\n";
    cout << "----------------------------------\n";
}

void multiplicationReport(const Matrix &first, const Matrix &second, char firstName, char secondName) {
    if (!first.exists || !second.exists) {
        if (!first.exists) cout << "Matrix " << firstName << " is missing.\n";
        if (!second.exists) cout << "Matrix " << secondName << " is missing.\n";
        return;
    }

    if (first.cols != second.rows) {
        cout << "Matrix " << firstName << " x Matrix " << secondName << " is not defined because Matrix "
             << firstName << " has " << first.cols << " columns but Matrix " << secondName
             << " has " << second.rows << " rows.\n";
        return;
    }

    MultiplyResult result = multiplyMatrices(first, second);

    cout << "------ Matrix Multiplication Report ------\n";
    cout << "Matrix " << firstName << ":\n";
    displayMatrix(first.values, first.rows, first.cols);
    cout << "Matrix " << secondName << ":\n";
    displayMatrix(second.values, second.rows, second.cols);
    cout << firstName << " x " << secondName << ":\n";
    displayMatrix(result.result, result.rows, result.cols);
    cout << "Result Order: " << result.rows << " x " << result.cols << endl;
    cout << "Scalar Multiplications: " << result.scalarMultiplications << endl;
    cout << "Additions: " << result.additions << endl;
    cout << "Theoretical Complexity:\n";

    if (firstName == 'A' && secondName == 'B') {
        cout << "Time Complexity: O(rA x cA x cB)\n";
        cout << "For square matrices n x n: O(n^3)\n";
        cout << "Auxiliary Space: O(rA x cB)\n";
    } else {
        cout << "Time Complexity: O(rB x cB x cA)\n";
        cout << "For square matrices n x n: O(n^3)\n";
        cout << "Auxiliary Space: O(rB x cA)\n";
    }

    cout << "----------------------------------------\n";
}

void scalarMultiplyMenu(Matrix &A, Matrix &B) {
    if (!A.exists && !B.exists) {
        cout << "Neither matrix exists yet. Create Matrix A or Matrix B first.\n";
        return;
    }

    cout << "Select matrix:\n";
    cout << "1- Matrix A\n";
    cout << "2- Matrix B\n";
    int choice = getIntInRange("Choice: ", 1, 2);

    Matrix *selected = nullptr;
    char selectedName;

    if (choice == 1) {
        if (!A.exists) {
            cout << "Matrix A has not yet been created.\n";
            return;
        }
        selected = &A;
        selectedName = 'A';
    } else {
        if (!B.exists) {
            cout << "Matrix B has not yet been created.\n";
            return;
        }
        selected = &B;
        selectedName = 'B';
    }

    int before[MAX_SIZE][MAX_SIZE];
    copyMatrix(before, selected->values, selected->rows, selected->cols);

    int k = getInt("Enter scalar value: ");
    ScalarResult result = scalarMultiply(*selected, k);

    cout << "Before Scalar Multiplication:\n";
    displayMatrix(before, selected->rows, selected->cols);
    cout << "Scalar Value: " << k << endl;
    cout << "After Scalar Multiplication:\n";
    displayMatrix(selected->values, selected->rows, selected->cols);
    cout << "Scalar Multiplications: " << result.scalarMultiplications << endl;
    cout << "Writes: " << result.writes << endl;
    cout << "Theoretical Complexity:\n";
    cout << "Time Complexity: O(r x c)\n";
    cout << "Auxiliary Space: O(1)\n";
}

void restoreMatrices(Matrix &A, Matrix &B) {
    if (!A.exists && !B.exists) {
        cout << "No matrices exist yet. Nothing can be restored.\n";
        return;
    }

    if (A.exists) {
        restoreMatrix(A);
    }

    if (B.exists) {
        restoreMatrix(B);
    }

    cout << "Original matrices restored successfully!\n";

    if (A.exists) {
        cout << "Matrix A ID: " << A.id << endl;
        cout << "Order: " << A.rows << " x " << A.cols << endl;
        cout << "Matrix A Restored:\n";
        displayMatrix(A.values, A.rows, A.cols);
    }

    if (B.exists) {
        cout << "Matrix B ID: " << B.id << endl;
        cout << "Order: " << B.rows << " x " << B.cols << endl;
        cout << "Matrix B Restored:\n";
        displayMatrix(B.values, B.rows, B.cols);
    }
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    Matrix A = {{{0}}, {{0}}, 0, 0, 0, 'A', false};
    Matrix B = {{{0}}, {{0}}, 0, 0, 0, 'B', false};

    int choice;

    do {
        showMenu();
        choice = getIntInRange("Choice: ", 0, 8);
        cout << endl;

        switch (choice) {
            case 1:
                createMatrix(A, 'A');
                pauseProgram();
                break;
            case 2:
                createMatrix(B, 'B');
                pauseProgram();
                break;
            case 3:
                displayCurrentMatrices(A, B);
                pauseProgram();
                break;
            case 4:
                compatibilityReport(A, B);
                pauseProgram();
                break;
            case 5:
                multiplicationReport(A, B, 'A', 'B');
                pauseProgram();
                break;
            case 6:
                multiplicationReport(B, A, 'B', 'A');
                cout << "If both A x B and B x A exist, the program does not assume the results are equal.\n";
                pauseProgram();
                break;
            case 7:
                scalarMultiplyMenu(A, B);
                pauseProgram();
                break;
            case 8:
                restoreMatrices(A, B);
                pauseProgram();
                break;
            case 0:
                cout << "Thank you for using the Matrix Workbench!\n";
                cout << "Goodbye!\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}
