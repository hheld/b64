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

/**
 * Encodes the contents of the given file \a fileName in Base64.
 *
 * @param fileName Name, i.e. path, of the file whose content is to be encoded
 * @return File content encoded in Base64
 */
char *b64_encodeFile(const char *fileName);

#endif // B64_H
