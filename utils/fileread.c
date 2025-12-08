#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *fileread(char *file_input, int *length)
{
    char *word_input = malloc(sizeof(char) * *length);
    FILE *fp;

    fp = fopen(file_input, "r");

    if (fp == NULL)
    {
        perror("Error opening file");
        return NULL;
    }

    size_t line_size = 0;
    ssize_t read;
    char *line = NULL;
    while ((read = getline(&line, &line_size, fp)) != -1)
    {
        *length += strlen(line);

        word_input = realloc(word_input, sizeof(char *) * *length);
        strcat(word_input, line);
    }

    return word_input;
}
