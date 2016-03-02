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

/* Declaration */
#include "functions.h"

/* C library */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/* LISP */
#include "interpret.h"

/* ************************************************************************ */

/**
 * @brief Addition arithmetic function.
 *
 * @param argc Number of arguments.
 * @param argv Array of arguments.
 *
 * @return Result value.
 */
static int f_add(unsigned int argc, int* argv)
{
    unsigned int i;
    int result = 0;

    for (i = 0; i < argc; i++)
        result += argv[i];

    return result;
}

/* ************************************************************************ */

/**
 * @brief Substraction arithmetic function.
 *
 * @param argc Number of arguments.
 * @param argv Array of arguments.
 *
 * @return Result value.
 */
static int f_sub(unsigned int argc, int* argv)
{
    unsigned int i;
    int result;

    if (argc == 0)
        return 0;

    result = argv[0];

    for (i = 1; i < argc; i++)
        result -= argv[i];

    return result;
}

/* ************************************************************************ */

/**
 * @brief Multiplication arithmetic function.
 *
 * @param argc Number of arguments.
 * @param argv Array of arguments.
 *
 * @return Result value.
 */
static int f_mult(unsigned int argc, int* argv)
{
    unsigned int i;
    int result = 1;

    for (i = 0; i < argc; i++)
        result *= argv[i];

    return result;
}

/* ************************************************************************ */

/**
 * @brief Division arithmetic function.
 *
 * @param argc Number of arguments.
 * @param argv Array of arguments.
 *
 * @return Result value.
 */
static int f_div(unsigned int argc, int* argv)
{
    unsigned int i;
    int result;

    if (argc == 0)
        return 0;

    result = argv[0];

    for (i = 1; i < argc; i++)
        result /= argv[i];

    return result;
}

/* ************************************************************************ */

/**
 * @brief Equation arithmetic function.
 *
 * @param argc Number of arguments.
 * @param argv Array of arguments.
 *
 * @return Result value.
 */
static int f_eq(unsigned int argc, int* argv)
{
    unsigned int i;
    int result = 1;

    for (i = 1; i < argc; i++)
        result &= (argv[0] == argv[i]);

    return result;
}

/* ************************************************************************ */

/**
 * @brief Non-equation arithmetic function.
 *
 * @param argc Number of arguments.
 * @param argv Array of arguments.
 *
 * @return Result value.
 */
static int f_neq(unsigned int argc, int* argv)
{
    unsigned int i;
    int result = 1;

    for (i = 1; i < argc; i++)
        result &= (argv[0] != argv[i]);

    return result;
}

/* ************************************************************************ */

/**
 * @brief Greater than function.
 *
 * @param argc Number of arguments.
 * @param argv Array of arguments.
 *
 * @return Result value.
 */
static int f_gt(unsigned int argc, int* argv)
{
    unsigned int i;
    int result = 1;

    for (i = 1; i < argc; i++)
        result &= (argv[i - 1] > argv[i]);

    return result;
}

/* ************************************************************************ */

/**
 * @brief Greater equals function.
 *
 * @param argc Number of arguments.
 * @param argv Array of arguments.
 *
 * @return Result value.
 */
static int f_ge(unsigned int argc, int* argv)
{
    unsigned int i;
    int result = 1;

    for (i = 1; i < argc; i++)
        result &= (argv[i - 1] >= argv[i]);

    return result;
}

/* ************************************************************************ */

/**
 * @brief Less than function.
 *
 * @param argc Number of arguments.
 * @param argv Array of arguments.
 *
 * @return Result value.
 */
static int f_lt(unsigned int argc, int* argv)
{
    unsigned int i;
    int result = 1;

    for (i = 1; i < argc; i++)
        result &= (argv[i - 1] < argv[i]);

    return result;
}

/* ************************************************************************ */

/**
 * @brief Less equals function.
 *
 * @param argc Number of arguments.
 * @param argv Array of arguments.
 *
 * @return Result value.
 */
static int f_le(unsigned int argc, int* argv)
{
    unsigned int i;
    int result = 1;

    for (i = 1; i < argc; i++)
        result &= (argv[i - 1] <= argv[i]);

    return result;
}

/* ************************************************************************ */

/**
 * @brief Converts expression to boolean result.
 *
 * @param expr Source expression.
 *
 * @return Modified expression.
 */
struct SExpression* to_bool(struct SExpression* expr)
{
    if (expr->lvalue[0] == '1')
    {
        strcpy(expr->lvalue, "T");
        expr->type = TYPE_VALUE;
    }
    else
    {
        expr->lvalue[0] = '\0';
        expr->type = TYPE_NIL;
    }

    return expr;
}

/* ************************************************************************ */

struct SExpression *func_quit(struct SExpression *expr)
{
    /* Memory is freed by atexit callback (clean_up) */
    exit(EXIT_SUCCESS);
}

/* ************************************************************************ */

struct SExpression *func_set(struct SExpression *expr)
{
    if (!expr->right)
        syntax_error("Missing variable name");

    if (!expr->right->right)
        syntax_error("Missing variable value");

    if (expr->right->right->type != TYPE_VALUE)
        syntax_error("Invalid value type");

    /* Store variable value */
    set_variable(expr->right->lvalue, atoi(expr->right->right->lvalue));

    /* Modify initial expression */
    expr->type = TYPE_VALUE;
    strcpy(expr->lvalue, expr->right->right->lvalue);

    /* Delete arguments parts */
    free_sexpr(expr->right);
    expr->right = NULL;

    /* Return set value */
    return expr;
}

/* ************************************************************************ */

struct SExpression *func_add(struct SExpression *expr)
{
    return func_arithm_base(expr, f_add);
}

/* ************************************************************************ */

struct SExpression *func_sub(struct SExpression *expr)
{
    return func_arithm_base(expr, f_sub);
}

/* ************************************************************************ */

struct SExpression *func_mult(struct SExpression *expr)
{
    return func_arithm_base(expr, f_mult);
}

/* ************************************************************************ */

struct SExpression *func_div(struct SExpression *expr)
{
    return func_arithm_base(expr, f_div);
}

/* ************************************************************************ */

struct SExpression *func_eq(struct SExpression *expr)
{
    return to_bool(func_arithm_base(expr, f_eq));
}

/* ************************************************************************ */

struct SExpression *func_neq(struct SExpression *expr)
{
    return to_bool(func_arithm_base(expr, f_neq));
}

/* ************************************************************************ */

struct SExpression *func_quote(struct SExpression *expr)
{
    struct SExpression *tmp = expr;

    for (; tmp != NULL; tmp = tmp->right)
        tmp->type = TYPE_QUOTED;

    return expr;
}

/* ************************************************************************ */

struct SExpression *func_list(struct SExpression *expr)
{
    /* Return list without the first value */
    struct SExpression *res = expr->right;
    res->type = TYPE_SEXPR;

    expr->right = NULL;
    free_sexpr(expr);

    return res;
}

/* ************************************************************************ */

struct SExpression *func_car(struct SExpression *expr)
{
    /* Return list without the first value */
    struct SExpression *res = expr->right;
    res->type = TYPE_VALUE;

    /* Free rest of list */
    if (res->right)
    {
        free_sexpr(res->right);
        res->right = NULL;
    }

    /* Free beginning */
    expr->right = NULL;
    free_sexpr(expr);

    return res;
}

/* ************************************************************************ */

struct SExpression *func_cdr(struct SExpression *expr)
{
    /* Return list without the first value */
    struct SExpression *res = expr->right->right;
    res->type = TYPE_SEXPR;

    expr->right->right = NULL;
    free_sexpr(expr);

    return res;
}

/* ************************************************************************ */

struct SExpression *func_gt(struct SExpression *expr)
{
    return to_bool(func_arithm_base(expr, f_gt));
}

/* ************************************************************************ */

struct SExpression *func_ge(struct SExpression *expr)
{
    return to_bool(func_arithm_base(expr, f_ge));
}

/* ************************************************************************ */

struct SExpression *func_lt(struct SExpression *expr)
{
    return to_bool(func_arithm_base(expr, f_lt));
}

/* ************************************************************************ */

struct SExpression *func_le(struct SExpression *expr)
{
    return to_bool(func_arithm_base(expr, f_le));
}

/* ************************************************************************ */
