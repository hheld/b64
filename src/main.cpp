/* This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file in the project root
 * for details.
 */

#include <iostream>
#include <fstream>
#include <string>

extern "C"
{
#include "b64.h"
}

int main(int argc, char *argv[])
{
    if(argc!=4)
    {
        std::cerr << "Usage:\n" << argv[0] << "<enc|dec> <input file> <output file>\n";
        return 1;
    }

    const std::string mode = argv[1];
    const char *in = argv[2];
    const char *out = argv[3];

    unsigned long numBytes = 0;

    char *contents = nullptr;

    if(mode=="enc")
    {
        contents = b64_encodeFile(in, &numBytes);
    }
    else if(mode=="dec")
    {
        contents = b64_decodeFile(in, &numBytes);
    }
    else
    {
        return 1;
    }

    if(!contents) return 1;

    std::ofstream outFile(out, std::ios::binary);

    if(!outFile.good())
    {
        return 1;
    }

    outFile.write(contents, numBytes);

    free(contents);

    return 0;
}
