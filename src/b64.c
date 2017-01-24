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

char *b64_decode(const char *s)
{
    (void) s;
    return 0;
}
