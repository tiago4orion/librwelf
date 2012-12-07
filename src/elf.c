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

#include "rwelf.h"
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

/**
 * Finds the string table used for ElfN_Sym and ElfN_Shdr
 */
static void inline _find_str_tables(rwelf *elf)
{
	int i;
	
	/* String table for section names */
	elf->shstrtab = elf->file +
		ELF_SHDR(elf, sh_offset, ELF_EHDR(elf, e_shstrndx));
	
	/* Find the symbol string table and symtab */
	for (i = 0; i < ELF_EHDR(elf, e_shnum); ++i) {
		switch (ELF_SHDR(elf, sh_type, i)) {
			case SHT_STRTAB:
				if (ELF_SHDR(elf, sh_flags, i) == 0
					&& ELF_EHDR(elf, e_shstrndx) != i) {
					/* Symbol name string table */
					elf->symstrtab = elf->file + 
						ELF_SHDR(elf, sh_offset, i);
				}
				break;
			case SHT_SYMTAB:
				/* symtab section */
				if (RWELF_IS_32(elf)) {
					SYM32(elf) = (Elf32_Sym*)(elf->file +
						ELF_SHDR(elf, sh_offset, i));
				} else {
					SYM64(elf) = (Elf64_Sym*)(elf->file +
						ELF_SHDR(elf, sh_offset, i));
				}
				
				elf->nsyms = ELF_SHDR(elf, sh_size, i) / 
					ELF_SHDR(elf, sh_entsize, i);
				break;
		}
	}
}

/**
 * Prepares internal data according to ELF's class
 */
static int _prepare_internal_data(rwelf *elf)
{
	switch (elf->file[EI_CLASS]) {
		case ELFCLASS32:
			EHDR32(elf) = (Elf32_Ehdr*) elf->file;
			PHDR32(elf) = (Elf32_Phdr*) (elf->file + EHDR32(elf)->e_phoff);
			SHDR32(elf) = (Elf32_Shdr*) (elf->file + EHDR32(elf)->e_shoff);
			break;
		case ELFCLASS64:
			EHDR64(elf) = (Elf64_Ehdr*) elf->file;
			PHDR64(elf) = (Elf64_Phdr*) (elf->file + EHDR64(elf)->e_phoff);
			SHDR64(elf) = (Elf64_Shdr*) (elf->file + EHDR64(elf)->e_shoff);
			break;
		case ELFCLASSNONE:
		default:
			return 0;
	}

	_find_str_tables(elf);
	
	return 1;
}

/**
 * rwelf_open(const char*)
 * Opens a ELF file for reading/writing
 */
rwelf *rwelf_open(const char *fname)
{
	unsigned char *mem;
	struct stat st;
	int fd;
	rwelf *elf;

	if ((fd = open(fname, O_RDONLY)) == -1) {
		return NULL;
	}
	
	fstat(fd, &st);

	mem = mmap(0, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
	
	if (mem == MAP_FAILED || memcmp(mem, ELFMAG, SELFMAG) != 0) {
		close(fd);
		return NULL;
	}
	
	elf = calloc(1, sizeof(rwelf));
	
	assert(elf != NULL);
	
	elf->file = mem;
	elf->fd   = fd;	
	elf->size = st.st_size;

	if (_prepare_internal_data(elf)) {
		return elf;		
	}
	
	rwelf_close(elf);
	
	return NULL;
}

/**
 * rwelf_close(rwelf *elf)
 * Closes fd and unmap memory related to internal rwelf data
 */
void rwelf_close(rwelf *elf)
{
	assert(elf != NULL);

	if (!elf->file) {
		return;
	}

	munmap(elf->file, elf->size);
	close(elf->fd);	
	free(elf);
}
