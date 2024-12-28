#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sum(char *input, int line_length);
int sum2(char *input, int line_length);

int main()
{
    int line_length = 0;
    char *word_input = malloc(sizeof(char) * line_length);
    FILE *fp;

    fp = fopen("input2.txt", "r");

    if (fp == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    size_t line_size = 0;
    ssize_t read;
    char *line = NULL;
    while ((read = getline(&line, &line_size, fp)) != -1)
    {
        line_length += strlen(line) - 1;

        word_input = realloc(word_input, sizeof(char *) * line_length);
        strcat(word_input, line);
        word_input[line_length] = '\0';
    }

    printf("Number for mul: %d\n", sum2(word_input, line_length));

    free(word_input);
    free(line);

    fclose(fp);

    return 0;
}

/**
 * Strat:
 *
 * - Search for `mul`;
 * - Collect characters until next mul;
 *
 * All the sauce between will be used!
 */
char **break_by_mul(char *input, int line_length, int *collector_length, int **inner_collector_length)
{
    int match_length = 0;
    char match[4] = "mul";
    char *token = malloc(sizeof(char) * 4);
    // Make clear.
    memset(token, 0, 4);

    // Collector
    *collector_length = 1;
    *inner_collector_length = malloc(sizeof(int) * *collector_length);
    (*inner_collector_length)[0] = 0;
    char **collector = malloc(sizeof(char *) * *collector_length);

    for (int c = 0; c < line_length; c++)
    {
        if (input[c] == match[match_length])
        {
            token[match_length] = input[c];
            match_length++;

            if (match_length == 3)
            {
                // Hit the end!
                memset(token, 0, 4);
                match_length = 0;

                *collector_length += 1;
                *inner_collector_length = realloc(*inner_collector_length, sizeof(int) * *collector_length);
                (*inner_collector_length)[*collector_length - 1] = 0;
                collector = realloc(collector, (sizeof(char *) * *collector_length));
            }

            continue;
        }
        else if (match_length > 0)
        {
            int new_collector_lengths = (*inner_collector_length)[*collector_length - 1] + match_length - 1;
            collector[*collector_length - 1] = realloc(collector[*collector_length - 1], sizeof(char) * new_collector_lengths);

            strcpy(collector[*collector_length - 1] + (*inner_collector_length)[*collector_length - 1], token);

            (*inner_collector_length)[*collector_length - 1] = new_collector_lengths;

            memset(token, 0, 4);
            match_length = 0;

            continue;
        }

        // Add character to end. I don't care that's it's not effective
        int new_collector_length = (*inner_collector_length)[*collector_length - 1] + 1;
        collector[*collector_length - 1] = realloc(collector[*collector_length - 1], sizeof(char) * new_collector_length);

        collector[*collector_length - 1][(*inner_collector_length)[*collector_length - 1]] = input[c];

        (*inner_collector_length)[*collector_length - 1] = new_collector_length;
    }

    return collector;
}

/**
 * Strat:
 *
 * - Search through remainder for dos or donts, set should multi occordingly
 * WARNING: Do not try this at home.
 *
 * Return yes or no depending on ending..
 */
int should_multi_check(char *search_token, int multi)
{
    while (search_token && *search_token)
    {
        if (
            *search_token == 'd' &&
            *(search_token + sizeof(char)) == 'o' &&
            *(search_token + 2 * sizeof(char)) == '(' &&
            *(search_token + 3 * sizeof(char)) == ')')
        {
            multi = 1;
            search_token += 4 * sizeof(char);
            continue;
        }

        if (
            *search_token == 'd' &&
            *(search_token + sizeof(char)) == 'o' &&
            *(search_token + 2 * sizeof(char)) == 'n' &&
            *(search_token + 3 * sizeof(char)) == '\'' &&
            *(search_token + 4 * sizeof(char)) == 't' &&
            *(search_token + 5 * sizeof(char)) == '(' &&
            *(search_token + 6 * sizeof(char)) == ')')
        {
            multi = 0;
            search_token += 7 * sizeof(char);
            continue;
        }

        search_token += sizeof(char);
    }

    return multi;
}

/**
 * Strat:
 *
 * - First character MUST be a `(`, otherwise place a null in;
 * - Search for `,` 3 or less away from `(`;
 * - Search for `)` 3 or less away from `,`;
 *
 * Return the bits we find in there if we find anything..
 */
int *find_matching_number(char *token, int inner_token_length, int *should_multi)
{
    char *match = malloc(sizeof(char) * inner_token_length);

    if (inner_token_length < 5 || token[0] != '(')
    {
        *should_multi = should_multi_check(token, *should_multi);
        return NULL;
    }

    char *token_search = token + sizeof(char);
    char *first_number = malloc(sizeof(char) * 4);
    int first_number_index = 0;
    memset(first_number, 0, 4);
    while (*token_search != ',' && *token_search)
    {
        if (first_number_index > 4)
        {
            *should_multi = should_multi_check(token, *should_multi);
            return NULL;
        }

        if (*token_search < 48 || *token_search > 57)
        {
            *should_multi = should_multi_check(token, *should_multi);
            return NULL;
        }

        first_number[first_number_index] = *token_search;
        first_number_index++;
        token_search += sizeof(char);
    }

    if (*token_search != ',')
    {
        *should_multi = should_multi_check(token, *should_multi);
        return NULL;
    }

    int first_number_int = atoi(first_number);
    free(first_number);

    token_search = token_search + sizeof(char);
    char *second_number = malloc(sizeof(char) * 4);
    int second_number_index = 0;
    memset(second_number, 0, 4);
    while (*token_search != ')' && *token_search)
    {
        if (second_number_index > 4)
        {
            *should_multi = should_multi_check(token, *should_multi);
            return NULL;
        }

        if (*token_search < 48 || *token_search > 57)
        {
            *should_multi = should_multi_check(token, *should_multi);
            return NULL;
        }

        second_number[second_number_index] = *token_search;
        second_number_index++;
        token_search += sizeof(char);
    }

    if (*token_search != ')')
    {
        *should_multi = should_multi_check(token, *should_multi);
        return NULL;
    }

    int second_number_int = atoi(second_number);
    free(second_number);

    *should_multi = should_multi_check(token_search, *should_multi);

    int *matching_numbers = malloc(sizeof(int) * 2);
    matching_numbers[0] = first_number_int;
    matching_numbers[1] = second_number_int;
    return matching_numbers;
}

int sum(char *input, int line_length)
{
    int collector_length = 0;
    int **inner_collector_length = malloc(sizeof(int *));
    char **mul_tokens = break_by_mul(input, line_length, &collector_length, inner_collector_length);

    int sum = 0;

    int found = 0;
    for (int find_numbers_index = 0; find_numbers_index < collector_length; find_numbers_index++)
    {
        int *find_numbers = find_matching_number(mul_tokens[find_numbers_index], (*inner_collector_length)[find_numbers_index], NULL);

        if (find_numbers)
        {
            found++;
            sum += find_numbers[0] * find_numbers[1];
            free(find_numbers);
        }
    }

    return sum;
}

int sum2(char *input, int line_length)
{
    int collector_length = 0;
    int **inner_collector_length = malloc(sizeof(int *));
    char **mul_tokens = break_by_mul(input, line_length, &collector_length, inner_collector_length);

    int sum = 0;
    int should_multi = 1;

    int found = 0;
    for (int find_numbers_index = 0; find_numbers_index < collector_length; find_numbers_index++)
    {
        int new_multi = should_multi;
        int *find_numbers = find_matching_number(mul_tokens[find_numbers_index], (*inner_collector_length)[find_numbers_index], &new_multi);

        if (find_numbers)
        {
            found++;
            printf("%s - mul(%d, %d)\n", should_multi ? "----------------------" : "", find_numbers[0], find_numbers[1]);
            sum += should_multi ? find_numbers[0] * find_numbers[1] : 0;
            free(find_numbers);
        }

        should_multi = new_multi;
    }

    return sum;
}
