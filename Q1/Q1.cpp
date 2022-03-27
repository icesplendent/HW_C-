#include <math.h>

#include <algorithm>
#include <iostream>
using namespace std;

class Polynomial;

class Term {
    friend Polynomial;
    friend ostream &operator<<(ostream &os, Polynomial &a);
    friend istream &operator>>(istream &in, Polynomial &a);

   private:
    float coef;
    int exp;
};

class Polynomial {
   public:
    Polynomial();
    Polynomial(float *coefl, int *expl, int t);
    Polynomial Psort(void);
    Polynomial Add(Polynomial b);
    Polynomial Mult(Polynomial p);
    float Eval(float f);
    void NewTerm(const float theCoeff, const int theExp);
    float Coef(int e);
    int LeadExp();
    friend ostream &operator<<(ostream &os, Polynomial &a);
    friend istream &operator>>(istream &in, Polynomial &a);

   private:
    Term *termArray;
    int capacity;
    int terms;
};

Polynomial::Polynomial() {
    capacity = 1;
    termArray = new Term[capacity];
    terms = 0;
}

Polynomial::Polynomial(float *coefl, int *expl, int t) {
    terms = 0;
    capacity = t;
    termArray = new Term[capacity];
    for (int i = 0; i < t; i++) {
        NewTerm(coefl[i], expl[i]);
    }
}

Polynomial Polynomial::Psort() {
    Polynomial a;
    int exp[terms];
    int i = 0, j = 0;
    int new_term;
    float coef;

    memset(exp, 0, terms);

    for (i = 0; i < terms; i++)
        exp[i] = termArray[i].exp;

    sort(exp, exp + terms, std::greater<int>());

    for (i = 1, j = 0; i < terms; i++) {
        if (exp[i] != exp[j])
            exp[++j] = exp[i];
    }
    new_term = j + 1;

    for (int i = 0; i < new_term; i++) {
        for (j = 0, coef = 0; j < terms; j++) {
            if (termArray[j].exp == exp[i])
                coef += termArray[j].coef;
        }
        a.NewTerm(coef, exp[i]);
    }
    return a;
}

Polynomial Polynomial::Add(Polynomial b) {
    Polynomial c;
    int aPos = 0, bPos = 0;
    while ((aPos < terms) && (bPos < b.terms)) {
        if (termArray[aPos].exp == b.termArray[bPos].exp) {
            float t = termArray[aPos].coef + b.termArray[bPos].coef;
            if (t) c.NewTerm(t, termArray[aPos].exp);
            aPos++;
            bPos++;
        } else if (termArray[aPos].exp < b.termArray[bPos].exp) {
            c.NewTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
            bPos++;
        } else {
            c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
            aPos++;
        }
    }
    for (; aPos < terms; aPos++)
        c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
    for (; bPos < b.terms; bPos++)
        c.NewTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
    c = c.Psort();
    return c;
}

Polynomial Polynomial::Mult(Polynomial p) {
    Polynomial c;
    for (int mPos = 0; mPos < terms; mPos++) {
        for (int pPos = 0; pPos < p.terms; pPos++) {
            c.NewTerm(termArray[mPos].coef * p.termArray[pPos].coef, termArray[mPos].exp + p.termArray[pPos].exp);
        }
    }
    c = c.Psort();
    return c;
}

float Polynomial::Eval(float f) {
    float ans = 0;
    for (int i = 0; i < terms; i++) {
        ans += termArray[i].coef * pow(f, termArray[i].exp);
    }
    return ans;
}

void Polynomial::NewTerm(const float theCoeff, const int theExp) {
    if (terms == capacity) {
        capacity *= 2;
        Term *temp = new Term[capacity];
        copy(termArray, termArray + terms, temp);
        delete[] termArray;
        termArray = temp;
    }
    termArray[terms].coef = theCoeff;
    termArray[terms++].exp = theExp;
}

float Polynomial::Coef(int e) {
    for (int i = 0; i < terms; i++) {
        if (termArray[i].exp == e) {
            return termArray[i].coef;
        }
    }
    return 0;
}

int Polynomial::LeadExp() {
    Psort();
    return termArray[0].exp;
}

istream &operator>>(istream &in, Polynomial &a) {
    int t, e, i;
    float c;
    cout << "Terms: ";
    in >> t;
    float Coef[t];  // list to store coef
    int Exp[t];     // list to store exp

    i = 0;
    while (i < t) {
        cout << "The " << i + 1 << " term: ";
        in >> c >> e;
        // check if the coef is 0 or not
        if (c == 0)
            cout << "Enter again. The coef can't be 0." << endl;
        else {
            Coef[i] = c;
            Exp[i] = e;
            i++;
        }
    }
    a = Polynomial(Coef, Exp, t);
    return in;
}

ostream &operator<<(ostream &os, Polynomial &a) {
    for (int aPos = 0; aPos < a.terms; aPos++) {
        if (a.termArray[aPos].coef > 0 && aPos != 0)
            os << " + " << a.termArray[aPos].coef << "x^(" << a.termArray[aPos].exp << ")";
        else
            os << a.termArray[aPos].coef << "x^(" << a.termArray[aPos].exp << ")";
    }
    return os;
};

int main() {
    Polynomial A, B, C;
    int exponent;
    cout << "Input Polynomial A: coef exp" << endl;
    cin >> A;
    cout << "Input Polynomial B: coef exp" << endl;
    cin >> B;
    A = A.Psort();
    B = B.Psort();
    cout << "A = " << A << " w/ leading exp: " << A.LeadExp() << endl;
    cout << "B = " << B << " w/ leading exp: " << B.LeadExp() << endl;
    cout << "\n";

    cout << "get the coef of the corresponding exp of A(enter the exp): ";
    cin >> exponent;
    cout << "the coef of x^(" << exponent << ") of A is: " << A.Coef(exponent);
    cout << "\n";

    C = A.Add(B);
    cout << "The result of A+B is\n";
    cout << "C = " << C << " w/ leading exp: " << C.LeadExp() << endl;
    cout << "\n";
    C = A.Mult(B);
    cout << "The result of A*B is\n";
    cout << "C = " << C << " w/ leading exp: " << C.LeadExp() << endl;
    cout << "\n";
    float x, value_A, value_B;
    cout << "If x = ";
    cin >> x;
    value_A = A.Eval(x);
    value_B = B.Eval(x);
    cout << "A(" << x << ") = " << value_A << endl;
    cout << "B(" << x << ") = " << value_B << endl;
    return 0;
}

// the execution trace
// first run
/*
3
3 2
1 3
6 5

4
1 0
2 1
3 2
4 3

5

2
*/

// second run
/*
2
1 1
2 2

5
1 0
1 1
1 2
1 3
1 4

1

5
*/
