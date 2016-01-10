#ifndef _EVAL_H_
#define _EVAL_H_

#include "cons.h"

/**
 * \brief Evaluate the expression tree whose root is pointed to by c
 * \return The value resulting from evaluating the expression.
 */
std::shared_ptr<Cell> eval(std::shared_ptr<Cell> c);

#endif // _EVAL_H_