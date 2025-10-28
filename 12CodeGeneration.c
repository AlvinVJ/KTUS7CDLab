#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 20

// Structure to hold a TAC statement: lhs = op1 op op2
struct Statement
{
    char lhs[MAX], op1[MAX], op2[MAX];
    char op;
} stmt[MAX];

int main()
{
    int n;

    printf("Enter number of statements: ");
    scanf("%d", &n);
    getchar(); // consume newline left in buffer

    printf("\nEnter TAC statements (e.g., x = a + b or x = y):\n");

    for (int i = 0; i < n; i++)
    {
        char line[100];
        printf("\nStatement %d: ", i + 1);
        fgets(line, sizeof(line), stdin);

        // remove trailing newline
        line[strcspn(line, "\n")] = '\0';

        // parse carefully, allowing spaces
        int fields = sscanf(line, "%s = %s %c %s",
                            stmt[i].lhs, stmt[i].op1, &stmt[i].op, stmt[i].op2);

        // Handle simple assignment (x = a)
        if (fields == 2)
        {
            stmt[i].op = '=';
            strcpy(stmt[i].op2, "_");
        }
    }

    printf("Generated Assembly Code\n");

    for (int i = 0; i < n; i++)
    {
        printf("\nStatement %d: %s = %s ", i + 1, stmt[i].lhs, stmt[i].op1);
        if (stmt[i].op != '=')
            printf("%c %s", stmt[i].op, stmt[i].op2);
        printf("\n");

        char reg[10];
        sprintf(reg, "R%d", i);

        // Load first operand
        printf("MOV %s, %s\n", reg, stmt[i].op1);

        // Perform operation if needed
        switch (stmt[i].op)
        {
        case '+':
            printf("ADD %s, %s\n", reg, stmt[i].op2);
            break;
        case '-':
            printf("SUB %s, %s\n", reg, stmt[i].op2);
            break;
        case '*':
            printf("MUL %s, %s\n", reg, stmt[i].op2);
            break;
        case '/':
            printf("DIV %s, %s\n", reg, stmt[i].op2);
            break;
        case '=': /* simple assignment */
            break;
        default:
            break;
        }

        // Store result back to variable
        printf("MOV %s, %s\n", stmt[i].lhs, reg);
    }

    return 0;
}
