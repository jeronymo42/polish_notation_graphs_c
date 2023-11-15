#include "expression_process.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void push_all_by_priority(struct stack** top, struct vector* expression, int cur_priority) {
    while (!stack_is_empty(*top) && vector_is_valid(*expression) &&
           stack_top(*top).priority >= cur_priority) {
        vector_push_back(expression, stack_top(*top));
        stack_pop(top);
    }
}

void push_all(struct stack** top, struct vector* expression) {
    while (!stack_is_empty(*top) && vector_is_valid(*expression)) {
        vector_push_back(expression, stack_top(*top));
        stack_pop(top);
    }
}

double calculate_expression(struct vector expression, double x) {
    struct stack_d* var_stack = stack_d_init();

    if (var_stack == NULL) return NAN;

    for (int i = 0; i < expression.size; ++i) {
        int status =
            vector_get_elem(&expression, i).calculate(vector_get_elem(&expression, i).value, &var_stack, x);
        if (!status) {
            stack_d_destroy(var_stack);
            return NAN;
        }
    }
    double res = NAN;
    if (!stack_d_is_empty(var_stack)) res = stack_d_top(var_stack);
    stack_d_destroy(var_stack);
    return res;
}

struct vector expression_to_postfix(char* str) {
    struct vector res = vector_init();
    if (!vector_is_valid(res)) return res;

    struct stack* oper_stack = stack_init();
    if (oper_stack == NULL) {
        vector_destroy(&res);
        return res;
    }

    int has_left_operand = 0;

    while (*str != '\0') {
        struct expr_item item;
        str = get_lexem(str, &item, &has_left_operand);
        if (item.type == UNDEF) {
            vector_destroy(&res);
            stack_destroy(oper_stack);
            return res;
        }
        if (item.type == VARIABLE || item.type == CONSTANT) {
            vector_push_back(&res, item);
            if (!vector_is_valid(res)) {
                stack_destroy(oper_stack);
                return res;
            }
        } else if (item.type == CLOSE_B) {
            push_all_by_priority(&oper_stack, &res, item.priority);
            if (!stack_is_empty(oper_stack) && vector_is_valid(res) && stack_top(oper_stack).type == OPEN_B) {
                stack_pop(&oper_stack);
            } else {
                if (vector_is_valid(res)) vector_destroy(&res);
                stack_destroy(oper_stack);
                return res;
            }
        } else if (item.type == OPEN_B) {
            stack_push(&oper_stack, item);
        } else {
            push_all_by_priority(&oper_stack, &res, item.priority);
            stack_push(&oper_stack, item);
            if (!vector_is_valid(res)) {
                stack_destroy(oper_stack);
                return res;
            }
        }
    }

    push_all(&oper_stack, &res);
    stack_destroy(oper_stack);
    return res;
}

char* get_lexem(char* str, struct expr_item* answer, int* has_left_operand) {
    answer->value = 0;
    if (strncmp("sin", str, 3) == 0 && !*has_left_operand) {
        answer->type = SIN;
        answer->priority = 3;
        answer->calculate = &calc_sin;
        *has_left_operand = 0;
        str += 3;
    } else if (strncmp("cos", str, 3) == 0 && !*has_left_operand) {
        answer->type = COS;
        answer->priority = 3;
        answer->calculate = &calc_cos;
        *has_left_operand = 0;
        str += 3;
    } else if (strncmp("tan", str, 3) == 0 && !*has_left_operand) {
        answer->type = TAN;
        answer->priority = 3;
        answer->calculate = &calc_tan;
        *has_left_operand = 0;
        str += 3;
    } else if (strncmp("ctg", str, 3) == 0 && !*has_left_operand) {
        answer->type = CTG;
        answer->priority = 3;
        answer->calculate = &calc_ctg;
        *has_left_operand = 0;
        str += 3;
    } else if (strncmp("sqrt", str, 4) == 0 && !*has_left_operand) {
        answer->type = SQRT;
        answer->priority = 3;
        answer->calculate = &calc_sqrt;
        *has_left_operand = 0;
        str += 4;
    } else if (strncmp("ln", str, 2) == 0 && !*has_left_operand) {
        answer->type = LN;
        answer->priority = 3;
        answer->calculate = &calc_ln;
        *has_left_operand = 0;
        str += 2;
    } else if (strncmp("-", str, 1) == 0 && !*has_left_operand) {
        answer->type = UN_MINUS;
        answer->priority = 3;
        answer->calculate = &calc_un_minus;
        *has_left_operand = 0;
        str += 1;
    } else if (strncmp("-", str, 1) == 0 && *has_left_operand) {
        answer->type = BIN_MINUS;
        answer->priority = 1;
        answer->calculate = &calc_bin_minus;
        *has_left_operand = 0;
        str += 1;
    } else if (strncmp("+", str, 1) == 0 && *has_left_operand) {
        answer->type = PLUS;
        answer->priority = 1;
        answer->calculate = &calc_plus;
        *has_left_operand = 0;
        str += 1;
    } else if (strncmp("*", str, 1) == 0 && *has_left_operand) {
        answer->type = MULTIPLY;
        answer->priority = 2;
        answer->calculate = &calc_multiply;
        *has_left_operand = 0;
        str += 1;
    } else if (strncmp("/", str, 1) == 0 && *has_left_operand) {
        answer->type = DIVIDE;
        answer->priority = 2;
        answer->calculate = &calc_divide;
        *has_left_operand = 0;
        str += 1;
    } else if (strncmp("(", str, 1) == 0 && !*has_left_operand) {
        answer->type = OPEN_B;
        answer->priority = -1;
        *has_left_operand = 0;
        str += 1;
    } else if (strncmp(")", str, 1) == 0 && *has_left_operand) {
        answer->type = CLOSE_B;
        answer->priority = 0;
        *has_left_operand = 1;
        str += 1;
    } else if (strncmp("x", str, 1) == 0 && !*has_left_operand) {
        answer->type = VARIABLE;
        answer->priority = 4;
        answer->calculate = &calc_var;
        *has_left_operand = 1;
        str += 1;
    } else {
        char* end;
        double val = strtod(str, &end);
        if (str != end && !*has_left_operand) {
            answer->type = CONSTANT;
            answer->priority = 4;
            answer->calculate = &calc_const;
            answer->value = val;
            *has_left_operand = 1;
            str = end;
        } else {
            answer->type = UNDEF;
        }
    }
    return str;
}

int calc_sin(double value, struct stack_d** top, double x) {
    if (!stack_d_is_empty(*top)) {
        double val = stack_d_top(*top);
        stack_d_pop(top);
        val = sin(val) + 0 * x + 0 * value;
        stack_d_push(top, val);
        return (*top)->data == val;
    }
    return 0;
}

int calc_cos(double value, struct stack_d** top, double x) {
    if (!stack_d_is_empty(*top)) {
        double val = stack_d_top(*top);
        stack_d_pop(top);
        val = cos(val) + 0 * x + 0 * value;
        stack_d_push(top, val);
        return (*top)->data == val;
    }
    return 0;
}

int calc_tan(double value, struct stack_d** top, double x) {
    if (!stack_d_is_empty(*top)) {
        double val = stack_d_top(*top);
        stack_d_pop(top);
        val = tan(val) + 0 * x + 0 * value;
        stack_d_push(top, val);
        return (*top)->data == val;
    }
    return 0;
}

int calc_ctg(double value, struct stack_d** top, double x) {
    if (!stack_d_is_empty(*top)) {
        double val = stack_d_top(*top);
        stack_d_pop(top);
        val = cos(val) / sin(val) + 0 * x + 0 * value;
        stack_d_push(top, val);
        return (*top)->data == val;
    }
    return 0;
}

int calc_sqrt(double value, struct stack_d** top, double x) {
    if (!stack_d_is_empty(*top)) {
        double val = stack_d_top(*top);
        stack_d_pop(top);
        val = sqrt(val) + 0 * x + 0 * value;
        stack_d_push(top, val);
        return (*top)->data == val;
    }
    return 0;
}

int calc_ln(double value, struct stack_d** top, double x) {
    if (!stack_d_is_empty(*top)) {
        double val = stack_d_top(*top);
        stack_d_pop(top);
        val = log(val) + 0 * x + 0 * value;
        stack_d_push(top, val);
        return (*top)->data == val;
    }
    return 0;
}

int calc_plus(double value, struct stack_d** top, double x) {
    if (!stack_d_is_empty((*top)->down)) {
        double rhs = stack_d_top(*top);
        stack_d_pop(top);
        double lhs = stack_d_top(*top);
        stack_d_pop(top);
        lhs += rhs + 0 * value + 0 * x;
        stack_d_push(top, lhs);
        return (*top)->data == lhs;
    }
    return 0;
}

int calc_un_minus(double value, struct stack_d** top, double x) {
    if (!stack_d_is_empty(*top)) {
        double val = stack_d_top(*top);
        stack_d_pop(top);
        val = -1 * val + 0 * x + 0 * value;
        stack_d_push(top, val);
        return (*top)->data == val;
    }
    return 0;
}

int calc_bin_minus(double value, struct stack_d** top, double x) {
    if (!stack_d_is_empty((*top)->down)) {
        double rhs = stack_d_top(*top);
        stack_d_pop(top);
        double lhs = stack_d_top(*top);
        stack_d_pop(top);
        lhs -= rhs + 0 * value + 0 * x;
        stack_d_push(top, lhs);
        return (*top)->data == lhs;
    }
    return 0;
}

int calc_multiply(double value, struct stack_d** top, double x) {
    if (!stack_d_is_empty((*top)->down)) {
        double rhs = stack_d_top(*top);
        stack_d_pop(top);
        double lhs = stack_d_top(*top);
        stack_d_pop(top);
        lhs *= rhs + 0 * value + 0 * x;
        stack_d_push(top, lhs);
        return (*top)->data == lhs;
    }
    return 0;
}

int calc_divide(double value, struct stack_d** top, double x) {
    if (!stack_d_is_empty((*top)->down)) {
        double rhs = stack_d_top(*top);
        stack_d_pop(top);
        double lhs = stack_d_top(*top);
        stack_d_pop(top);
        lhs /= rhs + 0 * value + 0 * x;
        stack_d_push(top, lhs);
        return (*top)->data == lhs;
    }
    return 0;
}

int calc_var(double value, struct stack_d** top, double x) {
    stack_d_push(top, x + 0 * value);
    return (*top)->data == x;
}

int calc_const(double value, struct stack_d** top, double x) {
    stack_d_push(top, value + 0 * x);
    return (*top)->data == value;
}