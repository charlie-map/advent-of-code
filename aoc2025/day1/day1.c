#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../../utils/fileread.h"
#include "../../utils/split_string.h"

int get_number_of_times_at_0(char **instructions, int instruction_length);

int main()
{
    int input_length = 0;
    char *input = fileread("input.txt", &input_length);

    int split_inputs_on_newline_length;
    int *each_newline_length;

    char **split_inputs_on_newline = split_string(input, strlen(input), "\n", &split_inputs_on_newline_length, &each_newline_length);

    free(each_newline_length);

    printf("Answer P1: %d\n", get_number_of_times_at_0(split_inputs_on_newline, split_inputs_on_newline_length));

    free(split_inputs_on_newline);
    free(input);

    return 0;
}

int get_number_of_times_at_0(char **instructions, int instruction_length)
{
    int zero_hits = 0;
    int current_position = 50;

    for (int instruction_index = 0; instruction_index < instruction_length; instruction_index++) {
        char* instruction = instructions[instruction_index];

        char** split = substring(instruction, 1);

        int raw_number = atoi(split[1]);

        printf("Checking line %s: %s %d with curr %d\n", instruction, split[0], raw_number, current_position);

        int clicks_past_zero = (int)floor(raw_number / 100);

        printf("clicks past current (%d) with new (%d) %d\n", current_position, raw_number, clicks_past_zero);

        zero_hits += clicks_past_zero;

        int number = raw_number % 100;

        if (strcmp(split[0], "R") == 0) {
            current_position += number;

            if (current_position >= 100) {
                zero_hits++;
                current_position -= 100;
            } else if (current_position == 0) {
                zero_hits++;
            }
        } else {
            current_position -= number;

            if (current_position < 0) {
                zero_hits++;
                current_position += 100;
            } else if (current_position == 0) {
                zero_hits++;
            }
        }

        free(split[0]); 
        free(split[1]); 
        free(split);

        printf("Get hits %d\n", zero_hits);
    }

    return zero_hits;
}
