#include <stdlib.h>
#include <stdio.h>

int compress();
int decompress();

int copybinf(FILE *, FILE *);
int readbinf(FILE *, int);

int main() {
    FILE *in = fopen("./test/finp.tst", "rb");
    FILE *out = fopen("./test/finp.rle", "wb");

    copybinf(in, out);
    
    fclose(in);
    return EXIT_SUCCESS;
}

int copybinf(FILE *in, FILE *out) {
    if (in == NULL) {
        perror("Error opening input file\n");
        return EXIT_FAILURE;
    }
    else if (out == NULL) {
        perror("Error opening output file\n");
        return EXIT_FAILURE;
    }

    unsigned char byte;
    while (fread(&byte, sizeof(byte), 1, in)) {
        fwrite(&byte, sizeof(byte), 1, out);
    }

    if (ferror(in) || ferror(out)) {
        perror("Error writing into file\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int readbinf(FILE *file, const int maxcol) {
    int col = maxcol;

    if (file == NULL) {
        perror("Error opening file\n");
        return EXIT_FAILURE;
    }

    unsigned char byte;
    while (fread(&byte, sizeof(byte), 1, file)) {
        printf("%02x ", byte);

        if (--col < 1) {
            col = maxcol;
            printf("\n");
        }
    }
    printf("\n");

    if (ferror(file)) {
        perror("Error reading file\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}