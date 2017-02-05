/* This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file in the project root
 * for details.
 */

#ifndef B64_H
#define B64_H

/**
 * Encode string \a s to base64.
 *
 * @param s string to be converted, must be 0 terminated
 * @param numBytes number of bytes in \a s
 * @param encodedNumBytes Will contain the length of the returned encoded contents, if the pointer is not NULL.
 *                        If it is NULL, it is ignored.
 * @return New string containing 0-terminated base64 encoded version of \a s. The caller is responsible for cleaning up.
 */
char *b64_encode(const char *s, const unsigned long numBytes, unsigned long *encodedNumBytes);

/**
 * Decode string \a s from base64.
 *
 * @param s string to be converted, must be 0 terminated
 * @param numBytes number of bytes in \a s
 * @param decodedNumBytes Will contain the length of the returned decoded contents, if the pointer is not NULL.
 *                        If it is NULL, it is ignored.
 * @return New string containing 0-terminated decoded version of \a s. The caller is responsible for cleaning up.
 */
char* b64_decode(const char *s, const unsigned long numBytes, unsigned long *decodedNumBytes);

/**
 * Encodes the contents of the given file \a fileName in Base64.
 *
 * @param fileName Name, i.e. path, of the file whose content is to be encoded
 * @param numBytes Will contain the length of the returned encoded contents
 * @return File content encoded in Base64
 */
char *b64_encodeFile(const char *fileName, unsigned long *numBytes);

/**
 * Decodes the contents of the given file \a fileName from Base64.
 *
 * @param fileName Name, i.e. path, of the file whose content is to be decoded
 * @param numBytes Will contain the length of the returned decoded contents
 * @return File content decoded from Base64
 */
char *b64_decodeFile(const char *fileName, unsigned long *numBytes);

#endif // B64_H
