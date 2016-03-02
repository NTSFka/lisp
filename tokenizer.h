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

#ifndef TOKENIZER_H_
#define TOKENIZER_H_

/* ************************************************************************ */

/* C library */
#include <stdio.h>

/* ************************************************************************ */

/**
 * @brief Maximum symbol name length
 */
#ifndef MAX_NAME_LENGTH
#define MAX_NAME_LENGTH 30
#endif

/* ************************************************************************ */

/**
 * @brief Maximum line length.
 */
#ifndef MAX_LINE_LENGTH
#define MAX_LINE_LENGTH 1024
#endif

/* ************************************************************************ */

/**
 * @brief List of tokenizer symbols
 */
enum Sym
{
    /** Invalid symbol */
    SYM_INV,
    /** Left parentesis */
    SYM_LPAREN,
    /** Right parenthesis */
    SYM_RPAREN,
    /** Quote symbol */
    SYM_QUOTE,
    /** Name symbol */
    SYM_NAME,
    /** Space symbol */
    SYM_SPACE,
    /** New line symbol */
    SYM_EOL,
    /** End of file symbol */
    SYM_EOF
};

/* ************************************************************************ */

/**
 * @brief Variable for storing current symbol name.
 */
extern char name[MAX_NAME_LENGTH];

/* ************************************************************************ */

/**
 * @brief Set source file.
 *
 * @param file A pointer to source file.
 */
void set_source(FILE* file);

/* ************************************************************************ */

/**
 * @brief Returns if source is standard input.
 *
 * @return Is source standard input?
 */
int is_source_stdin(void);

/* ************************************************************************ */

/**
 * @brief Reads the next line into current buffer.
 *
 * @return The obtained character on success or EOF on failure.
 */
int load_line(void);

/* ************************************************************************ */

/**
 * @brief Returns a pointer to current loaded line.
 *
 * Maximum loaded characters are defined by `MAX_LINE_LENGTH` macro.
 *
 * @return A pointer to current line.
 */
const char* cur_line(void);

/* ************************************************************************ */

/**
 * @brief Reads the next character from input.
 *
 * @return The obtained character on success or EOF on failure.
 */
int get_char(void);

/* ************************************************************************ */

/**
 * @brief Returns current character. It does nothing with input.
 *
 * @return Current character.
 */
int cur_char(void);

/* ************************************************************************ */

/**
 * @brief Reads the next symbol from input.
 *
 * @return The obtained symbol on success or SYM_EOF on failure.
 */
enum Sym get_sym(void);

/* ************************************************************************ */

/**
 * @brief Returns current token symbol. It does nothing with input.
 *
 * @return Current token symbol.
 */
enum Sym cur_sym(void);

/* ************************************************************************ */

#endif /* TOKENIZER_H_ */

/* ************************************************************************ */
