#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define FILENAME 256
#define COLOR_GREEN "\033[32m"
#define COLOR_RED   "\033[31m"
#define COLOR_RESET "\033[0m"

int compress(FILE *, FILE *);
int add_header(FILE *);
int add_checksum(FILE *);

int decompress();


int read_binary(FILE *);
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
        read_binary(in);
        
        fclose(in);
    }
    
    return EXIT_SUCCESS;
}

int read_binary(FILE *file) {
    bool isrepeated = false;
    
    unsigned char byte, prevb;

    unsigned int count = 1;
    while (fread(&byte, sizeof(byte), 1, file)) {
        switch (isrepeated) {
        case false:
            if (byte == prevb) {
                isrepeated = true;
                ++count;
            } else printf("%02x ", byte);
            break;
        case true:
            if (byte != prevb) {
                isrepeated = false;
                printf(COLOR_RED "%u" COLOR_GREEN "%02x" COLOR_RESET " ", count, prevb);
                printf("%02x ", byte);
                count = 1;
            } else ++count;
            break;
        }
        prevb = byte;
    }

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
