#ifndef FORMAT_H
#define FORMAT_H

#include <stdbool.h>

/**
 * @brief Check if the given file is a cryptfs file system.
 *
 * @param file_path Path to the file to check.
 * @return true if the file is a cryptfs file system.
 * @return false if the file is not a cryptfs file system.
 */
bool is_already_formatted(const char *file_path);

/**
 * @brief Format the given file to a cryptfs file system.
 * 
 * @param path The path to the file to format.
 */
void format_fs(const char *path);

#endif /* FORMAT_H */
