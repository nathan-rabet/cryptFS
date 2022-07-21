#ifndef CRYPT_FS_H
#define CRYPT_FS_H

#include <openssl/sha.h>
#include <stdint.h>

#define CRYPTFS_MAGIC 0x63727970746673
#define CRYPTFS_VERSION 1
#define CRYPTFS_BLOCK_SIZE 2048

#define NB_ENTRY_PER_FAT 32

#define NB_ENCRYPTION_KEYS 64
#define RSA_KEY_SIZE_BITS 2048
#define RSA_KEY_SIZE_BYTES (RSA_KEY_SIZE_BITS / 8)

#define ENTRY_NAME_LEN 128

enum ENTRY_TYPE {
	ENTRY_TYPE_FILE = 0,
	ENTRY_TYPE_DIRECTORY = 1,
	ENTRY_TYPE_HARDLINK = 2,
	ENTRY_TYPE_SYMLINK = 3,
	ENTRY_TYPE_UNKNOWN = 4
};

enum FAT_BLOCK_TYPE {
	FAT_BLOCK_INVALID = -2,
	FAT_BLOCK_END = -1,
	FAT_BLOCK_FREE = 0,
};

struct Crypt_FS_Entry {
	uint8_t type; // ENTRY_TYPE
	uint64_t start_block; // First block of the entry
	uint64_t size; // in bytes
	uint32_t uid; // User ID
	uint32_t gid; // Group ID
	uint32_t mode; // Permissions
	uint32_t atime; // Access time
	uint32_t mtime; // Modification time
	uint32_t ctime; // Creation time
	char name[ENTRY_NAME_LEN]; // Name of the entry
	char free[]; // Reserved for future use
} __attribute__((packed));

struct CryptFS_Directory {
	uint32_t num_entries;
	struct Crypt_FS_Entry entries[];
} __attribute__((packed));

struct Crypt_FS_Key {
	uint64_t rsa_e; // Public exponent of the RSA keypair
	uint8_t rsa_n[RSA_KEY_SIZE_BYTES]; // RSA public number 'n' (the modulus)
    uint8_t private_key_hash[SHA384_DIGEST_LENGTH]; // Hash of the private key
                                                    // (SHA384)
    uint8_t aes_key_ciphered[RSA_KEY_SIZE_BYTES]; // AES key ciphered with RSA
                                                  // public key
} __attribute__((packed));

struct CryptFS_FAT {
	uint64_t next_fat_block; // Next FAT block
	uint64_t entries[NB_ENTRY_PER_FAT];
} __attribute__((packed));

#define CRYPT_FS_ROOT_DIR_BLOCK(FAT)                                           \
	(sizeof(uint64_t) + 2 * sizeof(uint8_t) + sizeof(uint32_t) +           \
	 64 * sizeof(struct Crypt_FS_Key) + sizeof(struct CryptFS_FAT))

struct CryptFS_Header {
	uint8_t boot[5]; // Reserved for future use
	uint64_t magic; // CRYPTFS_MAGIC
	uint8_t version; // CRYPTFS_VERSION
	uint32_t blocksize; // in bytes
	struct Crypt_FS_Key keys
		[NB_ENCRYPTION_KEYS]; // Encrypted keys of users (max NB_ENCRYPTION_KEYS users)
	struct CryptFS_FAT fat; // File Allocation Table
	struct CryptFS_Directory root_directory; // Root directory
} __attribute__((packed));

#endif /* CRYPT_FS_H */
