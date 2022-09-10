#ifndef MOUNT_H
#define MOUNT_H

/**
 * @brief Check if the given mount point exists and if the given file is a cryptfs file system.
 *
 * @param mount_point The mount point to check.
 * @param file_path Path to the file to check.
 * @return 0 if the mount point exists and the file is a cryptfs file system
 * @return 1 if the file is not a cryptfs file system and/or the mount point does not exist
 * @return 1 if the file is a cryptfs and the mount point exist
 * @return -1 otherwise like if any error occured
 */
int mount_exists(const char *mount_point, const char * file_path);

#endif // MOUNT_H
