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

#ifndef INTERPRET_H_
#define INTERPRET_H_

/* ************************************************************************ */

/* C library */
#include <stdio.h>

/* LISP */
#include "desc.h"

/* ************************************************************************ */

/**
 * @brief Function pointer type for evaluation function.
 *
 * @param expr S-expression.
 *
 * @return Result S-expression.
 */
typedef struct SExpression *(*func_t)(struct SExpression *expr);

/* ************************************************************************ */

/**
 * @brief Function pointer type for arithmetic functions.
 *
 * @param argc Number of arguments.
 * @param argv Array of arguments.
 *
 * @return Result value.
 */
typedef int (*arithm_func_t)(unsigned int argc, int* argv);

/* ************************************************************************ */

/**
 * @brief Syntax error function.
 *
 * @param err Error string.
 *
 * @return NORETURN
 */
void syntax_error(const char* err);

/* ************************************************************************ */

/**
 * @brief Eval whole file.
 *
 * @param file Source file. Can be stdin.
 */
void eval_file(FILE *file);

/* ************************************************************************ */

/**
 * @brief Evaluate one "line" from current file and print result to stdout.
 *
 * @return If there is no more expression. When line is evaluated function
 *         returns 0.
 */
int eval_line(void);

/* ************************************************************************ */

/**
 * @brief Evaluate list.
 *
 * @param quoted List type.
 *
 * @return Result S-expression.
 */
struct SExpression *eval_list(enum Type quoted);

/* ************************************************************************ */

/**
 * @brief Evaluate whole S-expression.
 *
 * @param expr Source S-expression.
 *
 * @return Result S-expression.
 */
struct SExpression *eval_sexpr(struct SExpression *expr);

/* ************************************************************************ */

/**
 * @brief Set variable value.
 *
 * If variable with same name exists, value is overwritten.
 *
 * @param name  Variable name.
 * @param value Variable value.
 */
void set_variable(const char* name, int value);

/* ************************************************************************ */

/**
 * @brief Check if there is a variable with given name.
 *
 * @param name Variable name.
 *
 * @return If variable exists.
 */
int has_variable(const char* name);

/* ************************************************************************ */

/**
 * @brief Returns variable value.
 *
 * @param name Variable name.
 *
 * @return Variable value or 0 if variable doesn't exists.
 */
int get_variable(const char* name);

/* ************************************************************************ */

/**
 * @brief Removes variable.
 *
 * @param name Variable name.
 */
void unset_variable(const char* name);

/* ************************************************************************ */

/**
 * @brief Cleanup interpreter.
 *
 * Removes all variables from memory.
 */
void clean_up(void);

/* ************************************************************************ */

/**
 * @brief Helper function for arithmetic operations.
 *
 * @param expr Source S-expression.
 * @param func Arithmetic function pointer.
 *
 * @note `func` function cannot exit application because it will cause
 * memory-leak for arguments.
 *
 * @return Result S-expression.
 */
struct SExpression *func_arithm_base(struct SExpression *expr, arithm_func_t func);

/* ************************************************************************ */

#endif /* INTERPRET_H_ */

/* ************************************************************************ */
