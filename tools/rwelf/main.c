#include <rwelf.h>
#include <stdio.h>

int main(int argc, char **argv) {
	rwelf *elf = rwelf_open("./librwelf.so");
	
	if (elf) {
		printf("Class:   %s\n", rwelf_class(elf));
		printf("Data:    %s\n", rwelf_data(elf));
		printf("Version: %d\n", rwelf_version(elf));
		printf("Type:    %s\n", rwelf_type(elf));
		
		rwelf_close(elf);
	}

	return 0;
}
