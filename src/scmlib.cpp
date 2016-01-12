#include "frame.h"
#include "lambda.h"
#include "eval.h"
#include <functional>

// return second element of a list e.g. y for (x y z)
#define cadr(c) car(cdr(c))
// assert expr, throw a runtime_error with err_msg on failure
#define scm_assert(expr, err_msg) do {if (!(expr)) throw runtime_error(err_msg);} while(false)
// check if condition is #f
#define is_true(condition) (boolp(condition) && get_bool(condition) == false ? false : true)

using namespace std;

typedef     shared_ptr<Cell>   cell_ptr;
typedef     shared_ptr<frame>  frame_ptr;

// very ugly!!! avoid twice eval of function value
extern cell_ptr function_call(cell_ptr func, cell_ptr args);

template <typename func1, typename func2>
cell_ptr operation(cell_ptr op1, cell_ptr op2, string err_msg, func1 int_op, func2 double_op) {
    if (intp(op1) && intp(op2)) {
        return int_op(get_int(op1), get_int(op2));
    }
    else if (doublep(op1) && doublep(op2)) {
        return double_op(get_double(op1), get_double(op2));
    }
    else if (doublep(op1) && intp(op2)) {
        return double_op(get_double(op1), get_int(op2));
    }
    else if (intp(op1) && doublep(op2)) {
        return double_op(get_int(op1), get_double(op2));
    }
    else {
        throw runtime_error(err_msg);
    }
}

cell_ptr operator+(cell_ptr op1, cell_ptr op2) {
    return operation(op1, op2, "invalid operand for +", 
        [](int x, int y) ->cell_ptr {
            return make_int(x + y);
        },
        [](double x, double y) ->cell_ptr {
            return make_double(x + y);
        });
}

cell_ptr operator-(cell_ptr op1, cell_ptr op2) {
    return operation(op1, op2, "invalid operand for -",
        [](int x, int y) ->cell_ptr {
            return make_int(x - y);
        },
        [](double x, double y) ->cell_ptr {
            return make_double(x - y);
        });
}

cell_ptr operator/(cell_ptr op1, cell_ptr op2) {
    auto div_zero = [](cell_ptr c) ->bool {
        return (intp(c) && get_int(c) == 0) || (doublep(c) && get_double(c) == 0);
    };
    if (div_zero(op2)) {
        throw runtime_error("divided by zero");
    }
    return operation(op1, op2, "invalid operand for /",
        [](int x, int y) ->cell_ptr {
            return make_int(x / y);
        },
        [](double x, double y) ->cell_ptr {
            return make_double(x / y);
        });
}

cell_ptr operator*(cell_ptr op1, cell_ptr op2) {
    return operation(op1, op2, "invalid operand for *",
        [](int x, int y) ->cell_ptr {
            return make_int(x * y);
        },
        [](double x, double y) ->cell_ptr {
            return make_double(x * y);
        });
}

cell_ptr operator<(cell_ptr op1, cell_ptr op2) {
    return operation(op1, op2, "invalid operand for <",
        [](int x, int y) ->cell_ptr {
            return make_bool(x < y);
        },
        [](double x, double y) ->cell_ptr {
            return make_bool(x < y);
        });
}

cell_ptr operator>(cell_ptr op1, cell_ptr op2) {
    return operation(op1, op2, "invalid operand for >",
        [](int x, int y) ->cell_ptr {
            return make_bool(x > y);
        },
        [](double x, double y) ->cell_ptr {
            return make_bool(x > y);
        });
}

cell_ptr operator==(cell_ptr op1, cell_ptr op2) {
    return operation(op1, op2, "invalid operand for =",
        [](int x, int y) ->cell_ptr {
            return make_bool(x == y);
        },
        [](double x, double y) ->cell_ptr {
            return make_bool(x == y);
        });
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
    scm_assert(!nullp(args), "at least one arguments for -");
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
    scm_assert(!nullp(args), "at least one arguments for /");
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
    scm_assert(args->length() >= 2, "at least two arguments for <");
    cell_ptr ret = car(args);
    args = cdr(args);
    while (!nullp(args)) {
        ret = ret < car(args);
        if (!is_true(ret)) { return make_bool(false); }
        ret = car(args);
        args = cdr(args);
    }
    return make_bool(true);
}

static cell_ptr greater_than(cell_ptr args) {
    scm_assert(args->length() >= 2, "at least two arguments for >");
    cell_ptr ret = car(args);
    args = cdr(args);
    while (!nullp(args)) {
        ret = ret > car(args);
        if (!is_true(ret)) { return make_bool(false); }
        ret = car(args);
        args = cdr(args);
    }
    return make_bool(true);
}

static cell_ptr equal(cell_ptr args) {
    scm_assert(args->length() >= 2, "at least two arguments for =");
    cell_ptr ret = car(args);
    args = cdr(args);
    while (!nullp(args)) {
        ret = ret == car(args);
        if (!is_true(ret)) { return make_bool(false); }
        ret = car(args);
        args = cdr(args);
    }
    return make_bool(true);
}

static cell_ptr is_null(cell_ptr args) {
    scm_assert(args->length() == 1, "null? expect 1 argument");
    return make_bool(nullp(car(args)));
}

static cell_ptr is_number(cell_ptr args) {
    scm_assert(args->length() == 1, "number? expect 1 argument");
    return make_bool(intp(car(args)) || doublep(car(args)));
}

static cell_ptr scm_car(cell_ptr args) {
    scm_assert(args->length() == 1, "car expect 1 argument");
    scm_assert(listp(car(args)) || pairp(car(args)), "invalid operand for car");
    return car(car(args));
}

static cell_ptr scm_cdr(cell_ptr args) {
    scm_assert(args->length() == 1, "cdr expect 1 argument");
    scm_assert(listp(car(args)) || pairp(car(args)), "invalid operand for cdr");
    return cdr(car(args));
}

static cell_ptr scm_cons(cell_ptr args) {
    scm_assert(args->length() == 2, "cons expect 2 argument");
    return cons(car(args), cadr(args));
}

static cell_ptr scm_eval(cell_ptr args) {
    scm_assert(args->length() == 1, "eval expect 1 argument");
    return eval(car(args));
}

static cell_ptr scm_apply(cell_ptr args) {
    scm_assert(args->length() == 2, "apply expect 2 argument");
    scm_assert(procedurep(car(args)) || primitivep(car(args)), car(args)->to_str() + " is not a function");
    return function_call(car(args), cadr(args));
}

void register_primitive_function() {
    global_frame->define("+", make_func(add));
    global_frame->define("-", make_func(sub));
    global_frame->define("*", make_func(mul));
    global_frame->define("/", make_func(div));
    global_frame->define("<", make_func(less_than));
    global_frame->define(">", make_func(greater_than));
    global_frame->define("=", make_func(equal));
    global_frame->define("null?", make_func(is_null));
    global_frame->define("number?", make_func(is_number));
    global_frame->define("car", make_func(scm_car));
    global_frame->define("cdr", make_func(scm_cdr));
    global_frame->define("cons", make_func(scm_cons));
    global_frame->define("eval", make_func(scm_eval));
    global_frame->define("apply", make_func(scm_apply));
}