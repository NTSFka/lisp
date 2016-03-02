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

/* C library */
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

/* LISP */
#include "tokenizer.h"
#include "interpret.h"

/* ************************************************************************ */

#ifndef NDEBUG

/**
 * @brief S-expression statistics handler.
 */
static void exit_handler(void)
{
    printf("Count: %d\n", sexpr_count);
}
#endif

/* ************************************************************************ */

/**
 * @brief Main function.
 *
 * @param argc Argument count.
 * @param argv Argument values.
 */
int main(int argc, char **argv)
{
#ifndef NDEBUG
    atexit(exit_handler);
#endif

    /* Source file as argument */
    if (argc > 1)
    {
        /* Open source file */
        FILE *f = fopen(argv[1], "r");

        if (f == NULL)
        {
            perror(argv[1]);
            return EXIT_FAILURE;
        }

        /* Evaluate file */
        eval_file(f);

        /* Close file */
        fclose(f);
    }
    else
    {
        /* Input from standard input */
        eval_file(stdin);
    }

    return EXIT_SUCCESS;
}

/* ************************************************************************ */
