#ifndef EXPRESSION_PROCESS_H
#define EXPRESSION_PROCESS_H

#include "structs.h"

char* get_lexem(char* str, struct expr_item* answer, int* has_left_operand);

struct vector expression_to_postfix(
    char* str);  //если введенное выражение некорректно, вернет невалидный вектор
// Есть функция vector_is_valid, которая это и проверяет
// Также может быть невалидным если закончилась память, но разницы нет, мы работать не можем, выводи просто,
// что произошла ошибка

double calculate_expression(struct vector expression, double x);

int calc_sin(double value, struct stack_d** top, double x);
int calc_cos(double value, struct stack_d** top, double x);
int calc_tan(double value, struct stack_d** top, double x);
int calc_ctg(double value, struct stack_d** top, double x);
int calc_sqrt(double value, struct stack_d** top, double x);
int calc_ln(double value, struct stack_d** top, double x);
int calc_plus(double value, struct stack_d** top, double x);
int calc_un_minus(double value, struct stack_d** top, double x);
int calc_bin_minus(double value, struct stack_d** top, double x);
int calc_multiply(double value, struct stack_d** top, double x);
int calc_divide(double value, struct stack_d** top, double x);
int calc_var(double value, struct stack_d** top, double x);
int calc_const(double value, struct stack_d** top, double x);

#endif