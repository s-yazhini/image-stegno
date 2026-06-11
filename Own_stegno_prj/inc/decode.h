#ifndef DECODE_H
#define DECODE_H

int do_decoding(char *stego_img);
void decode_byte_from_lsb(char *data, FILE *src);
int decode_magic_string(char * magic, FILE *f_out);
int decode_secret_file_size(FILE *f_img);
void decode_extension(char *ext, int ext_len, FILE *f_img);
void decode_secret_data(FILE *f_img, FILE *f_secret, int size);
#endif
