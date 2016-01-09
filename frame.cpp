#include "frame.h"
#include <utility>

using namespace std;

typedef     std::shared_ptr<frame>                      frame_ptr;
typedef     std::shared_ptr<Cell>                       cell_ptr;
typedef     std::unordered_map<std::string, cell_ptr>   map_t;
typedef     map_t::iterator                             map_iter;

frame_ptr global_frame(new frame());

frame::frame(): parent_m(nullptr) {}

frame::frame(frame_ptr f): parent_m(f) {}

cell_ptr frame::look_up(std::string name) {
    map_iter result = _look_up(name);
    if (result != bindings.end()) {
        return result->second;
    }
    else {
        return nullptr;
    }
}

void frame::define(string name, cell_ptr value) {
    bindings[name] = value;
}

void frame::set(string name, cell_ptr value) {
    map_iter search = _look_up(name);
    if (search != bindings.end()) {
        search->second = value;
    }
    else {
        throw runtime_error("name " + name + " does not exist in current environment");
    }
}

frame_ptr frame::make_new_frame(cell_ptr formals, cell_ptr args, frame_ptr parent) {
    /**
     * cannot do this !!!
     * frame_ptr parent(this);
     * this will mess up reference count!
     * frame_ptr parent_m = shared_from_this();
     */
    frame_ptr ret(parent);
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

map_iter frame::_look_up(string name) {
    map_iter ret = bindings.find(name);
    if (ret != bindings.end()) {
        // found name
        return ret;
    }
    else {
        if (parent_m) {
            return parent_m->_look_up(name);
        }
        else {
            return bindings.end();
        }
    }
}