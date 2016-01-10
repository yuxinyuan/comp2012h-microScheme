#ifndef _CELL_H_
#define _CELL_H_

#include "abstractCell.h"

/**
 * template class for basic values
 * e.g. int double & symbols
 */
template <typename T>
class value_cell: public Cell {
public:
    typedef     T   value_type;
public:
    value_cell() = delete;
    value_cell(value_type v): value_m(v) {}
    value_cell(const value_cell& c) = delete;
    virtual ~value_cell() {}
    value_cell& operator=(const value_cell& c) = delete;

    virtual std::string type() const;
    virtual std::string to_str() const {
        std::stringstream ss;
        ss << value_m;
        return ss.str();
    }
    virtual unsigned int length() const { return 0; }

    value_type get_value() { return value_m; }
private:
    value_type value_m;
};

/* instanciate templated class */
typedef     value_cell<int>             IntCell;
typedef     value_cell<double>          DoubleCell;
typedef     value_cell<std::string>     SymbolCell;
typedef     value_cell<bool>            BoolCell;

/**
 * class cons pair
 * a cons pair is in the form of (x . y)
 * where x & y can be either basic values
 * or cons pair
 */
class ConsCell: public Cell {
public:
    typedef     std::shared_ptr<Cell>   cell_ptr;
public:
    ConsCell() = delete;
    ConsCell(cell_ptr car, cell_ptr cdr): car_m(car), cdr_m(cdr) {}
    ConsCell(const ConsCell& c) = delete;
    virtual ~ConsCell() {}
    ConsCell& operator=(const ConsCell& c) = delete;

    virtual std::string type() const;
    virtual std::string to_str() const;
    virtual unsigned int length() const;

    cell_ptr get_car() const;
    cell_ptr get_cdr() const;
private:
    cell_ptr car_m;
    cell_ptr cdr_m;
};

/**
 * class NilCell
 * nil class only have one instance
 * represent nil in scm
 */
class NilCell: public Cell {
public:
    NilCell();
    NilCell(const NilCell& c) = delete;
    virtual ~NilCell() {}
    NilCell& operator=(const NilCell& c) = delete;

    virtual std::string type() const;
    virtual std::string to_str() const;
    virtual unsigned int length() const;

private:
    static int instance_count;
};

extern std::shared_ptr<Cell> nil;

#endif // _CELL_H_