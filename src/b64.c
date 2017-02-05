/* This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file in the project root
 * for details.
 */

#include "b64.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                               "abcdefghijklmnopqrstuvwxyz"
                               "0123456789+/=";

char *b64_encode(const char *s, const unsigned long numBytes, unsigned long *encodedNumBytes)
{
    size_t srcLength = numBytes;
    size_t originalLength = srcLength;

    switch(srcLength%3)
    {
    case 1:
        srcLength += 2;
        break;
    case 2:
        srcLength += 1;
        break;
    }

    char *out = malloc((4*srcLength/3+1)*sizeof(char));

    int b;
    size_t i;
    size_t idx = 0;

    for(i=0; i<srcLength; i += 3)
    {
        // Start with first 6 bits from the left.
        // 0xfc = 0b11111100
        // 0xfc >> 2 = 0b00111111
        b = (s[i] & 0xfc) >> 2;
        out[idx++] = alphabet[b];

        // Next group of 6 bits starts with the last 2 of the first 8 bits.
        // 0x03 = 0b00000011
        // 0x03 << 4 = 0b00110000
        b = (s[i] & 0x03) << 4;

        if(i+1<originalLength)
        {
            // 2nd part of second 6 bit block -> first 4 bits of second 8 bit source
            // 0xf0 = 0b11110000
            // 0xf0 >> 4 = 0b00001111
            // b |= --> 0b00111111
            b |= (s[i+1] & 0xf0) >> 4;
            out[idx++] = alphabet[b];

            // 3rd 6 bit block: last 4 bits of second 8 bit source
            // 0x0f = 0b00001111
            // 0x0f << 2 = 0b00111100
            b = (s[i+1] & 0x0f) << 2;

            if(i+2<originalLength)
            {
                // last 2 bits of 3rd 6 bit block: first 2 bits of 3rd 8 bit source
                // 0xc0 = 0b11000000
                // 0xc0 >> 6 = 0b00000011
                // b |= --> 0b00111111
                b |= (s[i+2] & 0xc0) >> 6;
                out[idx++] = alphabet[b];

                // last 6 bits: last 6 bits of 3rd 8 bit source
                // 0x3f = 0b00111111
                b = s[i+2] & 0x3f;
                out[idx++] = alphabet[b];
            }
            else
            {
                out[idx++] = alphabet[b];
                out[idx++] = '=';
            }
        }
        else
        {
            out[idx++] = alphabet[b];
            out[idx++] = '=';
            out[idx++] = '=';
        }
    }

    out[4*srcLength/3] = '\0';

    if(encodedNumBytes)
    {
        *encodedNumBytes = 4*srcLength/3;
    }

    return out;
}

long indexIn(const char *a, char c)
{
    const char *p = strchr(a, c);

    if(p)
    {
        return p - a;
    }

    return 0;
}

char *b64_decode(const char *s, const unsigned long numBytes, unsigned long *decodedNumBytes)
{
    if(!numBytes%4) return 0;

    // This can only be 0, 1, or 2. Otherwise, something is wrong ...
    size_t numPaddingChars = 0;

    if(numBytes>=2)
    {
        if(s[numBytes-1]=='=') ++numPaddingChars;
        if(s[numBytes-2]=='=') ++numPaddingChars;
    }

    const size_t out_len = 3*numBytes/4-numPaddingChars;

    char *out = malloc(out_len+1);
    out[out_len] = '\0';

    int j = 0;
    long b[4];
    size_t i;

    for(i=0; i<numBytes; i+=4)
    {
        b[0] = indexIn(alphabet, s[i]);
        b[1] = indexIn(alphabet, s[i+1]);
        b[2] = indexIn(alphabet, s[i+2]);
        b[3] = indexIn(alphabet, s[i+3]);

        out[j++] = (char) ((b[0] << 2) | (b[1] >> 4));

        if(b[2] < 64)
        {
            out[j++] = (char) ((b[1] << 4) | (b[2] >> 2));

            if(b[3] < 64)
            {
                out[j++] = (char) ((b[2] << 6) | b[3]);
            }
        }
    }

    if(decodedNumBytes)
    {
        *decodedNumBytes = out_len;
    }

    return out;
}

char *b64_encodeFile(const char *fileName, unsigned long *numBytes)
{
    FILE *file;
    long fileSize;
    char *contents;

    file = fopen(fileName, "rb");

    if(file==NULL) return NULL;

    // get file size:
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    rewind(file);

    // allocate space for content and read the file
    contents = malloc((size_t)(fileSize+1)*sizeof(char));
    contents[fileSize] = '\0';

    if(contents==NULL) return NULL;

    size_t bytesRead = fread(contents, 1, (size_t)fileSize, file);

    if(bytesRead!=(size_t)fileSize)
    {
        free(contents);
        return NULL;
    }

    fclose(file);

    // encode to Base64
    char *encodedContents = b64_encode(contents, bytesRead, numBytes);

    free(contents);

    return encodedContents;
}

char *b64_decodeFile(const char *fileName, unsigned long *numBytes)
{
    FILE *file;
    long fileSize;
    char *contents;

    file = fopen(fileName, "rb");

    if(file==NULL) return NULL;

    // get file size:
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    rewind(file);

    // allocate space for content and read the file
    contents = malloc((size_t)(fileSize+1)*sizeof(char));
    contents[fileSize] = '\0';

    if(contents==NULL) return NULL;

    size_t bytesRead = fread(contents, 1, (size_t)fileSize, file);

    if(bytesRead!=(size_t)fileSize)
    {
        free(contents);
        return NULL;
    }

    fclose(file);

    // encode to Base64
    char *decodedContents = b64_decode(contents, bytesRead, numBytes);

    free(contents);

    return decodedContents;
}
