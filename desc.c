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
#include "desc.h"

/* C library */
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* ************************************************************************ */

#ifndef NDEBUG

/* Default initialization */
int sexpr_count = 0;

#endif

/* ************************************************************************ */

struct SExpression *alloc_sexpr(enum Type type)
{
    struct SExpression *expr;

    /* Try to allocate memory for S-expression */
    expr = malloc(sizeof(struct SExpression));

    /* Unable to allocate memory */
    if (expr == NULL)
    {
        perror("S-expression allocation fail");
        exit(EXIT_FAILURE);
    }

    /* Initialize expression */
    expr->lvalue[0] = '\0';
    expr->right = NULL;
    expr->type = type;

#ifndef NDEBUG
    /* Increase expression counter */
    sexpr_count++;
#endif

    return expr;
}

/* ************************************************************************ */

void free_sexpr(struct SExpression *expr)
{
    /* Pointer must be "valid" */
    assert(expr);

    /* Recursive free */
    if (expr->right)
        free_sexpr(expr->right);

    /** Free expression */
    free(expr);

#ifndef NDEBUG
    /* Decrease counter */
    sexpr_count--;
#endif

}

/* ************************************************************************ */

void print_sexpr(struct SExpression *expr)
{
    /* Must be "valid" expression */
    assert(expr);

    if (expr->type == TYPE_NIL)
    {
        /* Print NIL type */
        printf("NIL");
    }
    else if (expr->type == TYPE_VALUE)
    {
        /* Print value */
        printf("%s", expr->lvalue);
    }
    else
    {
        struct SExpression *tmp = expr;

        printf("(");

        /* Foreach */
        for (; tmp != NULL; tmp = tmp->right)
        {
            if (tmp != expr)
                printf(" ");

            /* Empty value doesn't make sense */
            assert(strlen(tmp->lvalue) != 0);

            printf("%s", tmp->lvalue);
        }

        printf(")");
    }
}

/* ************************************************************************ */
