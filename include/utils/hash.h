#pragma once

#include <stdint.h>
#include <stddef.h>

/**
 * \defgroup hash Data Hashing Utility
 *
 * Provides utility for fast hashing of data for usage in hash tables.
 *
 * Example usage:
 *
 * \code{.c}
 * uint32_t hashval = murmur_hash((void *)"hello", 5);
 * \endcode
 *
 * @{
 */

/**
 * \brief Get the 32-bit Murmur3 hash from the given data.
 *
 * MurmurHash3 was created by Austin Appleby in 2008. This code has been ported
 * from the C implementation in qLibc library.
 *
 * \param data The data to be hashed
 * \param nbytes How many bytes of the data to hash
 */
uint32_t murmur_hash(const void *data, size_t nbytes);

/** @} */
