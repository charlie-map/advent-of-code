#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **split_string(char *input, int line_length, char *delimiter, int *collector_length, int **inner_collector_length) {
    int match_length = 0;
    int delimiter_length = strlen(delimiter);
    char *token = malloc(sizeof(char) * delimiter_length + 1);
    memset(token, 0, delimiter_length + 1);

    // Collector
    *collector_length = 1;
    *inner_collector_length = malloc(sizeof(int) * *collector_length);
    (*inner_collector_length)[0] = 0;
    char **collector = malloc(sizeof(char *) * *collector_length);

    for (int c = 0; c < line_length; c++)
    {
        if (input[c] == delimiter[match_length])
        {
            token[match_length] = input[c];
            match_length++;

            if (match_length == delimiter_length)
            {
                // Hit the end!
                memset(token, 0, delimiter_length + 1);
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

            memset(token, 0, delimiter_length + 1);
            match_length = 0;

            continue;
        }

        // Add character to end. I don't care that's it's not efficient
        int new_collector_length = (*inner_collector_length)[*collector_length - 1] + 1;
        collector[*collector_length - 1] = realloc(collector[*collector_length - 1], sizeof(char) * (new_collector_length + 1));

        collector[*collector_length - 1][(*inner_collector_length)[*collector_length - 1]] = input[c];
        collector[*collector_length - 1][(*inner_collector_length)[*collector_length - 1] + 1] = '\0';

        (*inner_collector_length)[*collector_length - 1] = new_collector_length;
    }

    return collector;
}
