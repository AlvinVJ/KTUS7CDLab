#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

int tempCount = 1;

// Get operator precedence
int prec(char op)
{
    switch (op)
    {
    case '^':
        return 3;
    case '*':
    case '/':
        return 2;
    case '+':
    case '-':
        return 1;
    default:
        return 0;
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
        { // Operand
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
            top--; // pop '('
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

// Generate Three Address Code
void generateTAC(char postfix[][MAX], int n)
{
    char stack[MAX][MAX], temp[5];
    int top = -1;

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
            strcpy(stack[++top], temp);
        }
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