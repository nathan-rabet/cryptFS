#ifndef CRYPT_FS_H
#    define CRYPT_FS_H

#    include <openssl/sha.h>
#    include <stdint.h>

#    include "error.h"

#    define CRYPTFS_MAGIC 0x63727970746673
#    define CRYPTFS_VERSION 1
#    define CRYPTFS_BLOCK_SIZE_BYTES 512
#    define CRYPTFS_BLOCK_SIZE_BITS (CRYPTFS_BLOCK_SIZE_BYTES / 8)

#    define RSA_KEY_SIZE_BITS 2048
#    define RSA_KEY_SIZE_BYTES (RSA_KEY_SIZE_BITS / 8)
#    define NB_ENCRYPTION_KEYS 64

#    define AES_KEY_SIZE_BITS 256
#    define AES_KEY_SIZE_BYTES (AES_KEY_SIZE_BITS / 8)

#    define ENTRY_NAME_LEN 128

enum ENTRY_TYPE
{
    ENTRY_TYPE_FILE = 0,
    ENTRY_TYPE_DIRECTORY = 1,
    ENTRY_TYPE_HARDLINK = 2,
    ENTRY_TYPE_SYMLINK = 3,
    ENTRY_TYPE_UNKNOWN = 4
};

enum FAT_BLOCK_TYPE
{
    FAT_BLOCK_INVALID = -3,
    FAT_BLOCK_RESERVED = -2,
    FAT_BLOCK_END = -1,
    FAT_BLOCK_FREE = 0,
};

struct CryptFS_Entry
{
    uint8_t type; // ENTRY_TYPE
    uint64_t
        start_block; // First block of the entry, FAT_BLOCK_END for directory
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

#    define CRYPTFS_MAX_ENTRIES_PER_DIR (CRYPTFS_BLOCK_SIZE_BYTES - sizeof(uint64_t) - sizeof(uint32_t)) / sizeof(struct CryptFS_Entry))

struct CryptFS_Directory
{
    uint64_t used; // 1 if the directory is used, 0 if free
    uint32_t num_entries;
    struct CryptFS_Entry entries[];
} __attribute__((packed, aligned(CRYPTFS_BLOCK_SIZE_BYTES)));

struct CryptFS_Key
{
    uint8_t rsa_n[RSA_KEY_SIZE_BYTES]; // RSA public number 'n' (the modulus)
    uint8_t aes_key_ciphered[RSA_KEY_SIZE_BYTES]; // AES key ciphered with RSA
                                                  // public key
} __attribute__((packed));

#    define EEEE sizeof(struct Crypt_FS_Key)

struct CryptFS_FAT_Entry
{
    uint32_t fat_block; // Block of the entry
    uint32_t entry_offset; // Offset of the entry in the block
} __attribute__((packed));

struct CryptFS_FAT
{
    uint64_t next_fat_block; // Next FAT block
    struct CryptFS_FAT_Entry entries[];
} __attribute__((packed, aligned(CRYPTFS_BLOCK_SIZE_BYTES)));

struct CryptFS_Header
{
    uint8_t boot[5]; // Reserved for future use
    uint64_t magic; // CRYPTFS_MAGIC
    uint8_t version; // CRYPTFS_VERSION
    uint32_t blocksize; // in bytes
} __attribute__((packed, aligned(CRYPTFS_BLOCK_SIZE_BYTES)));

struct CryptFS
{
    struct CryptFS_Header header;
    struct CryptFS_Key keys_storage[NB_ENCRYPTION_KEYS];
    struct CryptFS_FAT first_fat;
    struct CryptFS_Directory root_directory;
};

#    define SSS sizeof(struct CryptFS)

#endif /* CRYPT_FS_H */

//
// BLOCK 1: Header
// BLOCK 2-65: Keys
// BLOCK 66: FAT
// BLOCK 67: Root directory
