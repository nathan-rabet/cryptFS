#include <stdio.h>
#include <stdlib.h>

#include "format.h"
#include "cryptfs.h"

bool is_already_formatted(const char *path)
{
	FILE *file = fopen(path, "r");
	if (file == NULL)
		return false;

	struct CryptFS_Header header = { 0 };

	fread(&header.magic, sizeof(uint64_t), 1, file);
	fread(&header.version, sizeof(uint32_t), 1, file);
	fread(&header.blocksize, sizeof(uint32_t), 1, file);
	fread(&header.num_keys, sizeof(uint8_t), 1, file);
	fread(&header.fat, sizeof(struct CryptFS_FAT), 1, file);
	fread(&header.root_directory, sizeof(struct CryptFS_Directory), 1,
	      file);

	// Check if the magic number is correct
	if (header.magic != CRYPTFS_MAGIC)
		return false;
	// Check if the version is correct
	else if (header.version != CRYPTFS_VERSION)
		return false;
	// Check if the blocksize is correct (must be a multiple of 2)
	else if (header.blocksize == 0 || header.blocksize % 2 != 0)
		return false;
	// Check if the number of keys is non-zero
	else if (header.num_keys == 0)
		return false;

	return true;
}

void format_fs(const char *path)
{
	// Check if the file is already formatted
	if (is_already_formatted(path)) {
		// Ask the user if he wants to overwrite the file
		printf("The file is already formatted. Do you want to overwrite it? (y/n) ");
		char answer[2];
		scanf("%s", answer);
		if (answer[0] != 'y' && answer[0] != 'Y')
			return;
	}

	FILE *file = fopen(path, "w+");
	if (file == NULL) {
		perror("Impossible to open the file for formatting\n");
		exit(EXIT_FAILURE);
	}

	// Get the size of the file
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	fseek(file, 0, SEEK_SET);

	// Write the header
	struct CryptFS_Header header = {
		.boot = { 0x90, 0x90, 0x90, 0x90, 0x90 }, // Nop operation
		.magic = CRYPTFS_MAGIC,
		.version = CRYPTFS_VERSION,
		.blocksize = CRYPTFS_BLOCK_SIZE,
		.num_keys = 0,
		.fat = { .next_fat_block = FAT_BLOCK_END, .entries = { 0 } },
	};

	// Write in a loop
	while (ftell(file) < size)
		fwrite(&header, sizeof(struct CryptFS_Header), 1, file);
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Usage: %s <file>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	format_fs(argv[1]);
	return 0;
}
