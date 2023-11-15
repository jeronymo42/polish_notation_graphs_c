
#include <stdio.h>

#include "expression_process.h"

#define PI 3.14159265359

#define X 80
#define Y 25

void fill_screen(char screen[X][Y], struct vector expression);
void print_screen(char screen[X][Y]);
int clamp(int val, int down, int up);

int main() {
    char expr_string[400];

    scanf("%399s", expr_string);

    struct vector expr = expression_to_postfix(expr_string);

    if (vector_is_valid(expr)) {
        char screen[X][Y];
        fill_screen(screen, expr);
        print_screen(screen);
        vector_destroy(&expr);
    } else {
        printf("Some error occured. Maybe expression is incorrect or not enough memory\n");
    }
    return 0;
}

void fill_screen(char screen[X][Y], struct vector expression) {
    for (int j = 0; j < Y; ++j)
        for (int i = 0; i < X; ++i) screen[i][j] = '.';

    for (int i = 0; i < X; ++i) {
        double x = (((double)i / X) + ((double)1 / (2 * X))) * 4 * PI;
        double y = calculate_expression(expression, x);
        if (y >= -1 && y <= 1) {
            screen[i][clamp(((y + 1) / 2 * Y), 0, Y - 1)] = '*';
        }
    }
}

void print_screen(char screen[X][Y]) {
    for (int j = 0; j < Y; ++j) {
        for (int i = 0; i < X; ++i) putchar(screen[i][j]);
        putchar('\n');
    }
}

int clamp(int val, int down, int up) {
    if (val > up)
        return up;
    else if (up < down)
        return down;
    else
        return val;
}