#ifndef ENCODE_H
#define ENCODE_H

void encode_byte_to_lsb(char data, FILE *src, FILE *dst);
int do_encoding(char *src_img, char *secret, char *out_img);
int encode_magic_string(const char *magic, FILE *src, FILE *dst);
int encode_secret_file_size(int size, FILE *src, FILE *out);
int encode_secret_file_extn_size(int , FILE*, FILE*);
int encode_ext(char* extn, FILE *src, FILE *out);
int encode_data(FILE *secret, FILE *f_src, FILE *f_out);
#endif
