#include <iostream>
#include <string>
#include <cctype>

using namespace std;

bool is_double(const string& elem) {
    auto it = elem.begin();
    auto it_end = elem.end();
    if (elem.length() == 1) {
        // length of double must be greater than one.
        return false;
    }
    bool dot = false;
    while (it != it_end) {
        if (isdigit(*it) == 0 && *it != '.') {
            // invalid character.
            return false;
        }
        else if (*it == '.') {
            // dot in a double
            if (!dot) {
                dot = true;
                ++it;
            }
            else {
                return false;
            }
        }
        else {
            ++it;
        }
    }
    return true;
}

int main() {
    string d1 = "1.234";
    string d2 = ".234";
    string d3 = "1.234a";
    string d4 = "1.234.232";

    cout << is_double(d1) << endl;
    cout << is_double(d2) << endl;
    cout << is_double(d3) << endl;
    cout << is_double(d4) << endl;

}