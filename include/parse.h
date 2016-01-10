#ifndef _PARSE_H_
#define _PARSE_H_

#include "cons.h"
#include <string>
#include <vector>
#include <memory>

/** 
 * Parse s-expression into a cell.
 *
 * A valid s-expression can be a numeric
 * value, a valid symbol or in the form of
 * (car, cdr), where car, cdr can be a 
 * s-expression.
 */
std::shared_ptr<Cell> parse(const std::vector<std::string>& tokens);

/**
 * read in s-expression from stdin
 * parse the tokens and return a vector
 * of tokens
 */
std::vector<std::string> read_expr();

#endif // _PARSE_H_