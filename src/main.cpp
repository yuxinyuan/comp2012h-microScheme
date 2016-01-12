#include "parse.h"
#include "eval.h"
#include "scmlib.h"
#include <iostream>

using namespace std;

void read_eval_print() {
    vector<string> tokens;
    shared_ptr<Cell> root;
    shared_ptr<Cell> result;
    do {
        try {
            cout << "scm> ";
            tokens = read_expr();
            root = parse(tokens);
            result = eval(root);
            if (result != nil) {
               cout << result->to_str() << endl;
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

void read_lib() {
    return;
}

int main() {
    // register standard scm library functions
    register_primitive_function();
    // read .scm files in scmlib/
    read_lib();
    read_eval_print();
    return 0;
}