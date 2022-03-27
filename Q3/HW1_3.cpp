#include <cstring>
#include <iostream>

using namespace std;

class String {
   public:
    String();
    String(int m);
    String(char *init, int m);
    int FastFind(String pat);
    void FailureFunction();
    String Delete(int start, int length);
    String CharDelete(char c);
    int Length();
    void Print();

   private:
    char *str;
    int strLength;
    int *f;
};

String::String() {
    strLength = 0;
}

String::String(int m) {
    str = new char[m];
    f = new int[m];
}

String::String(char *init, int m) {
    str = new char[m];
    f = new int[m];
    for (int i = 0; i < m; i++) {
        str[i] = init[i];
    }
    strLength = m;
}

int String::Length(void) {
    int length;
    length = strlen(str) - 1;
    return strLength;
}

int String::FastFind(String pat) {
    int posP = 0, posS = 0;
    int lengthP = pat.Length();
    int lengthS = Length();
    while ((posP < lengthP) && (posS < lengthS)) {
        if (pat.str[posP] == str[posS]) {
            posP++;
            posS++;
        } else {
            if (posP == 0)
                posS++;
            else
                posP = pat.f[posP - 1] + 1;
        }
    }
    if (posP < lengthP)
        return -1;
    else  // Textbook stops pattern matching once a match is found
        return posS - lengthP;
}

void String::FailureFunction() {
    int lengthP = Length();
    f[0] = -1;
    for (int j = 1; j < lengthP; j++) {
        int i = f[j - 1];
        while ((str[j] != str[i + 1]) && (i >= 0)) {
            i = f[i];
        }
        if (str[j] == str[i + 1])
            f[j] = i + 1;
        else
            f[j] = -1;
    }
    cout << "FailureFx: " << endl;
    for (int i = 0; i < strLength; i++) {
        cout << f[i];
    }
    cout << endl;
}

String String::Delete(int start, int length) {
    int temp_len = strLength - length;
    int temp_i = 0;
    String temp(temp_len);
    start = start - 1;
    for (int i = 0; i < strLength; i++) {
        if (i < start || i > start + length - 1) {
            temp.str[temp_i] = str[i];
            temp_i++;
        }
    }
    delete[] str;
    return temp;
}

String String::CharDelete(char c) {
    String temp_str;
    for (int i = 0; i < strLength; i++) {
        if (str[i] != c) {
            temp_str.str[temp_str.strLength++] = str[i];
        }
    }
    return temp_str;
}

void String::Print() {
    cout << str << endl;
}

int main() {
    int n, p;
    int start, l;
    char delete_c;
    char str[50];
    char pat[50];

    cout << "String Length:";
    cin >> n;
    cout << "Enter String: ";
    cin >> str;
    String Str(str, n);
    Str.FailureFunction();
    cout << "\n";

    cout << "Pattern Length:";
    cin >> p;
    cout << "Enter Pattern: ";
    cin >> pat;
    String Pat(pat, n);
    Pat.FailureFunction();
    cout << "Where is matched?" << endl;
    cout << Str.FastFind(Pat) << endl;
    cout << "\n";

    // cout << "Start delete from:";
    // cin >> start;
    // cout << "Delete length:";
    // cin >> l;
    // String D;
    // D = Str.Delete(start, l);
    // cout << "After deleting: " << endl;
    // D.Print();
    // cout << "\n";

    cout << "Delete which char:";
    cin >> delete_c;
    String DC;
    DC = Str.CharDelete(delete_c);
    cout << "After deleting " << delete_c << ":" << endl;
    DC.Print();

    return 0;
}

/*
sample input 1
10
abcababca
3
bca
2
6
b

*/

/*
sample input 1
20
aqwesdrggaaqweseswrf
3
gga
4
10
e

*/
