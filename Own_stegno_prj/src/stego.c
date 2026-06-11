#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage:\n");
        printf("./stego -e input.bmp secret.txt output.bmp\n");
        printf("./stego -d output.bmp\n");
        return 1;
    }

    if (strcmp(argv[1], "-e") == 0)
    {
        do_encoding(argv[2], argv[3], argv[4]);
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        do_decoding(argv[2]);
    }

    return 0;
}
