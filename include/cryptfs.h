#ifndef CRYPT_FS_H
#define CRYPT_FS_H

#include <linux/types.h>

#define CRYPTFS_MAGIC 0x63727970746673
#define CRYPTFS_VERSION 1
#define ENTRY_NAME_LEN 128

enum ENTRY_TYPE {
	ENTRY_TYPE_FILE = 0,
	ENTRY_TYPE_DIRECTORY = 1,
	ENTRY_TYPE_HARDLINK = 2,
	ENTRY_TYPE_SYMLINK = 3,
	ENTRY_TYPE_UNKNOWN = 4
};

struct Crypt_FS_Entry {
	uint8_t entry_type;
	uint32_t start_block;
	uint32_t size; // in bytes
	char name[ENTRY_NAME_LEN];
	uint32_t uid;
	uint32_t gid;
	uint32_t mode;
	uint32_t atime;
	uint32_t mtime;
	uint32_t ctime;
	uint32_t nb_links;
};

struct CryptFS_File {
	uint32_t next_block;
	uint32_t size; // in bytes, must NOT be greater than the size of a block
};

struct CryptFS_Directory {
	uint32_t magic;
	uint32_t num_entries;
	Crypt_FS_Entry entries[];
};

struct Crypt_FS_HashedKey {
	uint8_t key[8]; // SHA256 hash
};

struct CryptFS_FAT {
	// TODO
}

struct CryptFS_Header {
	uint64_t magic;
	uint32_t version;
	uint32_t blocksize;
	CryptFS_FAT fat;
	Crypt_FS_HashedKey hashed_keys[64];
	CryptFS_Directory root_directory;
}

#endif /* CRYPT_FS_H */
