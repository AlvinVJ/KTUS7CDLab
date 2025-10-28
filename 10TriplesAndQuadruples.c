#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

int tempCounter = 1; // For naming temporary variables t1, t2, ...

// Function to return operator precedence
int precedence(char op)
{
    switch (op)
    {
    case '^': return 3;
    case '*':
    case '/': return 2;
    case '+':
    case '-': return 1;
    default: return 0;
    }
}

// Function to convert infix expression to postfix
int infixToPostfix(char *expression, char postfix[][MAX])
{
    char operatorStack[MAX];
    int top = -1, postfixIndex = 0;

    for (int i = 0; expression[i]; i++)
    {
        char symbol = expression[i];

        if (isspace(symbol))
            continue;

        if (isalnum(symbol)) // Operand
        {
            char token[2] = {symbol, '\0'};
            strcpy(postfix[postfixIndex++], token);
        }
        else if (symbol == '(')
        {
            operatorStack[++top] = symbol;
        }
        else if (symbol == ')')
        {
            while (top != -1 && operatorStack[top] != '(')
            {
                char token[2] = {operatorStack[top--], '\0'};
                strcpy(postfix[postfixIndex++], token);
            }
            top--; // Pop '('
        }
        else if (strchr("+-*/^", symbol)) // Operator
        {
            while (top != -1 && operatorStack[top] != '(' && precedence(operatorStack[top]) >= precedence(symbol))
            {
                char token[2] = {operatorStack[top--], '\0'};
                strcpy(postfix[postfixIndex++], token);
            }
            operatorStack[++top] = symbol;
        }
    }

    // Pop remaining operators
    while (top != -1)
    {
        char token[2] = {operatorStack[top--], '\0'};
        strcpy(postfix[postfixIndex++], token);
    }

    return postfixIndex;
}

// Function to generate Three Address Code (TAC), Triples, and Quadruples
void generateIntermediateCode(char postfix[][MAX], int length)
{
    char operandStack[MAX][MAX];  // Operand stack for TAC generation
    int stackTop = -1;

    // Arrays for TAC
    char operatorList[MAX];
    char operand1[MAX][MAX], operand2[MAX][MAX], result[MAX][MAX];

    // Arrays for Triples/Quadruples (with references like (0), (1), etc.)
    char tripleArg1[MAX][MAX], tripleArg2[MAX][MAX];
    int instructionCount = 0;

    printf("\n=== Three Address Code (TAC) ===\n");

    for (int i = 0; i < length; i++)
    {
        char symbol = postfix[i][0];

        // Push operand to stack
        if (isalnum(symbol))
        {
            strcpy(operandStack[++stackTop], postfix[i]);
        }
        // Operator: pop two operands, generate a temp variable
        else if (strchr("+-*/^", symbol))
        {
            char rightOperand[MAX], leftOperand[MAX];
            strcpy(rightOperand, operandStack[stackTop--]);
            strcpy(leftOperand, operandStack[stackTop--]);

            char tempVar[10];
            sprintf(tempVar, "t%d", tempCounter++);

            // Print TAC
            printf("%s = %s %c %s\n", tempVar, leftOperand, symbol, rightOperand);

            // Store for TAC arrays
            operatorList[instructionCount] = symbol;
            strcpy(operand1[instructionCount], leftOperand);
            strcpy(operand2[instructionCount], rightOperand);
            strcpy(result[instructionCount], tempVar);

            // For Triples/Quadruples, replace t# with (index)
            if (leftOperand[0] == 't')
                sprintf(tripleArg1[instructionCount], "(%d)", atoi(leftOperand + 1) - 1);
            else
                strcpy(tripleArg1[instructionCount], leftOperand);

            if (rightOperand[0] == 't')
                sprintf(tripleArg2[instructionCount], "(%d)", atoi(rightOperand + 1) - 1);
            else
                strcpy(tripleArg2[instructionCount], rightOperand);

            // Push the new temp back to stack
            strcpy(operandStack[++stackTop], tempVar);
            instructionCount++;
        }
    }

    // === Triples ===
    printf("\n=== Triples ===\n");
    printf("Index\tOp\tArg1\tArg2\n");
    for (int i = 0; i < instructionCount; i++)
    {
        printf("%d\t%c\t%s\t%s\n", i, operatorList[i], tripleArg1[i], tripleArg2[i]);
    }

    // === Quadruples ===
    printf("\n=== Quadruples ===\n");
    printf("Index\tOp\tArg1\tArg2\tResult\n");
    for (int i = 0; i < instructionCount; i++)
    {
        printf("%d\t%c\t%s\t%s\t%s\n", i, operatorList[i], tripleArg1[i], tripleArg2[i], result[i]);
    }
}

int main()
{
    char expression[MAX], postfix[MAX][MAX];

    printf("Enter arithmetic expression (e.g., a+b*c^d): ");
    scanf("%s", expression);

    int tokenCount = infixToPostfix(expression, postfix);
    generateIntermediateCode(postfix, tokenCount);

    return 0;
}
