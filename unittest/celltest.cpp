#include "cons.h"
#include <iostream>

using namespace std;

int main() {
    shared_ptr<Cell> i = make_int(10);
    shared_ptr<Cell> d = make_double(1.2);
    shared_ptr<Cell> s = make_symbol("hello");

    cout << intp(i) << " " << get_int(i) << endl;
    cout << doublep(d) << " " << get_double(d) << endl;
    cout << symbolp(s) << " " << get_symbol(s) << endl;

    shared_ptr<Cell> c1 = cons(i, nullptr);
    shared_ptr<Cell> c2 = cons(c1, nullptr);
    shared_ptr<Cell> c3 = cons(s, c1);
    shared_ptr<Cell> c4 = cons(c1, c3);

    cout << c1->to_str() << endl;
    cout << c2->to_str() << endl;
    cout << c3->to_str() << endl;
    cout << c4->to_str() << endl;

    cout << car(c1)->to_str() << endl;
    cout << car(c2)->to_str() << endl;
    cout << car(c3)->to_str() << endl;
    cout << car(c4)->to_str() << endl;

    return 0;
}