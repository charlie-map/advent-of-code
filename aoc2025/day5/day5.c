#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>

#include "../../utils/fileread.h"
#include "../../utils/split_string.h"
#include "../../utils/string_helpers.h"

int get_fresh_ingredients(char **fresh_ingredients_range, int fresh_ingredients_length, char **aviailable_ingredients, int available_ingredients_length);
int get_fresh_ingredients_v2(char **fresh_ingredients_range, int fresh_ingredients_length);

int main()
{
    int input_length = 0;
    char *input = fileread("input_test.txt", &input_length);

    int _ingreds;
    int *_ingreds_length;

    char **all_ingredients = split_string(input, strlen(input), "\n\n", &_ingreds, &_ingreds_length);

    free(_ingreds_length);

    // Get freshies.
    int fresh_ingredients_length;
    int *_fresh_ingredients_size;
    char **fresh_ingredients_range = split_string(all_ingredients[0], strlen(all_ingredients[0]), "\n", &fresh_ingredients_length, &_fresh_ingredients_size);

    free(_fresh_ingredients_size);
    free(all_ingredients[0]);

    // Get availablees.
    int available_ingredients_length;
    int *_available_ingredients_size;
    char **available_ingredients = split_string(all_ingredients[1], strlen(all_ingredients[1]), "\n", &available_ingredients_length, &_available_ingredients_size);

    free(_available_ingredients_size);
    free(all_ingredients[1]);

    free(all_ingredients);

    // printf("Answer P1: %d\n", get_fresh_ingredients(fresh_ingredients_range, fresh_ingredients_length, available_ingredients, available_ingredients_length));

    print_string_arrays(fresh_ingredients_range, fresh_ingredients_length);
    printf("Answer P2: %d\n", get_fresh_ingredients_v2(fresh_ingredients_range, fresh_ingredients_length));

    return 0;
}

int is_number_in_fresh_ingredients_range(char **fresh_ingredients_range, int fresh_ingredients_length, double ingredient_number)
{
    for (int i = 0; i < fresh_ingredients_length; i++)
    {
        int _errr;
        int *_rrrrrr;

        char **range = split_string(fresh_ingredients_range[i], strlen(fresh_ingredients_range[i]), "-", &_errr, &_rrrrrr);

        double start_range = strtod(range[0], NULL);
        double end_range = strtod(range[1], NULL);

        free(_rrrrrr);
        free(range[0]);
        free(range[1]);
        free(range);

        if (start_range <= ingredient_number && ingredient_number <= end_range)
        {
            return 1;
        }
    };

    return 0;
}

int get_fresh_ingredients(char **fresh_ingredients_range, int fresh_ingredients_length, char **aviailable_ingredients, int available_ingredients_length)
{
    int fresh_ingredient_count = 0;

    for (int avail_ing_index = 0; avail_ing_index < available_ingredients_length; avail_ing_index++)
    {
        double ingredient_number = strtod(aviailable_ingredients[avail_ing_index], NULL);

        // Check if the ingredient is within any of the ranges
        fresh_ingredient_count += is_number_in_fresh_ingredients_range(fresh_ingredients_range, fresh_ingredients_length, ingredient_number);
    }

    return fresh_ingredient_count;
}

double *get_min_max(char *fresh_ingredient_range)
{
    int _errr;
    int *_rrrrrr;

    char **range = split_string(fresh_ingredient_range, strlen(fresh_ingredient_range), "-", &_errr, &_rrrrrr);

    printf("get range %s-%s\n", range[0], range[1]);
    double start_range = strtod(range[0], NULL);
    double end_range = strtod(range[1], NULL);

    free(range[0]);
    free(range[1]);
    free(_rrrrrr);
    free(range);

    double *final_range = malloc(sizeof(double) * 2);
    final_range[0] = start_range;
    final_range[1] = end_range;

    return final_range;
}

// If returns 1, there was crossover and the first min_max passed in is updated with the new values.
// If returns 0, there was no crossover and both values are left the same.
int handle_crossover(double *min_max1, double *min_max2)
{
    // If one's max is greater than the min of the other (but also falls within the range).
    if (min_max1[1] >= min_max2[0] && min_max1[1] < min_max2[1])
    {
        min_max1[0] = min_max1[0] < min_max2[0] ? min_max1[0] : min_max2[0];
        min_max1[1] = min_max1[1] > min_max2[1] ? min_max1[1] : min_max2[1];
        return 1;
    }

    // If one's min is less than the max of the other.
    if (min_max1[0] <= min_max2[1] && min_max1[0] > min_max2[0])
    {
        min_max1[1] = min_max1[1] > min_max2[1] ? min_max1[1] : min_max2[1];
        min_max1[0] = min_max1[0] < min_max2[0] ? min_max1[0] : min_max2[0];
        return 1;
    }

    return 0;
}

double** dedupe_ranges(double** deduped_ranges, int* deduped_ranges_length) {
    for (int i = 0; i < *deduped_ranges_length; i++)
    {
        double* min_max = deduped_ranges[i];

        // loop through to check.
        int mashed_into_another_value = 0;
        for (int check_deduped_index = i + 1; check_deduped_index < *deduped_ranges_length; check_deduped_index++)
        {
            if (handle_crossover(deduped_ranges[check_deduped_index], min_max))
            {
                mashed_into_another_value = 1;
                break;
            }
        }

        if (mashed_into_another_value) {
            free(min_max);

            for (int j = i; j < *deduped_ranges_length; j++) {
                deduped_ranges[j] = deduped_ranges[j + 1];
            }

            (*deduped_ranges_length)--;
            deduped_ranges = realloc(deduped_ranges, sizeof(double*) * *deduped_ranges_length);
        }
    }

    return deduped_ranges;
}

int get_fresh_ingredients_v2(char **fresh_ingredients_range, int fresh_ingredients_length)
{
    int deduped_ranges_length = 0;
    double **deduped_ranges = malloc(sizeof(double*));

    printf("Looping through %d ingreds\n", fresh_ingredients_length);
    for (int i = 0; i < fresh_ingredients_length; i++)
    {
        printf("check fresh ingred %d (of %d) %s - %s\n", i, fresh_ingredients_length, fresh_ingredients_range[i], fresh_ingredients_range[4]);
        double* min_max = get_min_max(fresh_ingredients_range[i]);

        printf("Checking min max %f %f\n", min_max[0], min_max[1]);

        // loop through to check.
        int mashed_into_another_value = 0;
        for (int check_deduped_index = 0; check_deduped_index < deduped_ranges_length; check_deduped_index++)
        {
            if (handle_crossover(deduped_ranges[check_deduped_index], min_max))
            {
                mashed_into_another_value = 1;
                break;
            }
        }

        if (!mashed_into_another_value) {
            // Add new value to deduped ranges.
            deduped_ranges_length++;
            deduped_ranges = realloc(deduped_ranges, sizeof(double*) * deduped_ranges_length);
            deduped_ranges[deduped_ranges_length - 1] = min_max;
        } else {
            free(min_max);
            printf("\nDEDUPE\n");
            print_double_grid(deduped_ranges, deduped_ranges_length);
            deduped_ranges = dedupe_ranges(deduped_ranges, &deduped_ranges_length);
            print_double_grid(deduped_ranges, deduped_ranges_length);
        }
    }

    double total_fresh_ingredients = 0.0;

    for (int i = 0; i < deduped_ranges_length; i++) {
        double* range = deduped_ranges[i];

        total_fresh_ingredients += range[1] - range[0] + 1;

        free(range);
    }

    free(deduped_ranges);

    return total_fresh_ingredients;
}
