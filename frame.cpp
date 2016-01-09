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
    return (_look_up(name))->second;
}

void frame::define(string name, cell_ptr value) {
    bindings[name] = value;
}

void frame::set(string name, cell_ptr value) {
    (_look_up(name))->second = value;
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

void frame::_bind_vars(cell_ptr formals, cell_ptr args, frame_ptr f) {
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
            throw runtime_error("name " + name + " does not exist in current environment");
        }
    }
}