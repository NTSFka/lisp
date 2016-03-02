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
#include "interpret.h"

/* C library */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

/* LISP */
#include "tokenizer.h"
#include "functions.h"

/* ************************************************************************ */

/**
 * @brief Maximum length of variable name.
 */
#ifndef MAX_VARIABLE_NAME_LENGTH
#define MAX_VARIABLE_NAME_LENGTH 9
#endif

/* ************************************************************************ */

/**
 * @brief Maximum length of function name.
 */
#ifndef MAX_FUNCTION_NAME_LENGTH
#define MAX_FUNCTION_NAME_LENGTH 10
#endif

/* ************************************************************************ */

/**
 * @brief Structure for storing variables.
 */
struct Variable
{
    /** Variable name. */
    char name[MAX_VARIABLE_NAME_LENGTH];

    /** Variable value. */
    int value;
};

/* ************************************************************************ */

/**
 * @brief Structure for storing functions by name.
 */
struct Function
{
    /** Function name. */
    char name[MAX_FUNCTION_NAME_LENGTH];

    /** Function pointer. */
    func_t function;
};

/* ************************************************************************ */

/**
 * @brief Current line number.
 */
static unsigned int l_line_no = 0;

/* ************************************************************************ */

/**
 * @brief Array for storage of variables.
 */
static struct Variable *l_variables = NULL;

/* ************************************************************************ */

/**
 * @brief Number of stored variables.
 */
static unsigned int l_variable_count = 0;

/* ************************************************************************ */

/**
 * @brief Array of supported functions.
 */
static const struct Function l_functions[] = {
    {"QUIT", func_quit},
    {"EXIT", func_quit},
    {"SET", func_set},
    {"+", func_add},
    {"-", func_sub},
    {"*", func_mult},
    {"/", func_div},
    {"=", func_eq},
    {"/=", func_neq},
    {"QUOTE", func_quote},
    {"LIST", func_list},
    {"CAR", func_car},
    {"CDR", func_cdr},
    {">", func_gt},
    {">=", func_ge},
    {"<", func_lt},
    {"<=", func_le}
};

/* ************************************************************************ */

/**
 * @brief Number of supported functions.
 */
static const unsigned int l_function_count = sizeof(l_functions) / sizeof(struct Function);

/* ************************************************************************ */

/**
 * @brief Current expression for `clean_up()` when error is occured.
 */
static struct SExpression* l_current_expr = NULL;

/* ************************************************************************ */

/**
 * @brief Find variable object by name.
 *
 * @param name Variable name.
 *
 * @return Found variable object pointer or NULL.
 */
static struct Variable *find_variable(const char *name)
{
    unsigned int i;

    /* No variables */
    if (l_variables == NULL)
        return NULL;

    /* Foreach whole table */
    for (i = 0; i < l_variable_count; ++i)
    {
        /* Check variable name */
        if (!strcmp(l_variables[i].name, name))
            return &l_variables[i];
    }

    /* Not found */
    return NULL;
}

/* ************************************************************************ */

/**
 * @brief Finds empty place for new variable.
 *
 * When a variable is unset, only name is removed and array is not reallocated
 * to save some time. After that when a new variable is set there is no need
 * for array reallocation, we can just use this empty place.
 *
 * @return A pointer to empty variable.
 */
static struct Variable *find_variable_empty()
{
    return find_variable("");
}

/* ************************************************************************ */

/**
 * @brief Finds function object by name.
 *
 * @param name Function name.
 *
 * @return A pointer to function structure or NULL.
 */
static const struct Function* find_function(const char *name)
{
    unsigned int i;

    /* Foreach whole table */
    for (i = 0; i < l_function_count; ++i)
    {
        /* Check function name */
        if (!strcmp(l_functions[i].name, name))
            return &l_functions[i];
    }

    /* Not found */
    return NULL;
}

/* ************************************************************************ */

void syntax_error(const char *err)
{
    fprintf(stderr, "Syntax error: %s\n", err);
    exit(EXIT_FAILURE);
}

/* ************************************************************************ */

void eval_file(FILE *file)
{
    assert(file);

    /* Register clean-up function */
    atexit(&clean_up);

    /* Set source file */
    set_source(file);

    /* Evaluate separate lines */
    while (!eval_line())
        continue;
}

/* ************************************************************************ */

int eval_line(void)
{
    struct SExpression *expr = NULL;
    int quoted = 0;

    if (is_source_stdin())
        printf("[%d]> ", ++l_line_no);

    /* Read symbols */
    while (1)
    {
        /* Get next symbol */
        enum Sym sym = get_sym();

        /* Nothing to read */
        if (sym == SYM_EOF)
            return 1;

        /* Quote */
        if (sym == SYM_QUOTE)
        {
            quoted = 1;
        }
        else if (sym == SYM_NAME)
        {
            expr = alloc_sexpr(TYPE_VALUE);

            /* Variable name */
            if (isalpha(name[0]))
            {
                if (has_variable(name))
                {
                    int value = get_variable(name);
                    sprintf(expr->lvalue, "%d", value);
                }
                else
                {
                    /* No variable */
                    expr->type = TYPE_NIL;
                }
            }
            else
            {
                /* Value */
                strncpy(expr->lvalue, name, sizeof(expr->lvalue));
            }

            break;
        }
        else if (sym == SYM_LPAREN)
        {
            /* Evaluate expression */
            expr = eval_list(quoted ? TYPE_QUOTED : TYPE_NIL);
            break;
        }
    }

    /* Print current command for non-stdin input */
    if (!is_source_stdin())
        printf("[%d]> %s", ++l_line_no, cur_line());

    assert(expr);

    /* Print result and free it */
    print_sexpr(expr);
    free_sexpr(expr);

    printf("\n");

    return 0;
}

/* ************************************************************************ */

struct SExpression *eval_list(enum Type type)
{
    /* Allocate initial expression and store it as current */
    struct SExpression *expr = l_current_expr = alloc_sexpr(type);
    struct SExpression *rexpr = expr;

    /* Must starts as list */
    assert(cur_sym() == SYM_LPAREN);

    /* Read until right paren is found */
    while (get_sym() != SYM_RPAREN)
    {
        if (cur_sym() == SYM_EOF)
        {
            /* Free expression */
            syntax_error("Missing )");
        }

        /* Inner list */
        if (cur_sym() == SYM_LPAREN)
        {
            /* Recursive evaluation */
            rexpr->type = TYPE_SEXPR;
            rexpr = rexpr->right = eval_list(type);
        }
        else if (cur_sym() == SYM_NAME)
        {
            if (rexpr->type == TYPE_NIL)
                rexpr->type = TYPE_SEXPR;

            if (strlen(rexpr->lvalue))
            {
                rexpr = rexpr->right = alloc_sexpr(isdigit(name[0]) ? TYPE_VALUE : type);
            }

            strncpy(rexpr->lvalue, name, sizeof(expr->lvalue));
        }
    }

    /* Evaluate parsed expression */
    expr = eval_sexpr(expr);

    /* After that there is no chance to error */
    l_current_expr = NULL;

    return expr;
}

/* ************************************************************************ */

struct SExpression *eval_sexpr(struct SExpression *expr)
{
    const struct Function *func;

    assert(expr);

    /* Nothing to evaluate */
    if (expr->type == TYPE_NIL || expr->type == TYPE_QUOTED)
        return expr;

    /* Find function */
    func = find_function(expr->lvalue);

    /* Function found */
    if (func)
    {
        /* Call found function */
        return func->function(expr);
    }

    /* Unable to find function with given name */
    {
        char tmp[200];

        /* Construct syntax error */
        strcpy(tmp, "Undefined function: ");
        strcat(tmp, expr->lvalue);

        syntax_error(tmp);
    }

    /* Get rid of no-return warning */
    return NULL;
}

/* ************************************************************************ */

void set_variable(const char *name, int value)
{
    struct Variable *var;

    /* Try to find variable with given name */
    var = find_variable(name);

    /* Not found */
    if (!var)
    {
        /* Try to find empty place */
        var = find_variable_empty();

        if (!var)
        {
            /* Variables needs to be reallocated */
            struct Variable* tmp = realloc(l_variables, (++l_variable_count) *
                sizeof(struct Variable));

            if (tmp == NULL)
            {
                perror("Unable to allocate memory for variables\n");
                exit(EXIT_FAILURE);
            }

            l_variables = tmp;

            /* Pointer to variable structure */
            var = &l_variables[l_variable_count - 1];

            /* Store name */
            strcpy(var->name, name);
        }
    }

    assert(var);

    /* Store variable value */
    var->value = value;
}

/* ************************************************************************ */

int has_variable(const char *name)
{
    /* Try to find variable */
    return (find_variable(name) != NULL);
}

/* ************************************************************************ */

int get_variable(const char *name)
{
    /* Try to find variable */
    struct Variable *var = find_variable(name);

    /* Return variable value */
    if (var)
        return var->value;

    /* Not found */
    return 0;
}

/* ************************************************************************ */

void unset_variable(const char *name)
{
    /* Try to find variable */
    struct Variable *var = find_variable(name);

    /* Remove variable name */
    if (var)
        var->name[0] = '\0';
}

/* ************************************************************************ */

void clean_up(void)
{
    if (l_current_expr)
        free_sexpr(l_current_expr);

    if (l_variables)
        free(l_variables);

    printf("Bye.\n");
}

/* ************************************************************************ */

struct SExpression *func_arithm_base(struct SExpression *expr, arithm_func_t func)
{
    int result;
    int* argv;
    unsigned int argc = 0;
    int* args_ptr;
    struct SExpression *tmp;

    assert(expr);
    assert(func);

    /* NIL */
    if (!expr->right)
        return expr;

    /* Count number of arguments */
    /* The first place is for function name */
    for (tmp = expr->right; tmp != NULL; tmp = tmp->right)
    {
        argc++;
    }

    /* Allocate memory for arguments */
    /* NOTE: C99 - variable-length automatic array */
    argv = calloc(argc, sizeof(int));

    if (argv == NULL)
    {
        perror("Cannot allocate memory for function arguments!\n");
        exit(EXIT_FAILURE);
    }

    /* Store arguments */
    for (tmp = expr->right, args_ptr = argv; tmp != NULL;
        tmp = tmp->right, args_ptr++)
    {
        /* No empty argument */
        assert(strlen(tmp->lvalue) > 0);

        /* Check first character for alpha */
        if (isalpha(tmp->lvalue[0]))
        {
            *args_ptr = get_variable(tmp->lvalue);
        }
        else
        {
            *args_ptr = atoi(tmp->lvalue);
        }
    }

    /* Call function to evaluate */
    result = func(argc, argv);

    /* Free arguments */
    free(argv);

    /* Free argument expressions */
    free_sexpr(expr->right);
    expr->right = NULL;

    /* Store result into expression */
    sprintf(expr->lvalue, "%d", result);

    /* Result is value */
    expr->type = TYPE_VALUE;

    return expr;
}

/* ************************************************************************ */
