#include <stdio.h>
#include <stdlib.h>

int decompress(const char *, const char *);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s SOURCE ... TARGET\n", argv[0]);
        return EXIT_FAILURE;
    }

    decompress(argv[1], argv[2]);

    return EXIT_SUCCESS;
}

int decompress(const char *ifname, const char *ofname) {
    /* OPEN INPUT FILE */
    FILE *in = fopen(ifname, "rb");
    if (in == NULL) {
        fprintf(stderr, "%s: No such file\n", ifname);
        return EXIT_FAILURE;
    }

    /* OPEN OUTPUT FILE */
    FILE *out = fopen(ofname, "wb");
    if (out == NULL) {
        fprintf(stderr, "%s: No such file\n", ofname);
        return EXIT_FAILURE;
    }

    unsigned char byte;
    signed char count;

    /* READ INPUT FILE BYTE BY BYTE */
    while (fread(&byte, sizeof(byte), 1, in)) {
        count = byte;
        /* WRITE COMPRESSED SEQUENCE */
        if (count > 0) {
            fread(&byte, sizeof(byte), 1, in);
            for (int i = count; i > 0; --i)
                fwrite(&byte, sizeof(byte), 1, out);
            /* WRITE UNCOMPRESSED SEQUENCE */
        } else if (count < 0) {
            for (int i = count; i < 0; ++i) {
                fread(&byte, sizeof(byte), 1, in);
                fwrite(&byte, sizeof(byte), 1, out);
            }
        }
    }
    return EXIT_SUCCESS;
}
