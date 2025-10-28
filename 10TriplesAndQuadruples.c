#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

int tempCount = 1;

int prec(char op)
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

// Convert infix to postfix
int infixToPostfix(char *exp, char postfix[][MAX])
{
    char stack[MAX];
    int top = -1, k = 0;

    for (int i = 0; exp[i]; i++)
    {
        char c = exp[i];
        if (isspace(c))
            continue;

        if (isalnum(c))
        {
            char t[2] = {c, '\0'};
            strcpy(postfix[k++], t);
        }
        else if (c == '(')
            stack[++top] = c;
        else if (c == ')')
        {
            while (top != -1 && stack[top] != '(')
            {
                char t[2] = {stack[top--], '\0'};
                strcpy(postfix[k++], t);
            }
            top--;
        }
        else if (strchr("+-*/^", c))
        {
            while (top != -1 && stack[top] != '(' && prec(stack[top]) >= prec(c))
            {
                char t[2] = {stack[top--], '\0'};
                strcpy(postfix[k++], t);
            }
            stack[++top] = c;
        }
    }

    while (top != -1)
    {
        char t[2] = {stack[top--], '\0'};
        strcpy(postfix[k++], t);
    }
    return k;
}

// Generate Three Address Code + Triples + Quadruples
void generateTAC(char postfix[][MAX], int n)
{
    char stack[MAX][MAX], temp[10];
    int top = -1;

    // Arrays for TAC
    char opList[MAX], arg1[MAX][MAX], arg2[MAX][MAX], res[MAX][MAX];

    // Arrays for triples/quadruples (using (0), (1), etc.)
    char tArg1[MAX][MAX], tArg2[MAX][MAX];
    int instrCount = 0;

    printf("\nThree Address Code:\n");

    for (int i = 0; i < n; i++)
    {
        char c = postfix[i][0];
        if (isalnum(c))
        {
            strcpy(stack[++top], postfix[i]);
        }
        else if (strchr("+-*/^", c))
        {
            char op2[MAX], op1[MAX];
            strcpy(op2, stack[top--]);
            strcpy(op1, stack[top--]);

            sprintf(temp, "t%d", tempCount++);
            printf("%s = %s %c %s\n", temp, op1, c, op2);

            // Store TAC
            opList[instrCount] = c;
            strcpy(arg1[instrCount], op1);
            strcpy(arg2[instrCount], op2);
            strcpy(res[instrCount], temp);

            // For Triples/Quadruples, replace t# with (index)
            // Example: if op1 = t1 → use (0)
            if (op1[0] == 't')
                sprintf(tArg1[instrCount], "(%d)", atoi(op1 + 1) - 1);
            else
                strcpy(tArg1[instrCount], op1);

            if (op2[0] == 't')
                sprintf(tArg2[instrCount], "(%d)", atoi(op2 + 1) - 1);
            else
                strcpy(tArg2[instrCount], op2);

            strcpy(stack[++top], temp);
            instrCount++;
        }
    }

    // Print Triples
    printf("\nTriples:\n");
    printf("Index\tOp\tArg1\tArg2\n");
    for (int i = 0; i < instrCount; i++)
    {
        printf("%d\t%c\t%s\t%s\n", i, opList[i], tArg1[i], tArg2[i]);
    }

    // Print Quadruples
    printf("\nQuadruples:\n");
    printf("Index\tOp\tArg1\tArg2\tResult\n");
    for (int i = 0; i < instrCount; i++)
    {
        printf("%d\t%c\t%s\t%s\t%s\n", i, opList[i], tArg1[i], tArg2[i], res[i]);
    }
}

int main()
{
    char expr[MAX], postfix[MAX][MAX];
    printf("Enter expression (e.g., a+b*c^d): ");
    scanf("%s", expr);

    int count = infixToPostfix(expr, postfix);
    generateTAC(postfix, count);
    return 0;
}
