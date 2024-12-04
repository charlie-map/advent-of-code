#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int find_xmas_p1(char **input, int width, int height);
int find_xmas_p2(char **input, int width, int height);

int main()
{
	int total_lines = 0;
	int line_length = 0;
	char **word_input = malloc(sizeof(char *) * total_lines);
	FILE *fp;

	fp = fopen("input.txt", "r");

	if (fp == NULL)
	{
		perror("Error opening file");
		return 1;
	}

	size_t line_size = 0;
	ssize_t read;
	char *line;
	while ((read = getline(&line, &line_size, fp)) != -1)
	{
		line_length = strlen(line);
		total_lines++;

		word_input = realloc(word_input, sizeof(char *) * total_lines);
		word_input[total_lines - 1] = malloc(sizeof(char) * line_length);
		strcat(word_input[total_lines - 1], line);
	}

	printf("Number for XMAS P1: %d\n", find_xmas_p1(word_input, line_length, total_lines));
	printf("Number for XMAS P2: %d\n", find_xmas_p2(word_input, line_length, total_lines));

	for (int y = 0; y < total_lines; y++)
	{
		free(word_input[y]);
	}

	free(word_input);
	free(line);

	return 0;
}

int search_for(char **input, char search_char, int width, int height, int cur_x, int cur_y, int direction_x, int direction_y)
{
	int x_location = cur_x + direction_x;
	int y_location = cur_y + direction_y;

	// Avoid oobs.
	if (x_location < 0 || x_location >= width)
	{
		return 0;
	}

	if (y_location < 0 || y_location >= height)
	{
		return 0;
	}

	return input[y_location][x_location] == search_char;
}

/**
 * Strat:
 *
 * 1. Find an X;
 * 2. Ask for M in any direction;
 * 3. Ask for A in any direction that had an M;
 * 4. Ask for S in any direction that had an A;
 *
 * If that succeeds, add 1 to our counter;
 */
int find_xmas_p1(char **input, int width, int height)
{
	int xmas_counter = 0;

	char mas[4] = "MAS";

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (input[y][x] != 'X')
			{
				continue;
			}

			// Start looping around the X.
			for (int horizontal = -1; horizontal < 2; horizontal++)
			{
				for (int vertical = -1; vertical < 2; vertical++)
				{
					int find_mas = 0;
					for (; find_mas < 3; find_mas++)
					{
						if (!search_for(input, mas[find_mas], width, height, x, y, horizontal * (find_mas + 1), vertical * (find_mas + 1)))
						{
							break;
						}
					}

					xmas_counter += find_mas == 3;
				}
			}
		}
	}

	return xmas_counter;
}

char **get_diagonals(char **input, int width, int height, int cur_x, int cur_y)
{
	// Check diagonal validity
	for (int direction_x = -1; direction_x < 2; direction_x++)
	{
		for (int direction_y = -1; direction_y < 2; direction_y++)
		{
			int x_location = cur_x + direction_x;
			int y_location = cur_y + direction_y;

			// Avoid oobs.
			if (x_location < 0 || x_location >= width)
			{
				return NULL;
			}

			if (y_location < 0 || y_location >= height)
			{
				return NULL;
			}
		}
	}

	char *diagonal_1 = malloc(sizeof(char) * 3);
	char *diagonal_2 = malloc(sizeof(char) * 3);

	diagonal_1[0] = input[cur_y - 1][cur_x - 1];
	diagonal_1[1] = input[cur_y + 1][cur_x + 1];
	diagonal_1[2] = '\0';

	diagonal_2[0] = input[cur_y - 1][cur_x + 1];
	diagonal_2[1] = input[cur_y + 1][cur_x - 1];
	diagonal_2[2] = '\0';

	char **diagonals = malloc(sizeof(char *) * 2);
	diagonals[0] = diagonal_1;
	diagonals[1] = diagonal_2;

	return diagonals;
}

/**
 * Strat:
 *
 * 1. Find an A;
 * 2. Grab diagonals;
 * 3. Check both diagonals contain an S and M;
 *
 * If that succeeds, add 1 to our counter;
 */
int find_xmas_p2(char **input, int width, int height)
{
	int xmas_counter = 0;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (input[y][x] != 'A')
			{
				continue;
			}

			char **diagonals = get_diagonals(input, width, height, x, y);
			if (!diagonals)
			{
				continue;
			}

			xmas_counter += (strstr(diagonals[0], "S") && strstr(diagonals[0], "M")) && (strstr(diagonals[1], "S") && strstr(diagonals[1], "M"));

			free(diagonals[0]);
			free(diagonals[1]);
			free(diagonals);
		}
	}

	return xmas_counter;
}
