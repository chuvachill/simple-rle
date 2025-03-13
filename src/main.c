#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define FILENAME 256

int compress(FILE *, FILE *);
int add_header(FILE *);
int add_checksum(FILE *);

int decompress();


int read_binary(FILE *, int);
void change_extension(char *, size_t, const char *, const char *);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s FILE...\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; i++) {
        /* OPEN INPUT FILE */
        FILE *in = fopen(argv[i], "rb");
        if (in == NULL) {
            fprintf(stderr, "%s: No such file\n", argv[i]);            
            return EXIT_FAILURE;
        }

        /* CREATE AND OPEN OUTPUT FILE */
        char new_filename[FILENAME];
        change_extension(new_filename, FILENAME, argv[i], ".rle");

        FILE *out = fopen(new_filename, "wb");
        if (out == NULL) {
            fprintf(stderr, "Error occured while compressing\n");
            return EXIT_FAILURE;
        }
    
        /* READ INPUT FILE BYTE BY BYTE */
        read_binary(in, 16);
        
        fclose(in);
    }
    
    return EXIT_SUCCESS;
}

int read_binary(FILE *file, const int maxcol) {
    int col = maxcol;
    
    unsigned char byte;
    unsigned char prevb = 0;
    while (fread(&byte, sizeof(byte), 1, file)) {
        if (byte == prevb)
            printf("\x1b[31m%02x \x1b[0m", byte);
        else
            printf("%02x ", byte);

        if (--col < 1) {
            col = maxcol;
            printf("\n");
        }

        prevb = byte;
    }
    printf("\n");

    if (ferror(file)) {
        perror("Error reading file\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void change_extension(char* dest, size_t maxname, const char* src, const char* ext) {
    strncpy(dest, src, maxname - 1);
    dest[maxname - 1] = '\0';

    char *dotpos = strrchr(dest, '.');
    if (dotpos != NULL)
        strcpy(dotpos, ext);
    else
        strncat(dest, ext, maxname - strlen(dest) - 1);
}
