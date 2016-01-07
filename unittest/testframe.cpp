#include "frame.h"
#include <iostream>

using namespace std;

typedef     std::shared_ptr<frame>  frame_ptr;
typedef     std::shared_ptr<Cell>   cell_ptr;

int main() {
    frame_ptr f(new frame(global_frame));
    global_frame->define("a", make_int(3));

    cout << f->look_up("a")->to_str() << endl;

    frame_ptr nf = f->make_new_frame(
        cons(make_symbol("x"), cons(make_symbol("y"), nil)),
        cons(make_int(1), cons(make_int(2), nil)));

    cout << nf->look_up("a")->to_str() << endl;

    nf->define("a", make_double(0.5));
    cout << nf->look_up("a")->to_str() << endl;

    cout << nf->look_up("x")->to_str() << endl;
    cout << nf->look_up("y")->to_str() << endl;

    return 0;
}