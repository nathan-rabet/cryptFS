#ifndef ERRORS_H
#define ERRORS_H

/**
 * @brief Prints an error message on stderr and exits the program.
 *
 * @param msg The error message to print.
 * @param error_code The error code to exit with.
 */
void error_exit(const char *msg, int error_code);

#endif /* ERRORS_H */
