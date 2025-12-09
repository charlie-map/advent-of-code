#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../../utils/fileread.h"
#include "../../utils/split_string.h"

double get_the_jigawatts(char** batteries, int battery_length);

int main()
{
    int input_length = 0;
    char* input = fileread("input.txt", &input_length);

    int split_inputs_on_newline_length;
    int* each_newline_length;

    char** split_inputs_on_newline = split_string(input, strlen(input), "\n", &split_inputs_on_newline_length, &each_newline_length);

    free(each_newline_length);

    printf("Answer P: %f\n", get_the_jigawatts(split_inputs_on_newline, split_inputs_on_newline_length));

    free(split_inputs_on_newline);
    free(input);

    return 0;
}

char* find_biggest_jigawatt(char* battery, int curr_index, int rest_of_size) {
    // printf("%s\n", battery);

    // Find largest number index 
    // 9899
    int largest_number = 0;
    int largest_number_index = 0;

    // Don't use the last position - that'll obviously be a smaller number (like 63249 - 9 would be smaller than 69, nice).
    for (int battery_index = curr_index; battery_index < strlen(battery) - rest_of_size; battery_index++) {
        int current_number = battery[battery_index] - '0';

        if (current_number > largest_number) {
            largest_number = current_number;
            largest_number_index = battery_index;
        }
    }

    // printf("largest number %d\n", largest_number);

    if (rest_of_size == 0) {
        char* base_big_number = malloc(sizeof(char) * 2);
        sprintf(base_big_number, "%d", largest_number);

        // printf("collecting base case %s\n", base_big_number);

        return base_big_number;
    }

    char* inner_number = find_biggest_jigawatt(battery, largest_number_index + 1, rest_of_size - 1);

    // Now we add this onto our number.
    char* new_number = malloc(sizeof(char) * (strlen(inner_number) + 2));
    new_number[0] = largest_number + '0';
    strcpy(new_number + sizeof(char), inner_number);

    free(inner_number);

    return new_number;
};

double get_the_jigawatts(char** batteries, int battery_length) {
    double total_jigawatts = 0;

    for (int battery_index = 0; battery_index < battery_length; battery_index++) {
        char* battery = batteries[battery_index];

        char* jigawatt_combo = find_biggest_jigawatt(battery, 0, 11);
        double single_jigawat = strtod(jigawatt_combo, NULL);

        printf("battery %s got jigawatt %f\n", battery, single_jigawat);

        free(jigawatt_combo);
        total_jigawatts += single_jigawat;
    }

    return total_jigawatts;
}
