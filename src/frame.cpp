#include "frame.h"
#include <utility>
#ifdef DEBUG
    #include <iostream>
#endif

using namespace std;

typedef     std::shared_ptr<frame>                      frame_ptr;
typedef     std::shared_ptr<Cell>                       cell_ptr;
typedef     std::unordered_map<std::string, cell_ptr>   map_t;
typedef     map_t::iterator                             map_iter;

frame_ptr global_frame(new frame());

frame::frame(): parent_m(nullptr) {}

frame::frame(frame_ptr f): parent_m(f) {}

frame::~frame() {
#ifdef DEBUG
    cout << "@@@@@@@@@@@@@@@deleted local frame@@@@@@@@@@@@@@@@@@@" << endl;
    for (auto it = bindings.begin(); it != bindings.end(); ++it) {
        cout << it->first << ": " << it->second->to_str() << endl;
    }
    cout << "@@@@@@@@@@@@@@@deleted local frame@@@@@@@@@@@@@@@@@@@" << endl;
#endif
}

cell_ptr frame::look_up(std::string name) {
    return (_look_up(name))->second;
}

void frame::define(string name, cell_ptr value) {
#ifdef DEBUG
    cout << "defining: " << name << ": " << value->to_str() << endl;
#endif
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
    frame_ptr ret(new frame(parent));
    _bind_vars(formals, args, ret);
    return ret;
}

void frame::_bind_vars(cell_ptr formals, cell_ptr args, frame_ptr f) {
    if (symbolp(formals)) {
        f->define(get_symbol(formals), args);
        return;
    }
    while (!nullp(formals)) {
        f->define(get_symbol(car(formals)), car(args));
        formals = cdr(formals);
        args = cdr(args);
    }
    return;
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