#include <stdlib.h>
#include <stdio.h>

int compress(FILE *, FILE *);
int add_header(FILE *);
int add_checksum(FILE *);


int decompress();



int copybinf(FILE *, FILE *);
int readbinf(FILE *, int);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s [FILE]...\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; i++) {
        FILE *in = fopen(argv[i], "rb");
        if (in == NULL) {
            perror("ERROR OPENING FILE");
            return EXIT_FAILURE;
        }
    
        printf("\x1b[31m%s\x1b[0m:\n", argv[i]);
        readbinf(in, 16);
        
        fclose(in);
    }

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