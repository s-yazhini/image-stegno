
#include <stdio.h>
#include "decode.h"

int do_decoding(char *stego_img)
{
    FILE* f_out = fopen(stego_img, "rb");
    if(!f_out)
    {
        printf("File open failure");
        return 1;
    }

    //STEP 1: skip header
    fseek(f_out, 54, SEEK_SET);
    
    //STEP 2.1: check for magic string
    if(decode_magic_string("#*", f_out) != 0)
    {
        printf("No secret data inside given image.");
        return 1;
    }

    //STEP 2.2: decode secret file size
    int secret_size = decode_secret_file_size(f_out);
    printf("Secret size = %d bytes\n", secret_size);

    //STEP 2.3: decode file extension length
    int ext_len = decode_secret_file_size(f_out);
    printf("Extension length = %d\n", ext_len);

    //STEP 2.4: decode file extension
    char ext[10];
    decode_extension(ext, ext_len, f_out);
    printf("Extension = %s\n", ext);

    // STEP 2.5: decode file data
    char secret_fname[50];
    sprintf(secret_fname, "decoded.%s", ext);

    FILE *f_secret = fopen(secret_fname, "wb");
    decode_secret_data(f_out, f_secret, secret_size);
    fclose(f_secret);

    fclose(f_out);
    printf("Decode successful, check decoded.txt for decoded secret data.\n");
    return 0;
}

void decode_byte_from_lsb(char *data, FILE *out)
{
    char img_byte;
    *data = 0;

    for (int i = 7; i >= 0; i--)
    {
        fread(&img_byte, 1, 1, out);
        *data |= (img_byte & 1) << i;
    }
}

int decode_magic_string(char * magic, FILE *f_out)
{
    char decoded_char;
    for (int i = 0; magic[i] != '\0'; i++)
    {
        decode_byte_from_lsb(&decoded_char, f_out);
        //printf("Decoded char: %c\n", decoded_char); -- #, *

        if (decoded_char != magic[i])
        {
            printf("Magic string mismatch.\n");
            return -1;
        }
    }
    printf("Magic string matched\n");
    return 0;
}

int decode_secret_file_size(FILE *f_img)
{
    int size = 0;
    char img_byte;
    for (int i = 31; i >= 0; i--)
    {
        fread(&img_byte, 1, 1, f_img);
        size |= (img_byte & 1) << i;
    }
    return size;
}

void decode_extension(char *ext, int ext_len, FILE *f_img)
{
    for (int i = 0; i < ext_len; i++)
    {
        decode_byte_from_lsb(&ext[i], f_img);
    }
    ext[ext_len] = '\0';
}

void decode_secret_data(FILE *f_img, FILE *f_secret, int size)
{
    char data;
    for (int i = 0; i < size; i++)
    {
        decode_byte_from_lsb(&data, f_img);
        fwrite(&data, 1, 1, f_secret);
    }
}
