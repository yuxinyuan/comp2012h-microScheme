#include "cons.h"
#include <iostream>

using namespace std;

int main() {
    shared_ptr<Cell> i = make_int(10);
    shared_ptr<Cell> d = make_double(1.2);
    shared_ptr<Cell> s = make_symbol("hello");
    shared_ptr<Cell> b = make_bool(true);

    cout << boolp(b) << " " << get_bool(b) << " " << b->to_str() << endl;

    cout << intp(i) << " " << get_int(i) << endl;
    cout << doublep(d) << " " << get_double(d) << endl;
    cout << symbolp(s) << " " << get_symbol(s) << endl;

    shared_ptr<Cell> c1 = cons(i, nil);
    shared_ptr<Cell> c2 = cons(c1, nil);
    shared_ptr<Cell> c3 = cons(s, c1);
    shared_ptr<Cell> c4 = cons(c1, c3);
    shared_ptr<Cell> c5 = cons(s, make_symbol("world"));
    shared_ptr<Cell> c6 = cons(i, cons(s, cons(c5, nil)));

    cout << c1->to_str() << endl;
    cout << c2->to_str() << endl;
    cout << c3->to_str() << endl;
    cout << c4->to_str() << endl;
    cout << c5->to_str() << endl;
    cout << c6->to_str() << endl;

    cout << c1->length() << endl;
    cout << c2->length() << endl;
    cout << c3->length() << endl;
    cout << c4->length() << endl;
    cout << c5->type() << endl;
    cout << c6->type() << c6->length() << endl;

    cout << nil->type() << endl;
    cout << nil->to_str() << endl;
    cout << nullp(nil) << endl;

    return 0;
}