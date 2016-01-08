#include "parse.h"
#include <iostream>

using namespace std;

void read_eval_print() {
    vector<string> tokens;
    shared_ptr<Cell> root;
    //shared_ptr<Cell> result;
    do {
        try {
            cout << "scm> ";
            tokens = read_expr();
            if (!tokens.empty()) {
                root = parse(tokens);
                cout << root->to_str() << endl;
            }
        }
        catch (runtime_error& e) {
            cout << "ERROR: " << e.what() << endl;
        }
        catch (exception& e) {
            cout << e.what() << endl;
            break;
        }
        
    } while (true); 
}

int main() {
    read_eval_print();
    return 0;
}