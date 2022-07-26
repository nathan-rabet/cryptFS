#ifndef ERRORS_H
#define ERRORS_H

#define STATIC_ASSERT(X)                                                       \
    ({                                                                         \
        extern int __attribute__(                                              \
            (error("assertion failure at compilation: '" #X "' not true")))    \
            compile_time_check();                                              \
        ((X) ? 0 : compile_time_check()), 0;                                   \
    })

/**
 * @brief Prints an error message on stderr and exits the program.
 *
 * @param msg The error message to print.
 * @param error_code The error code to exit with.
 */
void error_exit(const char *msg, int error_code);

#endif /* ERRORS_H */
