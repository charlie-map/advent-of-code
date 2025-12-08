#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../,,/utils/fileread.h"
#include "../../utils/split_string.h"

char *build_directional_input_from_keypad(
    char *keypad_input,
    int keypad_height,
    int keypad_width,
    char keypad[keypad_height][keypad_width],
    int start_y,
    int start_x,
    int priority_direction);

int main()
{
    int input_length = 0;
    char *input = fileread("input.txt", &input_length);

    int split_inputs_on_newline_length;
    int *each_newline_length;

    char **split_inputs_on_newline = split_string(input, strlen(input), "\n", &split_inputs_on_newline_length, &each_newline_length);

    free(each_newline_length);

    for (int code_index = 0; code_index < split_inputs_on_newline_length; code_index++)
    {
        int numpad_height = 4;
        int numpad_width = 3;
        char numpad[4][3] = {
            {'7', '8', '9'},
            {'4', '5', '6'},
            {'1', '2', '3'},
            {'1', '0', 'A'},
        };
        char *robot1_input = build_directional_input_from_keypad(split_inputs_on_newline[code_index], numpad_height, numpad_width, numpad, 3, 2, 1);
        printf("Code %s becomes input %s\n", split_inputs_on_newline[code_index], robot1_input);

        int keypad_height = 2;
        int keypad_width = 3;
        char keypad[2][3] = {
            {'1', '^', 'A'},
            {'<', 'v', '>'},
        };

        char *test_robot1_input = "<A^A>^^AvvvA";
        char *robot2_input = build_directional_input_from_keypad(robot1_input, keypad_height, keypad_width, keypad, 0, 2, 0);
        printf("Code %s vs %s becomes input %s\n", test_robot1_input, robot1_input, robot2_input);

        char *my_input = build_directional_input_from_keypad(robot2_input, keypad_height, keypad_width, keypad, 0, 2, 0);

        printf("my input %s\n\n", my_input);

        free(split_inputs_on_newline[code_index]);
    }

    free(split_inputs_on_newline);
    free(input);

    return 0;
}

int *get_number_position(int height, int width, char keypad[height][width], char number)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (keypad[y][x] == number)
            {
                int *position = malloc(sizeof(int) * 2);
                position[0] = y;
                position[1] = x;
                return position;
            }
        }
    }

    return NULL;
}

/**
 * Strat:
 *
 * [ [ 7,  8, 9 ],
 *   [ 4,  5, 6],
 *   [ 1,  2, 3],
 *   [ -1, 0, A],
 * ]
 *
 * Just do simple navigation. If we start at A (2, 3) and want to get to 7 (0, 0),
 *  Do three up (^^^) and two left (<<) and A.
 * Use helper function get_number_position.
 *
 * Priority direction - 1 if we need to dodge bottom left, 0 if we need to dodge the top left.
 *  Yeah... it's poor.
 */
char *build_directional_input_from_keypad(
    char *keypad_input,
    int keypad_height,
    int keypad_width,
    char keypad[keypad_height][keypad_width],
    int start_y,
    int start_x,
    int priority_direction)
{
    int *position = malloc(sizeof(int) * 2);
    position[0] = start_y;
    position[1] = start_x;

    int directional_result_length = 0;
    char *directional_result = malloc(sizeof(char) * directional_result_length);
    memset(directional_result, 0, directional_result_length);

    while (*keypad_input)
    {
        int *new_position = get_number_position(keypad_height, keypad_width, keypad, *keypad_input);

        printf("(%d, %d) to (%d, %d)\n", position[1], position[0], new_position[1], new_position[0]);
        int up_down_diff = new_position[0] - position[0];
        int left_right_diff = new_position[1] - position[1];

        int new_result_length = directional_result_length + abs(up_down_diff) + abs(left_right_diff);
        directional_result = realloc(directional_result, sizeof(char) * (new_result_length + 1));

        directional_result[new_result_length] = '\0';

        printf("up down %d\n", up_down_diff);
        if (priority_direction && up_down_diff > 0)
        {
            for (int left_right_index = 0; left_right_index < abs(left_right_diff); left_right_index++)
            {
                if (left_right_diff > 0)
                {
                    directional_result[directional_result_length] = '>';
                }
                else
                {
                    directional_result[directional_result_length] = '<';
                }

                directional_result_length++;
            }

            for (int up_down_index = 0; up_down_index < abs(up_down_diff); up_down_index++)
            {
                directional_result[directional_result_length] = 'v';
                directional_result_length++;
            }
        }
        else if (priority_direction)
        {
            for (int up_down_index = 0; up_down_index < abs(up_down_diff); up_down_index++)
            {
                directional_result[directional_result_length] = '^';
                directional_result_length++;
            }

            for (int left_right_index = 0; left_right_index < abs(left_right_diff); left_right_index++)
            {
                if (left_right_diff > 0)
                {
                    directional_result[directional_result_length] = '>';
                }
                else
                {
                    directional_result[directional_result_length] = '<';
                }

                directional_result_length++;
            }
        }

        directional_result[directional_result_length] = 'A';
        directional_result_length++;
        keypad_input += sizeof(char);

        int *old_position = position;
        position = new_position;
        free(old_position);
    }

    return directional_result;
}
