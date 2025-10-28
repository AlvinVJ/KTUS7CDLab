#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct variable {
    char var;
    char expression[100];
    int isConstant;
    int value;
} variables[26];

int numVariables;

int power(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; i++)
        result *= base;
    return result;
}

int isConstant(char c) {
    for (int i = 0; i < numVariables; i++)
        if (variables[i].var == c)
            return variables[i].isConstant;
    return 0;
}

int getValue(char c) {
    for (int i = 0; i < numVariables; i++)
        if (variables[i].var == c)
            return variables[i].value;
    return 0;
}

void replaceVarWithValue(char *expr, char var, int value) {
    char buf[200];
    int j = 0;
    for (int i = 0; expr[i]; i++) {
        if (expr[i] == var) {
            j += sprintf(&buf[j], "%d", value);
        } else {
            buf[j++] = expr[i];
        }
    }
    buf[j] = '\0';
    strcpy(expr, buf);
}

int canEvaluate(char *expr) {
    for (int i = 0; expr[i]; i++)
        if (isalpha(expr[i]))
            return 0;
    return 1;
}

int evaluateExpression(char *expr) {
    char temp[200];
    strcpy(temp, expr);
    char *tokens[100];
    int count = 0;

    char *token = strtok(temp, " ");
    while (token) {
        tokens[count++] = token;
        token = strtok(NULL, " ");
    }

    // Handle ^ (right associative)
    for (int i = count - 2; i > 0; i -= 2) {
        if (strcmp(tokens[i], "^") == 0) {
            int left = atoi(tokens[i - 1]);
            int right = atoi(tokens[i + 1]);
            int res = power(left, right);
            sprintf(tokens[i - 1], "%d", res);
            for (int j = i; j < count - 2; j++)
                tokens[j] = tokens[j + 2];
            count -= 2;
            i = count;
        }
    }

    // * and /
    for (int i = 1; i < count - 1; i += 2) {
        if (strcmp(tokens[i], "*") == 0 || strcmp(tokens[i], "/") == 0) {
            int left = atoi(tokens[i - 1]);
            int right = atoi(tokens[i + 1]);
            int res = (strcmp(tokens[i], "*") == 0) ? left * right : left / right;
            sprintf(tokens[i - 1], "%d", res);
            for (int j = i; j < count - 2; j++)
                tokens[j] = tokens[j + 2];
            count -= 2;
            i -= 2;
        }
    }

    // + and -
    for (int i = 1; i < count - 1; i += 2) {
        if (strcmp(tokens[i], "+") == 0 || strcmp(tokens[i], "-") == 0) {
            int left = atoi(tokens[i - 1]);
            int right = atoi(tokens[i + 1]);
            int res = (strcmp(tokens[i], "+") == 0) ? left + right : left - right;
            sprintf(tokens[i - 1], "%d", res);
            for (int j = i; j < count - 2; j++)
                tokens[j] = tokens[j + 2];
            count -= 2;
            i -= 2;
        }
    }

    return atoi(tokens[0]);
}

void printExpressions() {
    for (int i = 0; i < numVariables; i++) {
        if (variables[i].isConstant)
            printf("%c = %d\n", variables[i].var, variables[i].value);
        else
            printf("%c = %s\n", variables[i].var, variables[i].expression);
    }
    printf("----------------------\n");
}

int main() {
    printf("Enter number of variables: ");
    scanf("%d", &numVariables);
    getchar(); // consume newline

    for (int i = 0; i < numVariables; i++) {
        printf("Enter expression (e.g., a = 3 + 4 * 2): ");
        scanf(" %c =", &variables[i].var);       // read variable name (like 'a')
        getchar();                               // consume space after '='
        fgets(variables[i].expression, sizeof(variables[i].expression), stdin);
        variables[i].expression[strcspn(variables[i].expression, "\n")] = 0; // remove newline

        variables[i].isConstant = 0;
        variables[i].value = 0;

        if (canEvaluate(variables[i].expression)) {
            variables[i].value = evaluateExpression(variables[i].expression);
            variables[i].isConstant = 1;
        }
    }

    int updated = 1;
    while (updated) {
        updated = 0;
        for (int i = 0; i < numVariables; i++) {
            if (!variables[i].isConstant) {
                for (int j = 0; variables[i].expression[j]; j++) {
                    if (isalpha(variables[i].expression[j]) && isConstant(variables[i].expression[j])) {
                        replaceVarWithValue(variables[i].expression, variables[i].expression[j],
                                            getValue(variables[i].expression[j]));
                        updated = 1;
                    }
                }
                if (canEvaluate(variables[i].expression)) {
                    variables[i].value = evaluateExpression(variables[i].expression);
                    variables[i].isConstant = 1;
                    updated = 1;
                }
            }
        }
    }

    printf("\nOptimized after Constant Propagation:\n");
    printExpressions();
    return 0;
}
