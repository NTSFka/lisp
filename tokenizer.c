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
#include "tokenizer.h"

/* C library */
#include <assert.h>
#include <ctype.h>
#include <string.h>

/* ************************************************************************ */

/** Current file. */
static FILE* l_file = NULL;

/* ************************************************************************ */

/** Current symbol. */
static enum Sym l_symbol = SYM_INV;

/* ************************************************************************ */

/** Line buffer. */
static char l_line[MAX_LINE_LENGTH] = {'\0'};

/* ************************************************************************ */

/** Current character */
static const char* l_current = l_line;

/* ************************************************************************ */

char name[MAX_NAME_LENGTH];

/* ************************************************************************ */

static int is_symbol_name(int c)
{
    return !isspace(c) && c != ')' && c != '(';
}

/* ************************************************************************ */

void set_source(FILE* file)
{
    l_file = file;
}

/* ************************************************************************ */

int is_source_stdin(void)
{
    return l_file == stdin;
}

/* ************************************************************************ */

int load_line(void)
{
    assert(l_file);

    /* Reads line into buffer */
    if (!fgets(l_line, MAX_LINE_LENGTH, l_file))
    {
        return EOF;
    }

    /* Set current character pointer */
    l_current = l_line;

    return 0;
}

/* ************************************************************************ */

const char* cur_line(void)
{
    return l_line;
}

/* ************************************************************************ */

int get_char(void)
{
    assert(l_file);
    assert(l_current);

    /* Next character */
    if (*l_current != '\0')
        ++l_current;

    /* All characters are read */
    while (*l_current == '\0')
    {
        /* Load next line */
        if (load_line() == EOF)
            return EOF;
    }

    /* Return current character */
    return cur_char();
}

/* ************************************************************************ */

int cur_char(void)
{
    assert(l_file);

    if (feof(l_file))
        return EOF;

    assert(l_current);
    return *l_current;
}

/* ************************************************************************ */

enum Sym get_sym(void)
{
    /* Read the next character */
    int c = get_char();

    /* Nothing more */
    if (c == EOF)
        return SYM_EOF;

    switch (c)
    {
    default:
        /* Control characters are not supported */
        if (iscntrl(c))
        {
            l_symbol = SYM_INV;
        }
        else
        {
            const char *ptmp = l_current;
            char *pname = name;

            /* Read all characters that match name */
            while (is_symbol_name(get_char()))
                continue;

            /* Store name as upper case */
            /*strncpy(name, start, current - start);*/
            for (; ptmp < l_current; ptmp++, pname++)
            {
                /* TODO: Longer names than buffer? */
                *pname = toupper(*ptmp);
            }

            /* Terminate character */
            *pname = '\0';

            --l_current;

            /* Name symbol */
            l_symbol = SYM_NAME;
        }
        break;

    case '\n':
    case '\r':
        /* New line symbol */
        l_symbol = SYM_EOL;
        name[0] = '\0';
        break;

    case ' ':
    case '\t':
        /* Space symbol */
        l_symbol = SYM_SPACE;
        name[0] = '\0';
        break;

    case '(':
        /* Left parenthesis symbol */
        l_symbol = SYM_LPAREN;
        name[0] = '\0';
        break;

    case ')':
        /* Right parenthesis symbol */
        l_symbol = SYM_RPAREN;
        name[0] = '\0';
        break;

    case '\'':
        /* Quote symbol */
        l_symbol = SYM_QUOTE;
        name[0] = '\0';
        break;
    }

    return cur_sym();
}

/* ************************************************************************ */

enum Sym cur_sym(void)
{
    return l_symbol;
}

/* ************************************************************************ */
