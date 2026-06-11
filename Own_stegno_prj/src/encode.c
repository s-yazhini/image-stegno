#include <stdio.h>
#include<string.h>
#include "encode.h"

int do_encoding(char *src_img, char *secret, char *out_img)
{
/*
filename == phone number
FILE *	== active phone call
*/
    FILE *f_src = fopen(src_img, "rb");
    FILE *f_secret = fopen(secret, "rb");
    FILE *f_out = fopen(out_img, "wb");

    if (!f_src || !f_out || !f_secret)
    {
        printf("File open failed\n");
        return 1;
    }

    // STEP 1: Copy BMP header
    char header[54];
    //take (one by one) 54 bytes from src and put it inside header.
    fread(header, 1, 54, f_src);
    //take (one by one) 54 bytes from header and put it inside fout. So, copy done!
    fwrite(header, 1, 54, f_out);

    //STEP 2:  ENCODING
    //STEP 2.1: encode magic string to image
    encode_magic_string("#*", f_src, f_out);
    
    //STEP 2.2:
    //Finding secret.txt file size (i.e. number of chars)
    fseek(f_secret, 0, SEEK_END);
    int secret_size = ftell(f_secret);
    rewind(f_secret);
    //printf(" %d ", secret_size); -- 14
    
    //Encode secret file size (int → 32 bits)
    encode_secret_file_size(secret_size, f_src, f_out);

    //STEP 2.3: encode secret file extension length 
    encode_secret_file_extn_size(strlen("txt"), f_src, f_out);

    //STEP 2.4: encode extention chars t,x,t
    encode_ext("txt", f_src, f_out);

    //STEP 2.5: encoding secret data
    encode_data(f_secret, f_src, f_out);

    //STEP 3: copy remaining bytes
    unsigned char temp;
    while (fread(&temp, 1, 1, f_src) == 1)
    {
        fwrite(&temp, 1, 1, f_out);
    }

    fclose(f_src);
    fclose(f_out);
    fclose(f_secret);

    printf("Encode successful, check output.bmp\n");
    return 0;
}

void encode_byte_to_lsb(char data, FILE *src, FILE *dst)
{
    unsigned char img_byte;
    for (int i = 7; i >= 0; i--)
    {
        /*Take 1 byte (eg: here 55th byte in 1st iteration)
        From src file
        Put it into variable img_byte
        */
        fread(&img_byte, 1, 1, src);

        /*Now manipulate that 1 byte's LSB with the a bit from magic string*/
        //cur byte = (clear the lsb) +(or op)  (1 bit from data eg: #)
        img_byte = (img_byte & 0xFE) | ((data >> i) & 1);

        //copy that modified 1 byte to output file.
        fwrite(&img_byte, 1, 1, dst);

        /*repeate this untill we reach no bits left in passed char 
        (eg: magic st char #) using for loop.*/
    }
}

int encode_magic_string(const char *magic, FILE *src, FILE *dst)
{
    for (int i = 0; magic[i] != '\0'; i++)
    {
        encode_byte_to_lsb(magic[i], src, dst);
    }
    return 0;
}

int encode_secret_file_size(int size, FILE *src, FILE *dst)
{
    unsigned char img_byte;
    for (int i = 31; i >= 0; i--)
    {
        fread(&img_byte, 1, 1, src);
        img_byte = (img_byte & 0xFE) | ((size >> i) & 1);
        fwrite(&img_byte, 1, 1, dst);
    }
    return 0;
}

int encode_ext(char* extn, FILE *src, FILE *dst)
{
    for(int i=0; extn[i]!='\0'; i++)
        encode_byte_to_lsb(extn[i], src, dst);

    return 0;
}

int encode_secret_file_extn_size(int extn_size, FILE* src, FILE* out)
{
    unsigned char temp;
    for(int i=31; i>=0; i--)
    {
        fread(&temp, 1, 1, src);
        temp = (temp & 0xFE) | ((extn_size>>i)&1);
        fwrite(&temp, 1, 1, out);
    }
    return 0;
}

int encode_data(FILE *secret_data, FILE *src, FILE *dst)
{
    unsigned char data;
    while(fread(&data, 1, 1, secret_data) == 1)
    {
        encode_byte_to_lsb(data, src, dst);
    }
    return 0;
}

