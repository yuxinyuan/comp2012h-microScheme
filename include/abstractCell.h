#ifndef _ABSTRACT_CELL_H_
#define _ABSTRACT_CELL_H_

#include <string>
#include <sstream>
#include <memory>

/**
 * abstract class Cell
 * define basic interface of derived classes
 */
class Cell {
public:
    Cell() {}
    Cell(const Cell& c) = delete;
    virtual ~Cell() {}
    Cell& operator=(const Cell& c) = delete;

    virtual std::string type() const = 0;
    virtual std::string to_str() const = 0;
    virtual unsigned int length() const = 0;
};

#endif // _ABSTRACT_CELL_H_