#include <math.h>

#include <algorithm>
#include <iostream>
using namespace std;

class SparseMatrix;

class MatrixTerm {
    friend SparseMatrix;
    friend ostream &operator<<(ostream &os, SparseMatrix &a);
    friend istream &operator>>(istream &in, SparseMatrix &a);

   private:
    int row, col, value;
};

class SparseMatrix {
   public:
    SparseMatrix();
    SparseMatrix(int r, int c);
    SparseMatrix(int r, int c, int t);

    SparseMatrix FastTranspose();
    SparseMatrix Add(SparseMatrix b);
    SparseMatrix Mult(SparseMatrix b);
    void ChangeSize1D(const int newSize);
    void StoreSum(const int sum, const int r, const int c);
    int get_value(int r, int c);

    friend ostream &operator<<(ostream &os, SparseMatrix &a);
    friend istream &operator>>(istream &in, SparseMatrix &a);

   private:
    int rows, cols, terms, capacity;
    MatrixTerm *smArray;
    // rows is the number of rows in the matrix
    // cols is the number of columns in the matrix
    // terms is the total number of nonzero entries
    // capacity is the size of the smArray
};

SparseMatrix::SparseMatrix() {
    rows = 0;
    cols = 0;
}

SparseMatrix::SparseMatrix(int r, int c) {
    rows = r;
    cols = c;
}

SparseMatrix::SparseMatrix(int r, int c, int t) {
    rows = r;
    cols = c;
    terms = t;
    capacity = t;
    smArray = new MatrixTerm[capacity];
}

SparseMatrix SparseMatrix::FastTranspose() {
    SparseMatrix b(cols, rows, terms);
    if (terms > 0) {
        int *rowSize = new int[cols];
        int *rowStart = new int[cols];
        fill(rowSize, rowSize + cols, 0);
        for (int i = 0; i < terms; i++)
            rowSize[smArray[i].col]++;
        rowStart[0] = 0;
        for (int i = 1; i < cols; i++)
            rowStart[i] = rowStart[i - 1] + rowSize[i - 1];
        for (int i = 0; i < terms; i++) {
            int j = rowStart[smArray[i].col];
            b.smArray[j].row = smArray[i].col;
            b.smArray[j].col = smArray[i].row;
            b.smArray[j].value = smArray[i].value;
            rowStart[smArray[i].col]++;
        }

        delete[] rowSize;
        delete[] rowStart;
    }
    return b;
}

SparseMatrix SparseMatrix::Add(SparseMatrix b) {
    if (rows != b.rows || cols != b.cols) {
        throw "Addition doesn't work!!";
    } else {
        SparseMatrix c(rows, cols);
        c.capacity = terms + b.terms;
        c.smArray = new MatrixTerm[c.capacity];
        int aPos = 0, bPos = 0, cPos = 0;
        while ((aPos < terms) && (bPos < b.terms)) {
            if (smArray[aPos].row == b.smArray[bPos].row && smArray[aPos].col < b.smArray[bPos].col) {
                c.smArray[cPos] = smArray[aPos];
                aPos++;
                cPos++;
            } else if (smArray[aPos].row == b.smArray[bPos].row && smArray[aPos].col > b.smArray[bPos].col) {
                c.smArray[cPos] = b.smArray[bPos];
                bPos++;
                cPos++;
            } else if (smArray[aPos].row == b.smArray[bPos].row && smArray[aPos].col == b.smArray[bPos].col) {
                c.smArray[cPos].row = smArray[aPos].row;
                c.smArray[cPos].col = smArray[aPos].col;
                c.smArray[cPos].value = smArray[aPos].value + b.smArray[bPos].value;
                aPos++;
                bPos++;
                cPos++;
            } else if (smArray[aPos].row < b.smArray[bPos].row) {
                c.smArray[cPos] = smArray[aPos];
                aPos++;
                cPos++;
            } else {  // smArray[aPos].row > b.smArray[bPos].row
                c.smArray[cPos] = b.smArray[bPos];
                bPos++;
                cPos++;
            }
        }
        for (; aPos < terms; aPos++)
            c.smArray[cPos++] = smArray[aPos];
        for (; bPos < b.terms; bPos++)
            c.smArray[cPos++] = b.smArray[bPos];
        c.terms = cPos;
        return c;
    }
}

SparseMatrix SparseMatrix::Mult(SparseMatrix b) {
    if (cols != b.rows)
        throw "Incompatible matrices";
    SparseMatrix bXpose = b.FastTranspose();
    SparseMatrix d(rows, b.cols, 0);
    int currRowIndex = 0,
        currRowBegin = 0,
        currRowA = smArray[0].row;

    if (terms == capacity)
        ChangeSize1D(terms + 1);
    bXpose.ChangeSize1D(bXpose.terms + 1);
    smArray[terms].row = rows;
    bXpose.smArray[b.terms].row = b.cols;
    bXpose.smArray[b.terms].col = -1;
    int sum = 0;
    while (currRowIndex < terms) {
        int currColB = bXpose.smArray[0].row;
        int currColIndex = 0;
        while (currColIndex <= b.terms) {
            if (smArray[currRowIndex].row != currRowA) {
                d.StoreSum(sum, currRowA, currColB);
                sum = 0;
                currRowIndex = currRowBegin;
                while (bXpose.smArray[currColIndex].row == currColB)
                    currColIndex++;
                currColB = bXpose.smArray[currColIndex].row;
            } else if (bXpose.smArray[currColIndex].row != currColB) {
                d.StoreSum(sum, currRowA, currColB);
                sum = 0;
                currRowIndex = currRowBegin;
                currColB = bXpose.smArray[currColIndex].row;
            } else if (smArray[currRowIndex].col < bXpose.smArray[currColIndex].col)
                currRowIndex++;
            else if (smArray[currRowIndex].col == bXpose.smArray[currColIndex].col) {
                sum += smArray[currRowIndex].value *
                       bXpose.smArray[currColIndex].value;
                currRowIndex++;
                currColIndex++;
            } else
                currColIndex++;
        }
        while (smArray[currRowIndex].row == currRowA)
            currRowIndex++;
        currRowBegin = currRowIndex;
        currRowA = smArray[currRowIndex].row;
    }
    return d;
}

void SparseMatrix::ChangeSize1D(const int newSize) {
    if (newSize < terms)
        throw "New size must be >= number of terms";
    MatrixTerm *temp = new MatrixTerm[newSize];
    copy(smArray, smArray + terms, temp);
    delete[] smArray;
    smArray = temp;
    capacity = newSize;
}

void SparseMatrix::StoreSum(const int sum, const int r, const int c) {
    if (sum != 0) {
        if (terms == capacity)
            ChangeSize1D(2 * capacity);
        smArray[terms].row = r;
        smArray[terms].col = c;
        smArray[terms++].value = sum;
    }
}

int SparseMatrix::get_value(int r, int c) {
    int val = 0;
    for (int i = 0; i < terms; i++) {
        if (smArray[i].row == r && smArray[i].col == c) {
            return smArray[i].value;
        }
    }
    return val;
}

ostream &operator<<(ostream &os, SparseMatrix &a) {
    for (int r = 0; r < a.rows; r++) {
        for (int c = 0; c < a.cols; c++) {
            os << a.get_value(r, c) << " ";
        }
        cout << endl;
    }
    return os;
}

istream &operator>>(istream &in, SparseMatrix &a) {
    for (int i = 0; i < a.terms; i++) {
        in >> a.smArray[i].row >> a.smArray[i].col >> a.smArray[i].value;
    }
    return in;
}

int main() {
    int rows, cols, terms;
    cout << "A = " << endl;
    cin >> rows >> cols >> terms;
    SparseMatrix A(rows, cols, terms);
    cin >> A;
    cout << "Matrix A" << endl;
    cout << A << endl;

    cout << "B = " << endl;
    cin >> rows >> cols >> terms;
    SparseMatrix B(rows, cols, terms);
    cin >> B;
    cout << "Matrix B" << endl;
    cout << B << endl;

    SparseMatrix C;
    C = A.Add(B);
    cout << "A + B = " << endl;
    cout << C << endl;

    C = A.Mult(B);
    cout << "A * B = " << endl;
    cout << C << endl;
    return 0;
}
/*
sample input1
3 3 5
0 0 1
0 2 8
1 1 6
2 1 2
2 2 8

3 3 4
0 0 2
1 0 1
1 2 3
2 1 8
*/
/*
sample input2
5 5 4
0 3 5
1 4 9
2 5 3
4 3 2

6 6 3
0 0 1
1 1 1
2 2 1

*/
