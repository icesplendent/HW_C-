#include <cstring>
#include <iostream>

using namespace std;

class String {
   public:
    // Constructor
    String();
    String(int m);
    String(char *init, int m);

    // Basic
    bool operator==(String t);
    bool operator!();
    int Length();
    String Concat(String t);
    String Substr(int i, int j);
    void FailureFunction();
    int Find(String pat);

    // Additional
    String Delete(int start, int length);
    String CharDelete(char c);
    int Compare(String y);

    // useful tools
    void Print();

   private:
    char *str;
    int strLength;
    int *f;
};

String::String() {
    strLength = 0;
    // str[0] = '\0';
}

String::String(int m) {
    str = new char[m];
    // str[m] = '\0';
    strLength = m;
    f = new int[m];
}

String::String(char *init, int m) {
    str = new char[m];
    f = new int[m];
    for (int i = 0; i < m; i++) {
        str[i] = init[i];
    }
    // str[m] = '\0';
    strLength = m;
}

int String::Length(void) {
    return strLength;
}

// Equality Test
bool String::operator==(String t) {
    if (strlen(t.str) == strLength) {
        for (int i = 0; i < t.strLength; i++) {
            if (t.str[i] != str[i]) return false;
        }
    } else {
        return false;
    }
    return true;
}

// Empty Test
bool String::operator!() {
    if (this->strLength == 0) return true;
    return false;
}

// Concat
String String::Concat(String t) {
    strcpy(str + strLength, t.str);
    strLength += t.strLength;
    return *this;
}

// Substr with i start form zero
String String::Substr(int i, int j) {
    String temp(j);
    strncpy(temp.str, str + i, j);
    return temp;
}

// implemented by FastFind
int String::Find(String pat) {
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
    cout << "FailureFunction: " << endl;
    for (int i = 0; i < strLength; i++) {
        cout << f[i] << " ";
    }
    cout << endl;
}

// "start" is start from "1"
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
    // delete[] str;
    return temp;
}

String String::CharDelete(char c) {
    int count = 0;
    int j = 0;

    for (int i = 0; i < strLength; i++) {
        if (str[i] == c) {
            count++;
        }
    }

    String temp(strLength - count);

    for (int i = 0; i < strLength; i++) {
        if (str[i] != c) {
            temp.str[j++] = str[i];
            // cout << "Hello from charDelete, i = " << i << temp.str[temp.strLength - 1] << endl;
        }
    }
    // temp_str.str[temp_str.strLength] = '\0';
    delete[] str;
    return temp;
}

int String::Compare(String y) {
    int Pos = 0, yPos = 0;

    while (Pos < strLength && yPos < y.strLength) {
        if (str[Pos] != y.str[yPos]) {
            cout << "hello" << endl;
            if (str[Pos] > y.str[yPos]) {
                return 1;

            } else {
                return -1;
            }
        }
        Pos++;
        yPos++;
    }

    if (strLength == y.strLength) {
        return 0;
    } else {
        if (strLength > y.strLength) {
            return 1;
        } else {
            return -1;
        }
    }
}

void String::Print() {
    cout << str << endl;
}

int main() {
    int n, p;
    int start, l;
    char delete_c;
    char strToBeComp[50];
    char str[50];
    char pat[50];

    cout << "String Length:";
    cin >> n;
    cout << "Enter String: ";
    cin >> str;
    String Str(str, n);

    // Test for Equality
    cout << "strToBeCompared Length:";
    cin >> n;
    cout << "Enter strToBeCompared: ";
    cin >> strToBeComp;
    String StrToBeComp(strToBeComp, n);

    switch (Str.Compare(StrToBeComp)) {
        case -1:
            cout << "Str < StrToBeComp" << endl;
            break;
        case 0:
            cout << "Str == StrToBeComp" << endl;
            break;
        case 1:
            cout << "Str > StrToBeComp" << endl;
            break;

        default:
            cout << "what the hell happens" << endl;
            break;
    }
    if (Str == StrToBeComp) {
        cout << "they are the same\n"
             << endl;
    } else {
        cout << "they are not the same\n"
             << endl;
    }

    // Str.FailureFunction();
    // cout << "\n";

    // cout << "Pattern Length:";
    // cin >> p;
    // cout << "Enter Pattern: ";
    // cin >> pat;
    // String Pat(pat, p);
    // Pat.FailureFunction();
    // cout << "Where is matched?" << endl;
    // cout << Str.Find(Pat) << endl;
    // cout << "\n";

    cout << "Start delete from:";
    cin >> start;
    cout << "Delete length:";
    cin >> l;
    String D;
    D = Str.Delete(start, l);
    cout << "After deleting: " << endl;
    D.Print();
    cout << "\n";

    // Test for Empty operator
    if (!D) {
        cout << "the string is empty\n"
             << endl;
    } else {
        cout << "the string is not emtpy with length " << D.Length() << "\n"
             << endl;
    }

    // Test for Concat
    Str.Concat(StrToBeComp);
    cout << "the result of Concat: " << endl;
    Str.Print();

    // Test for SubStr
    cout << "the result of Substr" << endl;
    Str.Substr(1, 3).Print();

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
