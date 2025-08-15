#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decode.h"
#include "types.h"
#include "common.h"

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    // Validate image file
    if (strstr(argv[2], ".bmp") != NULL)
    {
        decInfo->spy_image_fname = argv[2];
    }
    else
    {
        fprintf(stderr, "Invalid stego image file. Must be .bmp\n");
        return e_failure;
    }

    // Output file (optional)
    if (argv[3] != NULL)
    {
        decInfo->decoded_fname = argv[3];
    }
    else
    {
        decInfo->decoded_fname = "decoded.txt";
    }

    return e_success;
}

Status open_decode_files(DecodeInfo *decInfo)
{
    // Open stego image
    decInfo->fptr_spy_image = fopen(decInfo->spy_image_fname, "r");
    if (decInfo->fptr_spy_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->spy_image_fname);
        return e_failure;
    }

    // Open output file
    decInfo->decoded_fptr_secret = fopen(decInfo->decoded_fname, "w");
    if (decInfo->decoded_fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->decoded_fname);
        return e_failure;
    }

    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    if (open_decode_files(decInfo) != e_success)
    {
        printf("Open file failed\n");
        return e_failure;
    }
    printf("Open file successful\n");

    if (decode_magic_string(MAGIC_STRING, decInfo) != e_success)
    {
        printf("Magic string matching failed\n");
        return e_failure;
    }
    printf("Magic string match successful\n");

    if (decode_secret_file_extn_size(&decInfo->decoded_extn_size, decInfo->fptr_spy_image) != e_success)
    {
        printf("Decoding secret file extension size failed\n");
        return e_failure;
    }
    printf("Decoding secret file extension size successful\n");

    if (decode_secret_file_extn(decInfo->decoded_extn, decInfo->decoded_extn_size, decInfo) != e_success)
    {
        printf("Decoding secret file extension failed\n");
        return e_failure;
    }
    printf("Decoding secret file extension successful\n");
    if (decode_secret_file_size(&decInfo->decoded_size_file, decInfo->fptr_spy_image) != e_success)
    {
        printf("Decoding secret file size failed\n");
        return e_failure;
    }
    printf("Decoding secret file size successful\n");

    if (decode_secret_file_data(decInfo) != e_success)
    {
        printf("Decoding secret file data failed\n");
        return e_failure;
    }
    printf("Decoding secret file successful\n");

    fclose(decInfo->fptr_spy_image);
    fclose(decInfo->decoded_fptr_secret);
    return e_success;
}

Status decode_magic_string(char *magic_string, DecodeInfo *decInfo)
{
    char decoded[strlen(magic_string) + 1];
    fseek(decInfo->fptr_spy_image, 54, SEEK_SET);  // Skip BMP header

    if (decode_image_data_to_char(decoded, strlen(magic_string), decInfo) != e_success)
    {
        return e_failure;
    }
    decoded[strlen(magic_string)] = '\0';  
    // printf("Decoded magic string: \"%s\"\n", decoded);
    if (strcmp(decoded, magic_string) == 0)
    {
        return e_success;
    }
    else
    {
        fprintf(stderr, "Magic string mismatch.\n");
        return e_failure;
    }
}

Status decode_image_data_to_char(char *data, int size, DecodeInfo *decInfo)
{
    for (int i = 0; i < size; i++)
    {
        char ch = 0;
        for (int bit = 0; bit < 8; bit++)
        {
            char byte;
            if (fread(&byte, sizeof(char), 1, decInfo->fptr_spy_image) != 1)
            {
                fprintf(stderr, "Error reading from stego image.\n");
                return e_failure;
            }
            // LSB first (bit 0 at position 0)
            ch |= ((byte & 1) << bit);
        }
        data[i] = ch;
    }
    data[size] = '\0';  // null-terminate
    return e_success;
}

Status decode_image_data_to_int(int *value, FILE *fptr_stego)
{
    *value = 0;
    for (int i = 0; i < 32; i++)
    {
        char byte;
        if (fread(&byte, sizeof(char), 1, fptr_stego) != 1)
        {
            fprintf(stderr, "Error reading from stego image\n");
            return e_failure;
        }
        *value |=  ((byte & 1) <<i);
    }
    return e_success;
}

Status decode_secret_file_extn_size(int *size, FILE *fptr_stego)
{
    return decode_image_data_to_int(size, fptr_stego);
}

Status decode_secret_file_extn(char *buffer, int size, DecodeInfo *decInfo)
{
    Status status = decode_image_data_to_char(buffer, size, decInfo);
    if (status == e_success)
    {
        buffer[size] = '\0'; 
    }
    return status;
}

Status decode_secret_file_size(int *file_size, FILE *fptr_stego)
{
    return decode_image_data_to_int(file_size, fptr_stego);
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char *str = (char *)malloc(decInfo->decoded_size_file);
    if (str == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed for secret file data.\n");
        return e_failure;
    }

    if (decode_image_data_to_char(str, decInfo->decoded_size_file, decInfo) != e_success)
    {
        free(str);
        return e_failure;
    }

    if (fwrite(str, sizeof(char), decInfo->decoded_size_file, decInfo->decoded_fptr_secret) != decInfo->decoded_size_file)
    {
        fprintf(stderr, "Failed to write decoded data to output file.\n");
        free(str);
        return e_failure;
    }

    free(str);
    return e_success;
}