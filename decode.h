#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include "types.h" 

typedef struct _DecodeInfo
{
    char *spy_image_fname;
    FILE *fptr_spy_image;

    char *decoded_fname;
    FILE *decoded_fptr_secret;

    int decoded_extn_size;
    char decoded_extn[10];

    int decoded_size_file;

} DecodeInfo;

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);
Status open_decode_files(DecodeInfo *decInfo);
Status do_decoding(DecodeInfo *decInfo);
Status decode_magic_string(char *magic_string,  DecodeInfo *decInfo);
Status decode_image_data_to_char(char *data, int size, DecodeInfo *decInfo);
Status decode_image_data_to_int(int *value, FILE *fptr_stego);
Status decode_secret_file_extn_size(int *size, FILE *fptr_stego);
Status decode_secret_file_extn(char *buffer, int size, DecodeInfo *decInfo);
Status decode_secret_file_size(int *file_size, FILE *fptr_stego);
Status decode_secret_file_data(DecodeInfo *decInfo);


#endif