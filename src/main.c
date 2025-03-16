/* https://en.wikipedia.org/wiki/Run-length_encoding */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

#define SEQSIZE 128  // SIZE OF (UN)COMPRESSED SEQUENCE
#define FILENAME 256
#define COLOR_GREEN "\033[32m"
#define COLOR_RED   "\033[31m"
#define COLOR_RESET "\033[0m"

void change_extension(char *, size_t, const char *, const char *);

int compress(const char *);
int add_header(FILE *);
int add_checksum(FILE *);

int decompress();

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s [OPTION] FILE...\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; i++)
        compress(argv[i]);
    
    return EXIT_SUCCESS;
}

int compress(const char* ifname) {
    /* OPEN INPUT FILE */
    FILE *in = fopen(ifname, "rb");
    if (in == NULL) {
        fprintf(stderr, "%s: No such file\n", ifname);            
        return EXIT_FAILURE;
    }
    
    /* CHANGE OUTPUT FILE EXTENSION */
    char ofname[FILENAME];
    change_extension(ofname, FILENAME, ifname, ".rle");
    
    /* OPEN OUTPUT FILE */
    FILE *out = fopen(ofname, "wb");
    if (out == NULL) {
        fprintf(stderr, "%s: No such file\n", ofname);            
        return EXIT_FAILURE;
    }

    unsigned char bytes[SEQSIZE];            // SEQUENCE OF BYTES
    unsigned char* ptr_byte;                 // ONE BYTE OUT OF SEQUENCE
    signed char count;                       // COUNT (REPEATED|UNIQUE) BYTES WHILE (POSITIVE|NEGATIVE)

    ptr_byte = bytes;   // GET FIRST BYTE
    count = 0;          // SET TO BE NOT IN ANY SEQUENCE

    // /* READ INPUT FILE BYTE BY BYTE */
    // while (fread(ptr_byte, sizeof(bytes[0]), 1, in)) {
    //     if (count > 0) {
    //         if ((count >= SEQSIZE - 1) || (*ptr_byte != *(ptr_byte - 1))) {
    //             /* WRITE COMPRESSED */
    //             system("clear");
    //             printf("%d ", count);
    //             printf("%02x ", bytes[0]);
    //             fflush(stdout);
    //             getchar();
    //             system("clear");

    //             /* SHIFT TO THE START */
    //             bytes[0] = *ptr_byte;
    //             ptr_byte = bytes;
    //             count = 0;
    //         } else {
    //             ++count;
    //         }
    //     } else if (count < 0) {
    //         if ((abs(count) >= SEQSIZE) || (*ptr_byte == *(ptr_byte - 1))) {
    //             /* WRITE UNCOMPRESSED */
    //             system("clear");
    //             printf("%d\n", count);
    //             for (size_t i = 0; i < abs(count); i++) {
    //                 if ((i != 0) && (i % 16 == 0))
    //                     printf("\n");
    //                 printf("%02x ", bytes[i]);
    //             }
    //             fflush(stdout);
    //             getchar();
    //             system("clear");
                
    //             /* SHIFT TO THE START */
    //             bytes[0] = *ptr_byte;
    //             ptr_byte = bytes;
    //             count = 0;
    //         }
    //         else {
    //             --count;
    //         }
    //     }
    //     if ((count == 0) && (ptr_byte != bytes)) {
    //         if (*ptr_byte == *(ptr_byte - 1))
    //             count = count + 2;
    //         else
    //             count = count - 2;
    //     }
    //     printf("%d\n", count);
    //     usleep(50000);
    //     system("clear");
        
    //     for (size_t i = 0; i < abs(count); i++) {
    //         if ((i != 0) && (i % 16 == 0))
    //             printf("\n");

    //         if (count > 0)
    //             printf(COLOR_RED);
    //         else if (count < 0)
    //             printf(COLOR_GREEN);
            
    //         printf("%02x " COLOR_RESET, bytes[i]);
    //     }
    //     fflush(stdout);

    //     ++ptr_byte;  // GO TO THE NEXT BYTE
    // }

    // if (count > 0) {
    //     system("clear");
    //     printf("%d ", count);
    //     printf("%02x ", bytes[0]);
    //     fflush(stdout);
    //     getchar();
    //     system("clear");
    // } else if (count < 0) {
    //     system("clear");
    //     printf("%d\n", count);
    //     for (size_t i = 0; i < abs(count); i++) {
    //         if ((i != 0) && (i % 16 == 0))
    //             printf("\n");
    //         printf("%02x ", bytes[i]);
    //     }
    //     fflush(stdout);
    //     getchar();
    //     system("clear");
    // }
    // else if (count == 0) {
    //     --count;
    //     system("clear");
    //     printf("%d ", count);
    //     printf("%02x ", bytes[0]);
    //     fflush(stdout);
    //     getchar();
    //     system("clear");
    // }

    /* READ INPUT FILE BYTE BY BYTE */
    while (fread(ptr_byte, sizeof(bytes[0]), 1, in)) {
        if (count > 0) {
            if ((count >= SEQSIZE - 1) || (*ptr_byte != *(ptr_byte - 1))) {
                /* WRITE COMPRESSED */
                fwrite(&count, sizeof(count), 1, out);
                fwrite(bytes, sizeof(bytes[0]), 1, out);

                /* SHIFT TO THE START */
                bytes[0] = *ptr_byte;
                ptr_byte = bytes;
                count = 0;
            } else {
                ++count;
            }
        } else if (count < 0) {
            if ((abs(count) >= SEQSIZE) || (*ptr_byte == *(ptr_byte - 1))) {
                /* WRITE UNCOMPRESSED */
                fwrite(&count, sizeof(count), 1, out);
                fwrite(bytes, sizeof(bytes[0]), abs(count), out);
                
                /* SHIFT TO THE START */
                bytes[0] = *ptr_byte;
                ptr_byte = bytes;
                count = 0;
            }
            else {
                --count;
            }
        }
        if ((count == 0) && (ptr_byte != bytes)) {
            if (*ptr_byte == *(ptr_byte - 1))
                count = count + 2;
            else
                count = count - 2;
        }
        ++ptr_byte;  // GO TO THE NEXT BYTE
    }

    if (count > 0) {
        fwrite(&count, sizeof(count), 1, out);
        fwrite(bytes, sizeof(bytes[0]), 1, out);
    } else if (count < 0) {
        fwrite(&count, sizeof(count), 1, out);
        fwrite(bytes, sizeof(bytes[0]), abs(count), out);
    }
    else if (count == 0) {
        fwrite(&count, sizeof(count), 1, out);
        fwrite(bytes, sizeof(bytes[0]), 1, out);
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
