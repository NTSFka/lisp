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

#ifndef DESC_H_
#define DESC_H_

/* ************************************************************************ */

/**
 * @brief Maximum length of stored value.
 */
#ifndef MAX_VALUE_LENGTH
#define MAX_VALUE_LENGTH 30
#endif

/* ************************************************************************ */

/* Expression type */
enum Type
{
    TYPE_UNDEF,
    TYPE_SEXPR,
    TYPE_VALUE,
    TYPE_NIL,
    TYPE_QUOTED
};

/* ************************************************************************ */

/**
 * @brief Structure for storing a single S-expression
 */
struct SExpression
{
    /** A pointer to next expression. */
    struct SExpression *right;

    /** Stored expression value. */
    char lvalue[MAX_VALUE_LENGTH];

    /** Expression type. */
    enum Type type;
};

/* ************************************************************************ */

#ifndef NDEBUG

/**
 * @brief Counter for allocated S-expressions. At the end it should be zero.
 */
extern int sexpr_count;

#endif

/* ************************************************************************ */

/**
 * @brief Create a new S-expression of given type.
 *
 * Allocated object must be freed by calling `free_sexpr` function.
 *
 * Function never returns NULL pointer. If memory cannot be allocated, it
 * prints error to stderr and exit application with EXIT_FAILURE code.
 *
 * @param type S-expression type.
 *
 * @return Allocated object.
 */
struct SExpression *alloc_sexpr(enum Type type);

/* ************************************************************************ */

/**
 * @brief Free S-expression object.
 *
 * @param expr Object.
 */
void free_sexpr(struct SExpression *expr);

/* ************************************************************************ */

/**
 * @brief Prints S-expression to stdout.
 *
 * @param expr S-expression object.
 */
void print_sexpr(struct SExpression *expr);

/* ************************************************************************ */

#endif /* DESC_H_ */

/* ************************************************************************ */
