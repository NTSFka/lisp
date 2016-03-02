/* ************************************************************************ */
/*                                                                          */
/* The MIT License (MIT)                                                    */
/* Copyright (c) 2016 Jiří Fatka <ntsfka@gmail.com>                         */
/*                                                                          */
/* Permission is hereby granted, free of charge, to any person obtaining    */
/* a copy of this software and associated documentation files (the          */
/* "Software"), to deal in the Software without restriction, including      */
/* without limitation the rights to use, copy, modify, merge, publish,      */
/* distribute, sublicense, and/or sell copies of the Software, and to       */
/* permit persons to whom the Software is furnished to do so, subject to    */
/* the following conditions:                                                */
/*                                                                          */
/* The above copyright notice and this permission notice shall be           */
/* included in all copies or substantial portions of the Software.          */
/*                                                                          */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,          */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF       */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                    */
/* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE   */
/* LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION   */
/* OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION    */
/* WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.          */
/*                                                                          */
/* ************************************************************************ */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

/* ************************************************************************ */

/* LISP */
#include "desc.h"

/* ************************************************************************ */

/**
 * @brief Quit function.
 *
 * @param expr S-expression.
 *
 * @return NORETURN
 */
struct SExpression *func_quit(struct SExpression *expr);

/* ************************************************************************ */

/**
 * @brief Store variable in expression.
 *
 * @param expr S-expression.
 *
 * @return Stored value.
 */
struct SExpression *func_set(struct SExpression *expr);

/* ************************************************************************ */

/**
 * @brief Addition of all values in expression.
 *
 * @param expr S-expression.
 *
 * @return Result value.
 */
struct SExpression *func_add(struct SExpression *expr);

/* ************************************************************************ */

/**
 * @brief Substraction of all values in expression.
 *
 * @param expr S-expression.
 *
 * @return Result value.
 */
struct SExpression *func_sub(struct SExpression *expr);

/* ************************************************************************ */

/**
 * @brief Multiplication of all values in expression.
 *
 * @param expr S-expression.
 *
 * @return Result value.
 */
struct SExpression *func_mult(struct SExpression *expr);

/* ************************************************************************ */

/**
 * @brief Division of all values in expression.
 *
 * @param expr S-expression.
 *
 * @return Result value.
 */
struct SExpression *func_div(struct SExpression *expr);

/* ************************************************************************ */

/**
 * @brief Equation of all values in expression.
 *
 * @param expr S-expression.
 *
 * @return Result value.
 */
struct SExpression *func_eq(struct SExpression *expr);

/* ************************************************************************ */

/**
 * @brief Not-equation of all values in expression.
 *
 * @param expr S-expression.
 *
 * @return Result value.
 */
struct SExpression *func_neq(struct SExpression *expr);

/* ************************************************************************ */

/**
 * @brief QUOTE function.
 *
 * @param expr S-expression.
 *
 * @return Result value.
 */
struct SExpression *func_quote(struct SExpression *expr);

/* ************************************************************************ */

/**
 * @brief LIST function.
 *
 * @param expr S-expression.
 *
 * @return Result value.
 */
struct SExpression *func_list(struct SExpression *expr);

/* ************************************************************************ */

/**
 * @brief CAR function.
 *
 * @param expr S-expression.
 *
 * @return Result value.
 */
struct SExpression *func_car(struct SExpression *expr);

/* ************************************************************************ */

/**
 * @brief CDR function.
 *
 * @param expr S-expression.
 *
 * @return Result value.
 */
struct SExpression *func_cdr(struct SExpression *expr);

/* ************************************************************************ */

/**
 * @brief > function.
 *
 * @param expr S-expression.
 *
 * @return Result value.
 */
struct SExpression *func_gt(struct SExpression *expr);

/* ************************************************************************ */

/**
 * @brief >= function.
 *
 * @param expr S-expression.
 *
 * @return Result value.
 */
struct SExpression *func_ge(struct SExpression *expr);

/* ************************************************************************ */

/**
 * @brief < function.
 *
 * @param expr S-expression.
 *
 * @return Result value.
 */
struct SExpression *func_lt(struct SExpression *expr);

/* ************************************************************************ */

/**
 * @brief <= function.
 *
 * @param expr S-expression.
 *
 * @return Result value.
 */
struct SExpression *func_le(struct SExpression *expr);

/* ************************************************************************ */

#endif /* FUNCTIONS_H_ */

/* ************************************************************************ */
