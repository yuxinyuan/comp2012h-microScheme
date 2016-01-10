#include "frame.h"
#include "lambda.h"
#include "cons.h"
#include <functional>

// assert expr, throw a runtime_error with err_msg on failure
#define scm_assert(expr, err_msg) do {if (!(expr)) throw runtime_error(err_msg);} while(false)
// check if condition is #f
#define is_true(condition) (boolp(condition) && get_bool(condition) == false ? false : true)

using namespace std;

typedef     shared_ptr<Cell>   cell_ptr;
typedef     shared_ptr<frame>  frame_ptr;

template <typename func>
cell_ptr operation(cell_ptr op1, cell_ptr op2, string err_msg, func op) {
    if (intp(op1) && intp(op2)) {
        return make_int(op(get_int(op1), get_int(op2)));
    }
    else if (doublep(op1) && doublep(op2)) {
        return make_double(op(get_double(op1), get_double(op2)));
    }
    else if (doublep(op1) && intp(op2)) {
        return make_double(op(get_double(op1), get_int(op2)));
    }
    else if (intp(op1) && doublep(op2)) {
        return make_double(op(get_int(op1), get_double(op2)));
    }
    else {
        throw runtime_error(err_msg);
    }
}

cell_ptr operator+(cell_ptr op1, cell_ptr op2) {
    return operation(op1, op2, "invalid operand for +", plus<double>());
}

cell_ptr operator-(cell_ptr op1, cell_ptr op2) {
    return operation(op1, op2, "invalid operand for -", minus<double>());
}

cell_ptr operator/(cell_ptr op1, cell_ptr op2) {
    auto div_zero = [](cell_ptr c) ->bool {
        return (intp(c) && get_int(c) == 0) || (doublep(c) && get_double(c) == 0);
    };
    if (div_zero(op2)) {
        throw runtime_error("divided by zero");
    }
    return operation(op1, op2, "invalid operand for /", divides<double>());
}

cell_ptr operator*(cell_ptr op1, cell_ptr op2) {
    return operation(op1, op2, "invalid operand for *", multiplies<double>());
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
    scm_assert(!nullp(args), "at least one operand for -");
    cell_ptr ret;
    if (args->length() == 1) {
        ret = make_int(0);
    }
    else {
        ret = car(args);
        args = cdr(args);
    }
    while (!nullp(args)) {
        ret = ret - car(args);
        args = cdr(args);
    }
    return ret;
}

static cell_ptr div(cell_ptr args) {
    scm_assert(!nullp(args), "at least one operand for /");
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

static cell_ptr mul(cell_ptr args) {
    cell_ptr ret = make_int(1);
    while (!nullp(args)) {
        ret = ret * car(args);
        args = cdr(args);
    }
    return ret;
}

static cell_ptr less_than(cell_ptr args) {
    return args;
}

void register_primitive_function() {
    global_frame->define("+", make_func(add));
    global_frame->define("-", make_func(sub));
    global_frame->define("*", make_func(mul));
    global_frame->define("/", make_func(div));
    global_frame->define("<", make_func(less_than));
}