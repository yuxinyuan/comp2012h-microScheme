#include "frame.h"

using namespace std;

typedef     std::shared_ptr<frame>  frame_ptr;
typedef     std::shared_ptr<Cell>   cell_ptr;

frame_ptr global_frame(new frame());

frame::frame(): parent(nullptr) {}

frame::frame(frame_ptr f): parent(f) {}

cell_ptr frame::look_up(std::string name) {
    if (bindings.count(name) != 0) {
        return bindings[name];
    }
    else if (parent) {
        return parent->look_up(name);
    }
    else {
        return nullptr;
    }
}

void frame::define(string name, cell_ptr value) {
    bindings[name] = value;
}

frame_ptr frame::make_new_frame(cell_ptr formals, cell_ptr args) {
    /**
     * cannot do this !!!
     * frame_ptr parent(this);
     * this will mess up reference count!
     */
    frame_ptr parent = shared_from_this();
    frame_ptr ret(new frame(parent));
    _bind_vars(formals, args, ret);
    return ret;
}

/**
 * check if both formals and args are of type list
 * and both formals and args have the same length
 * throw runtime_error on failure
 */
static void check_formals(cell_ptr formals, cell_ptr args) {
    if (listp(formals) && listp(args)) {
        unsigned int formals_length = formals->length();
        unsigned int args_length = args->length();

        if (formals_length == args_length) {
            return;
        }
        else {
            stringstream err_msg;
            err_msg << "expect " << formals_length << " but receive " << args_length;
            throw runtime_error(err_msg.str());
        }
    }
    else if (nullp(formals) && nullp(args)) {
        return;
    }
    else {
        throw runtime_error("formals and args must be lists");
    }
}

void frame::_bind_vars(cell_ptr formals, cell_ptr args, frame_ptr f) {
    check_formals(formals, args);
    while (!nullp(formals)) {
        f->define(get_symbol(car(formals)), car(args));
        formals = cdr(formals);
        args = cdr(args);
    }
}
