#include <stdlib.h>
#include <stdio.h>

int compress();
int decompress();

int readbinf(FILE *, int);

int main() {
    FILE *file = fopen("./test/finp.tst", "rb");

    if (readbinf(file, 16))
        perror("Error reading file\n");

    
    fclose(file);
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

    if (ferror(file))
        perror("Error reading file\n");

    return EXIT_SUCCESS;
}