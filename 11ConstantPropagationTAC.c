#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_STATEMENTS 10
#define MAX_LEN 10

typedef struct {
    char op[2];
    char operand1[MAX_LEN];
    char operand2[MAX_LEN];
    char result[MAX_LEN];
    int isConstant;
    int value;
} Statement;

Statement statements[MAX_STATEMENTS];
int statementCount;

// --- Helper: Check if string is numeric
int isNumber(const char *s) {
    if (!s || !*s) return 0;
    for (int i = 0; s[i]; i++)
        if (!isdigit(s[i])) return 0;
    return 1;
}

// --- Helper: Get constant value if known
int getConstantValue(const char *varName) {
    for (int i = 0; i < statementCount; i++) {
        if (statements[i].isConstant && strcmp(statements[i].result, varName) == 0)
            return statements[i].value;
    }
    return INT_MIN;  // Unknown
}

// --- Helper: Evaluate simple expression if possible
int tryEvaluate(const char *operand1, const char *op, const char *operand2) {
    int val1, val2;
    int hasVal1 = 0, hasVal2 = 0;

    if (isNumber(operand1)) {
        val1 = atoi(operand1);
        hasVal1 = 1;
    } else {
        val1 = getConstantValue(operand1);
        if (val1 != INT_MIN) hasVal1 = 1;
    }

    if (isNumber(operand2)) {
        val2 = atoi(operand2);
        hasVal2 = 1;
    } else {
        val2 = getConstantValue(operand2);
        if (val2 != INT_MIN) hasVal2 = 1;
    }

    if (op[0] == '=' && hasVal1) return val1;
    if (hasVal1 && hasVal2) {
        switch (op[0]) {
            case '+': return val1 + val2;
            case '-': return val1 - val2;
            case '*': return val1 * val2;
            case '/': return (val2 != 0) ? val1 / val2 : INT_MIN;
            default:  return INT_MIN;
        }
    }
    return INT_MIN;  // Not evaluable
}

int main() {
    printf("Enter number of statements: ");
    scanf("%d", &statementCount);

    printf("Enter statements (result = op1 op op2, use '_' for no operator):\n");

    for (int i = 0; i < statementCount; i++) {
        char equals[2];
        scanf("%s %s %s %s %s",
              statements[i].result, equals, statements[i].operand1, statements[i].op, statements[i].operand2);
        if (statements[i].op[0] == '_') statements[i].op[0] = '=';
        statements[i].isConstant = 0;
    }

    printf("\nAfter Constant Propagation:\n");

    for (int i = 0; i < statementCount; i++) {
        int resultVal = tryEvaluate(statements[i].operand1, statements[i].op, statements[i].operand2);
        if (resultVal != INT_MIN) {
            statements[i].isConstant = 1;
            statements[i].value = resultVal;
            printf("%s = %d\n", statements[i].result, resultVal);
        } else {
            printf("%s = %s %s %s\n",
                   statements[i].result, statements[i].operand1, statements[i].op, statements[i].operand2);
        }
    }

    return 0;
}
