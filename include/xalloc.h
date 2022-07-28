#ifndef XMALLOC_H
#define XMALLOC_H

#include <stddef.h>

/**
 * @brief Malloc wrapper that exits the program if the allocation fails
 * or if nmemb * size overflows
 *
 * @param nmemb Number of elements to allocate
 * @param size Size of each element
 * @return void*
 */
void *xmalloc(size_t nmemb, size_t size);

/**
 * @brief Aligned malloc wrapper that exits the program if the allocation fails
 * or if nmemb * size overflows
 *
 * @param alignment Alignment of the allocation
 * @param nmemb Number of elements to allocate
 * @param size Size of each element
 * @return void*
 */
void *xaligned_alloc(size_t alignment, size_t nmemb, size_t size);

/**
 * @brief Calloc wrapper that exits the program if the allocation fails
 * or if nmemb * size overflows
 *
 * @param nmemb Number of elements to allocate
 * @param size Size of each element
 * @return void*
 */
void *xcalloc(size_t nmemb, size_t size);

/**
 * @brief Realloc wrapper that exits the program if the allocation fails
 * or if nmemb * size overflows
 *
 * @param ptr Pointer to the memory to reallocate
 * @param nmemb Number of elements to allocate
 * @param size Size of each element
 * @return void*
 */
void *xrealloc(void *ptr, size_t nmemb, size_t size);

#endif /* XALLOC_H */
