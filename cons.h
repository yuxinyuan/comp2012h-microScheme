#ifndef _CONS_H_
#define _CONS_H_

#include "Cell.h"

//#define nil nullptr
typedef std::shared_ptr<Cell> cell_ptr;
extern cell_ptr nil;

/****************************************************************
 ***********************make cells*******************************
 ***************************************************************/

/**
 * \brief Make an int cell.
 * \param i The initial int value to be stored in the new cell.
 */
inline cell_ptr make_int(const int i)
{
    return cell_ptr(new IntCell(i));
}

/**
 * \brief Make a double cell.
 * \param d The initial double value to be stored in the new cell.
 */
inline cell_ptr make_double(const double d)
{
    return cell_ptr(new DoubleCell(d));
}

/**
 * \brief Make a symbol cell.
 * \param s The initial symbol name to be stored in the new cell.
 */
inline cell_ptr make_symbol(const std::string s)
{
    return cell_ptr(new SymbolCell(s));
}

/**
 * \brief Make a conspair cell.
 * \param my_car The initial car pointer to be stored in the new cell.
 * \param my_cdr The initial cdr pointer to be stored in the new cell.
 */
inline cell_ptr cons(cell_ptr my_car, cell_ptr my_cdr)
{
    return cell_ptr(new ConsCell(my_car, my_cdr));
}


/****************************************************************
 ********************check cell type*****************************
 ***************************************************************/

/**
 * \brief Check if c points to an empty list, i.e., is a null pointer.
 * \return True iff c points to an empty list, i.e., is a null pointer.
 */
inline bool nullp(cell_ptr c)
{
    return (c == nil);
}

/**
 * \brief Check if c points to an int cell.
 * \return True iff c points to an int cell.
 */
inline bool intp(cell_ptr c)
{
    return c->type() == "int";
}

/**
 * \brief Check if c points to a double cell.
 * \return True iff c points to a double cell.
 */
inline bool doublep(cell_ptr c)
{
    return c->type() == "double";
}

/**
 * \brief Check if c points to a symbol cell.
 * \return True iff c points to a symbol cell.
 */
inline bool symbolp(cell_ptr c)
{
    return c->type() == "symbol";
}

/**
 * \brief Check if c points to a list (i.e., nil or a cons cell).
 * \return True iff c points to a list (i.e., nil or a cons cell).
 */
inline bool listp(cell_ptr c)
{
    return c->type() == "cons";
}


/****************************************************************
 ***************************getter*******************************
 ***************************************************************/
/**
 * \brief Accessor (error if c is not an int cell).
 * \return The value in the int cell pointed to by c.
 */
inline int get_int(cell_ptr c)
{
    if (!intp(c)) {
        throw std::runtime_error(c->to_str() + "is not of type int");
    }
    return (std::static_pointer_cast<IntCell>(c))->get_value();
}

/**
 * \brief Accessor (error if c is not a double cell).
 * \return The value in the double cell pointed to by c.
 */
inline double get_double(cell_ptr c)
{
    if (!doublep(c)) {
        throw std::runtime_error(c->to_str() + "is not of type double");
    }
    return (std::static_pointer_cast<DoubleCell>(c))->get_value();
}

/**
 * \brief Retrieve the symbol name as a string (error if c is not a
 * symbol cell).
 * \return The symbol name in the symbol cell pointed to by c.
 */
inline std::string get_symbol(cell_ptr c)
{
    if (!symbolp(c)) {
        throw std::runtime_error(c->to_str() + "is not of type symbol");
    }
    return (std::static_pointer_cast<SymbolCell>(c))->get_value();
}

/**
 * \brief Accessor (error if c is not a cons cell).
 * \return The car pointer in the cons cell pointed to by c.
 */
inline cell_ptr car(cell_ptr c)
{
    if (!listp(c)) {
        throw std::runtime_error(c->to_str() + "is not of type cons");
    }
    return (std::static_pointer_cast<ConsCell>(c))->get_car();
}

/**
 * \brief Accessor (error if c is not a string cell).
 * \return The cdr pointer in the cons cell pointed to by c.
 */
inline cell_ptr cdr(cell_ptr c)
{
    if (!listp(c)) {
        throw std::runtime_error(c->to_str() + "is not of type cons");
    }
    return (std::static_pointer_cast<ConsCell>(c))->get_cdr();
}

#endif // _CONS_H_