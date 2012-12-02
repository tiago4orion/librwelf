/**
 * rwelf
 * Copyright (c) 2012-2013 Felipe Pena <felipensp(at)gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef RWELF_H
#define RWELF_H

#include <assert.h>
#include <elf.h>
#include <link.h>

#ifndef NULL
#define NULL ((void*)0)
#endif

struct rwelf;

typedef struct _rwelf {
	int fd;
	unsigned char *file;
	size_t size;	
	ElfW(Ehdr) *header;
} rwelf;


extern rwelf *rwelf_open(const char *);
extern const char *rwelf_class(const rwelf *);
extern const char *rwelf_data(const rwelf *);
extern int rwelf_version(const rwelf *);
extern const char *rwelf_type(const rwelf *);
extern void rwelf_close(rwelf *);


#endif /* RWELF_H */