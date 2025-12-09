#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../../utils/fileread.h"
#include "../../utils/split_string.h"

double number_of_invalids(char **ids, int ids_length);

int main()
{
    int input_length = 0;
    char *input = fileread("input.txt", &input_length);

    int split_inputs_on_commas_length;
    int *each_newline_length;

    char **split_inputs_on_commas = split_string(input, strlen(input), ",", &split_inputs_on_commas_length, &each_newline_length);

    free(each_newline_length);

    // printf("Answer P1: %f\n", number_of_invalids(split_inputs_on_commas, split_inputs_on_commas_length));
    printf("Answer P2: %f\n", number_of_invalids(split_inputs_on_commas, split_inputs_on_commas_length));

    free(split_inputs_on_commas);
    free(input);

    return 0;
}

double check_id_rangep1(double id_start, double id_end)
{
    double invalid_id_sum = 0;

    for (double check_ids = id_start; check_ids <= id_end; check_ids++)
    {
        char initial_id_value[20];

        sprintf(initial_id_value, "%f", check_ids);

        int split_on_pre_decimal_length;
        int *split_on_pre_decimal_lengths;
        char **only_take_pre_decimal = split_string(initial_id_value, strlen(initial_id_value), ".", &split_on_pre_decimal_length, &split_on_pre_decimal_lengths);

        char *cut_id_value = only_take_pre_decimal[0];
        int id_value_length = strlen(cut_id_value);

        if (id_value_length % 2 == 1)
        {
            continue;
        }

        char **id_value_halves = substring(cut_id_value, id_value_length * 0.5);

        free(only_take_pre_decimal[0]);
        free(only_take_pre_decimal[1]);

        free(split_on_pre_decimal_lengths);
        free(only_take_pre_decimal);

        if (strcmp(id_value_halves[0], id_value_halves[1]) == 0)
        {
            invalid_id_sum += check_ids;
        }

        free(id_value_halves[0]);
        free(id_value_halves[1]);

        free(id_value_halves);
    }

    return invalid_id_sum;
}

// Only access double_string_split[0]!!
// Make sure to free!!
char **get_double_string_without_decimal(char *double_string)
{
    int no;
    int *no_no;

    char **double_string_split = split_string(double_string, strlen(double_string), ".", &no, &no_no);

    free(no_no);

    free(double_string_split[1]);

    return double_string_split;
}

double check_id_rangep2(double id_start, double id_end)
{
    double invalid_id_sum = 0;

    for (double check_ids = id_start; check_ids <= id_end; check_ids++)
    {
        char initial_id_value[20];

        sprintf(initial_id_value, "%f", check_ids);
        char **double_string_split = get_double_string_without_decimal(initial_id_value);
        char *id_value = double_string_split[0];

        // brute force it, no one can stop me.
        int id_value_length = strlen(id_value);
        int max_value_length = id_value_length * 0.5;


        // Try every size from 1 to right less than half.
        for (int attempted_size = 1; attempted_size <= max_value_length; attempted_size++)
        {
            // Kill if it's not even possible.
            if (id_value_length % attempted_size != 0)
            {
                continue;
            }

            // Check how many positions there are to look at for a given attempted size.
            int position_num = id_value_length / attempted_size;

            int okay = 1;

            for (int position_check = 0; position_check < position_num - 1; position_check++)
            {
                int check_attempted_size_positions = 0;

                for (; check_attempted_size_positions < attempted_size; check_attempted_size_positions++)
                {
                    char first_pos = id_value[(position_check * attempted_size) + check_attempted_size_positions];
                    char second_pos = id_value[((position_check + 1) * attempted_size) + check_attempted_size_positions];

                    if (first_pos != second_pos)
                    {
                        break;
                    }
                }

                if (check_attempted_size_positions != attempted_size)
                {
                    okay = 0;
                }
            };

            if (okay)
            {
                invalid_id_sum += check_ids;
                break;
            }
        }
    }

    return invalid_id_sum;
}

double number_of_invalids(char **ids, int ids_length)
{
    double invalid_id_sum = 0;

    for (int id_index = 0; id_index < ids_length; id_index++)
    {
        int split_id_on_dash_length;
        int *id_length;

        char **id_range = split_string(ids[id_index], strlen(ids[id_index]), "-", &split_id_on_dash_length, &id_length);

        double id_start = strtod(id_range[0], NULL);
        double id_end = strtod(id_range[1], NULL);

        invalid_id_sum += check_id_rangep2(id_start, id_end);

        free(id_range[0]);
        free(id_range[1]);

        free(id_range);

        free(id_length);
    }

    return invalid_id_sum;
}
