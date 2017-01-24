#include "b64.h"

#include <stdlib.h>
#include <string.h>

static const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                               "abcdefghijklmnopqrstuvwxyz"
                               "0123456789+/=";

char *b64_encode(const char *s)
{
    size_t srcLength = strlen(s);
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

char *b64_decode(const char *s)
{
    const size_t s_len = strlen(s);

    if(!s_len%4) return 0;

    // This can only be 0, 1, or 2. Otherwise, something is wrong ...
    size_t numPaddingChars = 0;

    if(s_len>=2)
    {
        if(s[s_len-1]=='=') ++numPaddingChars;
        if(s[s_len-2]=='=') ++numPaddingChars;
    }

    const size_t out_len = 3*s_len/4-numPaddingChars;

    char *out = malloc(out_len+1);
    out[out_len] = '\0';

    int j = 0;
    long b[4];
    size_t i;

    for(i=0; i<s_len; i+=4)
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

    return out;
}
