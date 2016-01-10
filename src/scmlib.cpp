#include "frame.h"
#include "lambda.h"
#include "cons.h"

using namespace std;

typedef     shared_ptr<Cell>   cell_ptr;
typedef     shared_ptr<frame>  frame_ptr;

cell_ptr operator+(cell_ptr op1, cell_ptr op2) {
    if (intp(op1) && intp(op2)) {
        return make_int(get_int(op1) + get_int(op2));
    }
    else if (doublep(op1) && doublep(op2)) {
        return make_double(get_double(op1) + get_double(op2));
    }
    else if (doublep(op1) && intp(op2)) {
        return make_double(get_double(op1) + get_int(op2));
    }
    else if (intp(op1) && doublep(op2)) {
        return make_double(get_int(op1) + get_double(op2));
    }
    else {
        throw runtime_error("invalid operand for +");
    }
}

cell_ptr operator-(cell_ptr op1, cell_ptr op2) {
    if (intp(op1) && intp(op2)) {
        return make_int(get_int(op1) - get_int(op2));
    }
    else if (doublep(op1) && doublep(op2)) {
        return make_double(get_double(op1) - get_double(op2));
    }
    else if (doublep(op1) && intp(op2)) {
        return make_double(get_double(op1) - get_int(op2));
    }
    else if (intp(op1) && doublep(op2)) {
        return make_double(get_int(op1) - get_double(op2));
    }
    else {
        throw runtime_error("invalid operand for -");
    }
}

cell_ptr operator/(cell_ptr op1, cell_ptr op2) {
    auto div_zero = [](cell_ptr c) ->bool {
        return (intp(c) && get_int(c) == 0) || (doublep(c) && get_double(c) == 0);
    };
    if (div_zero(op2)) {
        throw runtime_error("divided by zero");
    }
    else if (intp(op1) && intp(op2)) {
        return make_int(get_int(op1) / get_int(op2));
    }
    else if (doublep(op1) && doublep(op2)) {
        return make_double(get_double(op1) / get_double(op2));
    }
    else if (doublep(op1) && intp(op2)) {
        return make_double(get_double(op1) / get_int(op2));
    }
    else if (intp(op1) && doublep(op2)) {
        return make_double(get_int(op1) / get_double(op2));
    }
    else {
        throw runtime_error("invalid operand for /");
    }
}

cell_ptr operator*(cell_ptr op1, cell_ptr op2) {
    if (intp(op1) && intp(op2)) {
        return make_int(get_int(op1) * get_int(op2));
    }
    else if (doublep(op1) && doublep(op2)) {
        return make_double(get_double(op1) * get_double(op2));
    }
    else if (doublep(op1) && intp(op2)) {
        return make_double(get_double(op1) * get_int(op2));
    }
    else if (intp(op1) && doublep(op2)) {
        return make_double(get_int(op1) * get_double(op2));
    }
    else {
        throw runtime_error("invalid operand for *");
    }
}

static cell_ptr add(cell_ptr args) {
    cell_ptr ret = make_int(0);
    while (!nullp(args)) {
        ret = ret + car(args);
        args = cdr(args);
    }
    return ret;
}

static cell_ptr sub(cell_ptr args) {
    if (nullp(args)) {
        throw runtime_error("at least one operand for -");
    }
    else {
        cell_ptr ret = make_int(0);
        while (!nullp(args)) {
            ret = ret - car(args);
            args = cdr(args);
        }
        return ret;
    }
}

static cell_ptr div(cell_ptr args) {
    if (nullp(args)) {
        throw runtime_error("at least one operand for /");
    }
    else {
        cell_ptr ret;
        if (args->length() == 1) {
            ret = make_int(1);
        }
        else {
            ret = car(args);
            args = cdr(args);
        }
        while (!nullp(args)) {
            ret = ret / car(args);
            args = cdr(args);
        }
        return ret;
    }
}

static cell_ptr mul(cell_ptr args) {
    cell_ptr ret = make_int(1);
    while (!nullp(args)) {
        ret = ret * car(args);
        args = cdr(args);
    }
    return ret;
}

void register_primitive_function() {
    global_frame->define("+", make_func(add));
    global_frame->define("-", make_func(sub));
    global_frame->define("*", make_func(mul));
    global_frame->define("/", make_func(div));
}