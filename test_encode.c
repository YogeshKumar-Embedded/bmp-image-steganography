#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include "common.h"

int main(int argc, char **argv)
{   
    OperationType check_operation_type(char *argv[])
    {
        if(strcmp(argv[1],"-e") == 0)
        {
            return e_encode;
        }
        else if(strcmp(argv[1],"-d") == 0)
        {
            return e_decode;
        }
        else
        {
            return e_unsupported;
        }
    }

    if(check_operation_type(argv) == e_encode)
    {
        printf("Selected encoding\n");
        EncodeInfo encode;
        if(read_and_validate_encode_args(argv, &encode ) == e_success)
        {
            printf("Read and validate encode arguments is success\n");
            printf("<--------------Started Encoding-------------->\n");
            if(do_encoding(&encode) == e_success)
            {
                printf("Encoding Successful\n");
            }
            else
            {
                printf("Failed to perform encoding\n");
                return -2;
            }
        }
        else
        {
            printf("Validation of encode arguments Failure\n");
            return -1;
        }
    }
    else if(check_operation_type(argv) == e_decode)
    {
        printf("Selected decoding\n");
        DecodeInfo decode;
        if(read_and_validate_decode_args(argv, &decode ) == e_success)   
        {
            printf("Read and validate decode arguments is success\n");
            printf("<--------------Started Decoding-------------->\n");
            if(do_decoding(&decode) == e_success)                 
            {
                printf("Decoding Successful\n");
            }
            else
            {
                printf("Failed to Decode\n");
                return -2;
            }
        }
        else
        {
            printf("Validation of decode arguments Failure\n");
            return -1;
        }
    }
    return 0;
}
