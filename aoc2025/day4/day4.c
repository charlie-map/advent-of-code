#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../../utils/fileread.h"
#include "../../utils/split_string.h"

int **basic_grid(char **rows, int row_length, int *col_length);
void print_basic_grid(int **grid, int row_length, int *col_length);

int movable_rolls(int **grid, int grid_rows, int *grid_cols);

int how_many_rolls_can_we_remove(int **grid, int **coppier_grid, int grid_rows, int *grid_cols);

int main()
{
    int input_length = 0;
    char *input = fileread("input.txt", &input_length);

    int split_inputs_on_newline_length;
    int *each_newline_length;

    char **split_inputs_on_newline = split_string(input, strlen(input), "\n", &split_inputs_on_newline_length, &each_newline_length);

    int **grid = basic_grid(split_inputs_on_newline, split_inputs_on_newline_length, each_newline_length);

    print_basic_grid(grid, split_inputs_on_newline_length, each_newline_length);

    // printf("Answer P1: %d\n", movable_rolls(grid, split_inputs_on_newline_length, each_newline_length));

    int **extra_grid = basic_grid(split_inputs_on_newline, split_inputs_on_newline_length, each_newline_length);

    printf("Answer P2: %d\n", how_many_rolls_can_we_remove(grid, extra_grid, split_inputs_on_newline_length, each_newline_length));

    free(split_inputs_on_newline);
    free(each_newline_length);
    free(input);

    return 0;
}

int **basic_grid(char **rows, int row_length, int *col_length)
{
    int **grid = malloc(sizeof(int *) * row_length);

    for (int row_index = 0; row_index < row_length; row_index++)
    {
        int *new_row = malloc(sizeof(int) * col_length[row_index]);

        for (int col_index = 0; col_index < col_length[row_index]; col_index++)
        {
            new_row[col_index] = rows[row_index][col_index] == '@' ? 1 : 0;
        }

        grid[row_index] = new_row;
    }

    return grid;
}

void print_basic_grid(int **grid, int row_length, int *col_length)
{
    for (int y = 0; y < row_length; y++)
    {
        for (int x = 0; x < col_length[y]; x++)
        {
            printf("%c", grid[y][x] ? '@' : '.');
        }

        printf("\n");
    }

    printf("\n");
}

int amount_of_surrounding_tp(int **grid, int y, int grid_rows, int x, int *grid_cols)
{
    int surrounding_tp = 0;

    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
            // Ignore current position.
            if (i == 0 && j == 0)
            {
                continue;
            }

            // Check for boundaries.
            if (y + i < 0 || y + i >= grid_rows)
            {
                continue;
            }

            if (x + j < 0 || x + j >= grid_cols[y + i])
            {
                continue;
            }

            surrounding_tp += grid[y + i][x + j];
        }
    }

    return surrounding_tp;
}

int movable_rolls(int **grid, int grid_rows, int *grid_cols)
{
    int available_rolls = 0;

    // Loop through each cell and check the surrounding 8.
    for (int y = 0; y < grid_rows; y++)
    {
        for (int x = 0; x < grid_cols[y]; x++)
        {
            // Make sure there's a roll at this spot. If not, skip.
            if (grid[y][x] != 1)
            {
                continue;
            }

            int surrounding_tp = amount_of_surrounding_tp(grid, y, grid_rows, x, grid_cols);

            if (surrounding_tp < 4)
            {
                available_rolls++;
            }
        }
    }

    return available_rolls;
}

void remove_movable_rolls(int **grid, int **updated_grid, int grid_rows, int *grid_cols)
{
    // Loop through each cell and check the surrounding 8.
    for (int y = 0; y < grid_rows; y++)
    {
        for (int x = 0; x < grid_cols[y]; x++)
        {
            // Make sure there's a roll at this spot. If not, skip.
            if (grid[y][x] != 1)
            {
                continue;
            }

            int surrounding_tp = amount_of_surrounding_tp(grid, y, grid_rows, x, grid_cols);

            if (surrounding_tp < 4)
            {
                updated_grid[y][x] = 0;
            }
        }
    }

    // Loop through and update og grid.
    for (int y = 0; y < grid_rows; y++)
    {
        for (int x = 0; x < grid_cols[y]; x++)
        {
            grid[y][x] = updated_grid[y][x];
        }
    }

    return;
}

int how_many_rolls_can_we_remove(int **grid, int **coppier_grid, int grid_rows, int *grid_cols)
{
    int number_of_movable_rolls = 0;

    while (movable_rolls(grid, grid_rows, grid_cols))
    {
        printf("\n\nROUND\n");

        number_of_movable_rolls += movable_rolls(grid, grid_rows, grid_cols);
        printf("update moved, now what do we have? %d\n", number_of_movable_rolls);

        remove_movable_rolls(grid, coppier_grid, grid_rows, grid_cols);

        print_basic_grid(grid, grid_rows, grid_cols);
    }

    return number_of_movable_rolls;
}
