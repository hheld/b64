#ifndef B64_H
#define B64_H

/**
 * Encode string \a s to base64.
 *
 * @param s string to be converted, must be 0 terminated
 * @return New string containing 0-terminated base64 encoded version of \a s. The caller is responsible for cleaning up.
 */
char *b64_encode(const char *s);

/**
 * Decode string \a s from base64.
 *
 * @param s string to be converted, must be 0 terminated
 * @return New string containing 0-terminated decoded version of \a s. The caller is responsible for cleaning up.
 */
char* b64_decode(const char *s);


#endif // B64_H
