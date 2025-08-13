#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEQSIZE 128 // SIZE OF (UN)COMPRESSED SEQUENCE
#define FILENAME 256

int compress(const char *);
void change_extension(char *, size_t, const char *, const char *);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s FILE...\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; i++)
        compress(argv[i]);

    return EXIT_SUCCESS;
}

int compress(const char *ifname) {
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

    unsigned char bytes[SEQSIZE]; // SEQUENCE OF BYTES
    unsigned char *ptr_byte;      // ONE BYTE OUT OF SEQUENCE
    signed char
        count; // COUNT (REPEATED|UNIQUE) BYTES WHILE (POSITIVE|NEGATIVE)

    ptr_byte = bytes; // GET FIRST BYTE
    count = 0;        // SET TO BE NOT IN ANY SEQUENCE

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
            } else {
                --count;
            }
        }
        if ((count == 0) && (ptr_byte != bytes)) {
            if (*ptr_byte == *(ptr_byte - 1))
                count = count + 2;
            else
                count = count - 2;
        }
        ++ptr_byte; // GO TO THE NEXT BYTE
    }

    /* WRITE LEFT BYTES */
    if (count > 0) {
        fwrite(&count, sizeof(count), 1, out);
        fwrite(bytes, sizeof(bytes[0]), 1, out);
    } else if (count < 0) {
        fwrite(&count, sizeof(count), 1, out);
        fwrite(bytes, sizeof(bytes[0]), abs(count), out);
    } else if (count == 0) {
        fwrite(&count, sizeof(count), 1, out);
        fwrite(bytes, sizeof(bytes[0]), 1, out);
    }

    return EXIT_SUCCESS;
}

void change_extension(char *dest, size_t maxname, const char *src,
                      const char *ext) {
    strncpy(dest, src, maxname - 1);
    dest[maxname - 1] = '\0';

    char *dotpos = strrchr(dest, '.');
    if (dotpos != NULL)
        strcpy(dotpos, ext);
    else
        strncat(dest, ext, maxname - strlen(dest) - 1);
}
