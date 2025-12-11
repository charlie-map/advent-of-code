
#include <stdio.h>

void print_string_arrays(char** string, int length) {
    printf("[\n");

    for (int i = 0; i < length - 1; i++) {
        printf("  \"%s\",\n", string[i]);
    }

    printf("  \"%s\"\n", string[length - 1]);
    printf("]\n");
    return;
}

void print_double_grid(double** doubles, int length) {
    printf("[\n");

    for (int i = 0; i < length - 1; i++) {
        printf("  %f - %f,\n", doubles[i][0], doubles[i][1]);
    }

    printf("  %f - %f\n", doubles[length - 1][0], doubles[length - 1][1]);
    printf("]\n");
    return;
}
