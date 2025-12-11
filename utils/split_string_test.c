#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "split_string.h"
#include "string_helpers.h"

int test_split_string_on_multi_character_delimiter() {
    char* test_input = "blabblab\n\nblack\nblab";

    char** expected = malloc(sizeof(char*) * 2);
    expected[0] = "blabblab";
    expected[1] = "black\nblab";

    int output_length;
    int* outputs_length;
    char** output = split_string(test_input, strlen(test_input), "\n\n", &output_length, &outputs_length);

    assert(strcmp(output[0], expected[0]) == 0);
    assert(strcmp(output[1], expected[1]) == 0);

    free(output[0]);
    free(output[1]);
    free(outputs_length);
    free(output);

    free(expected);

    return 0;
}

int main() {
    test_split_string_on_multi_character_delimiter();

    printf("TEST PASSED\n");

    return 0;
}
