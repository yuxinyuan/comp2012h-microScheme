#include "Cell.h"

using namespace std;

/**************************************************************
 ******************* class value_cell *************************
 *************************************************************/
template <>
string value_cell<int>::type() const {
    return "int";
}

template <>
string value_cell<double>::type() const {
    return "double";
}

template <>
string value_cell<string>::type() const {
    return "symbol";
}

/**************************************************************
 ******************* class ConsCell ***************************
 *************************************************************/
typedef     shared_ptr<Cell>        cell_ptr;
typedef     shared_ptr<ConsCell>    cons_ptr;

string ConsCell::type() const {
    return "cons";
}

string ConsCell::to_str() const {
    stringstream ss;
    cell_ptr curr = car_m;
    cons_ptr next = cdr_m && cdr_m->type() == "cons" ? static_pointer_cast<ConsCell>(cdr_m) : nullptr;
    ss << "(";
    do {
        ss << curr->to_str();
        curr = next ? next->get_car() : nullptr;
        next = next && next->get_cdr() && (next->get_cdr())->type() == "cons" ? 
                static_pointer_cast<ConsCell>(next->get_cdr()) : nullptr;
        if (curr) {
            ss << " ";
        }
    } while (curr);
    ss << ")";
    return ss.str();
}

unsigned int ConsCell::length() const {
    if (cdr_m && cdr_m->type() == "cons") {
        return 1 + cdr_m->length();
    }
    else {
        return 1;
    }
}

cell_ptr ConsCell::get_car() const {
    return car_m;
}

cell_ptr ConsCell::get_cdr() const {
    return cdr_m;
}