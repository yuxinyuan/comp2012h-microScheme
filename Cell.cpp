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
    cell_ptr next = cdr_m;
    ss << "(";
    do {
        ss << curr->to_str();
        if (next->type() == "cons") {
            ss << " ";
            curr = (static_pointer_cast<ConsCell>(next))->get_car();
            next = (static_pointer_cast<ConsCell>(next))->get_cdr();
        }
        else if (next->type() != "nil") {
            ss << " . " << next->to_str() << ")";
            break;
        }
        else {
            ss << ")";
            break;
        }
    } while (true);

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

/**************************************************************
 ******************* class NilCell ****************************
 *************************************************************/
int NilCell::instance_count = 0;

NilCell::NilCell() {
    if (instance_count == 0) {
        ++instance_count;
    }
    else {
        throw logic_error("try to create another instance of nil");
    }
}

string NilCell::type() const {
    return "nil";
}

string NilCell::to_str() const {
    return "()";
}

unsigned int NilCell::length() const {
    throw logic_error("try to access length() method of nil");
}

shared_ptr<Cell> nil(new NilCell);