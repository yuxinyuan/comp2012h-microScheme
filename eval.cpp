#include "eval.h"
#include "frame.h"
#include "lambda.h"
#include "syntax.h"

// return second element of a list e.g. y for (x y z)
#define cadr(c) car(cdr(c))
// assert expr, throw a runtime_error with err_msg on failure
#define scm_assert(expr, err_msg) do {if (!(expr)) throw runtime_error(err_msg);} while(false)
// check if condition is #f
#define is_true(condition) (boolp(condition) && get_bool(condition) == false ? false : true)

using namespace std;

static cell_ptr _eval(cell_ptr c, frame_ptr env) {
    auto get_value = [&](cell_ptr cp) ->cell_ptr {
        // get value of a primitive type cell
        if (nullp(cp) || intp(cp) || doublep(cp) || boolp(cp)) { return cp; }
        else if (symbolp(cp)) { return env->look_up(get_symbol(cp)); }
        else { throw runtime_error("non-evaluable type"); }
    };
    if (!listp(c)) {
        return get_value(c);
    }
    else {
        cell_ptr car_cell = car(c);
        if (symbolp(car_cell)) {
            // first check if c is a syntax list
            string op = get_symbol(car_cell);
            if (op == "define") {
                return define_syntax(cdr(c), env);
            }
            else if (op == "set!") {
                return set_syntax(cdr(c), env);
            }
            else if (op == "if") {
                return if_syntax(cdr(c), env);
            }
            else if (op == "and") {
                return and_syntax(cdr(c), env);
            }
            else if (op == "or") {
                return or_syntax(cdr(c), env);
            }
            else if (op == "cond") {
                return cond_syntax(cdr(c), env);
            }
            else if (op == "let") {
                return let_syntax(cdr(c), env);
            }
            else if (op == "begin") {
                return begin_syntax(cdr(c), env);
            }
            else if (op == "lambda") {
                return lambda_syntax(cdr(c), env);
            }
            else if (op == "quote") {
                return quote_syntax(cdr(c));
            }
            else {
                car_cell = get_value(car_cell);
                if (procedurep(car_cell)) {
                    frame_ptr new_frame = frame::make_new_frame(get_formals(car_cell), cdr(c), get_env(car_cell));
                    return begin_syntax(get_body(car_cell), new_frame);
                }
                else {
                    throw runtime_error(car(c)->to_str() + " is not a function");
                }
            }
        }
        else {
            throw runtime_error(car_cell->to_str() + " is not a function");
        }
    }
}

static cell_ptr lambda_syntax(cell_ptr c, frame_ptr env) {
    scm_assert(c->length() >= 2, "invalid lambda syntax");
    cell_ptr formals = car(c);
    cell_ptr body = cdr(c);
    return lambda(formals, body, env);
}

static cell_ptr define_syntax(cell_ptr c, frame_ptr env) {
    cell_ptr define_target = car(c);
    cell_ptr define_tail = cdr(c);
    cell_ptr define_value;
    if (listp(define_target)) {
        // syntax sugar, define a function
        define_value = lambda_syntax(cons(cdr(define_target), define_tail), env);
        define_target = car(define_target);
        scm_assert(symbolp(define_target), "cannot define " + define_target->to_str());
        env->define(get_symbol(define_target), define_value);
        return nil;
    }
    else {
        // define a symbol in current env
        scm_assert(define_tail->length() == 1, "invalid define syntax");
        scm_assert(symbolp(define_target), "cannot define " + define_target->to_str());
        define_value = _eval(car(define_tail), env);
        env->define(get_symbol(define_target), define_value);
        return nil;
    }
}

static cell_ptr set_syntax(cell_ptr c, frame_ptr env) {
    scm_assert(c->length() == 2, "invalid set! syntax");
    scm_assert(symbolp(car(c)), "cannot set " + (car(c))->to_str());
    cell_ptr set_value = _eval(cadr(c), env);
    env->set(get_symbol(car(c)), set_value);
    return nil;
}

static cell_ptr if_syntax(cell_ptr c, frame_ptr env) {
    scm_assert(c->length() == 2 || c->length() == 3, "invalid if syntax");
    cell_ptr condition = _eval(car(c), env);
    cell_ptr clause = cdr(c);
    if (is_true(condition)) {
        // evaluate the then-clause
        return _eval(car(clause), env);
    }
    else {
        // evaluate the else-clause
        return nullp(cdr(clause)) ? nil : _eval(cadr(clause), env);
    }
}

static cell_ptr and_syntax(cell_ptr c, frame_ptr env) {
    cell_ptr ret = make_bool(true);
    while (!nullp(c)) {
        ret = _eval(car(c), env);
        if (!is_true(ret)) { return ret; }
        c = cdr(c);
    }
    return ret;
}

static cell_ptr or_syntax(cell_ptr c, frame_ptr env) {
    cell_ptr ret = make_bool(false);
    while (!nullp(c)) {
        ret = _eval(car(c), env);
        if (is_true(ret)) { return ret; }
        c = cdr(c);
    }
    return ret;
}

static cell_ptr cond_syntax(cell_ptr c, frame_ptr env) {
    cell_ptr val = nil;
    cell_ptr expr;
    while (!nullp(c)) {
        expr = car(c);
        scm_assert(listp(expr) && expr->length() == 2, "invalid cond syntax");
        val = car(expr);
        if (symbolp(val) && get_symbol(val) == "else") {
            val = nil;
        }
        else {
            val = _eval(val, env);
        }
        if (is_true(val)) { return _eval(cadr(expr), env); }
        c = cdr(c);
    }
    return nil;
}

static cell_ptr begin_syntax(cell_ptr c, frame_ptr env) {
    cell_ptr ret = nil;
    while (!nullp(c)) {
        ret = _eval(car(c), env);
        c = cdr(c);
    }
    return ret;
}

static cell_ptr let_syntax(cell_ptr c, frame_ptr env) {
    scm_assert(c->length() >= 2, "invalid let syntax");
    cell_ptr bindings = car(c);
    scm_assert(nullp(bindings) || listp(bindings), "invalid bindings in let syntax");
    frame_ptr new_frame = frame::make_new_frame(nil, nil, env);
    while (!nullp(bindings)) {
        scm_assert(listp(car(bindings)), "invalid bindings in let syntax");
        define_syntax(car(bindings), new_frame);
        bindings = cdr(bindings);
    }
    return begin_syntax(cdr(c), new_frame);
}

static cell_ptr quote_syntax(cell_ptr c) {
    scm_assert(c->length() == 1, "invalid quote syntax");
    return car(c);
}

cell_ptr eval(cell_ptr c) {
    return _eval(c, global_frame);
}