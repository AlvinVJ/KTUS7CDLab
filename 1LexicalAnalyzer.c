#include <stdio.h>
#include <string.h>
#include <ctype.h>

FILE *fp, *fpSymTab;
int count = 0;

int isRelationalOp(char c)
{
    return (c == '>' || c == '<' || c == '=' || c == '!');
}

int isBinaryOp(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

void clearString(char string[26])
{
    for (int i = 0; i < strlen(string); i++)
        string[i] = '\0';
}

int findInSymbolTable(char s[26])
{
    char type[26], name[26];
    fseek(fpSymTab, 0, SEEK_SET);
    while (!feof(fpSymTab))
    {
        fscanf(fpSymTab, "%s %s", type, name);
        if (strcmp(name, s) == 0)
        {
            if (strcmp(type, "id") == 0)
                printf("%s - identifier , %d\n", s, count);
            else
                printf("%s - %s\n", s, type);
            return 1;
        }
        count++;
    }
    return 0;
}

void addToSymbolTable(char s[26])
{
    fprintf(fpSymTab, "id %s\n", s);
    printf("%s - identifier , %d\n", s, count);
    count++;
}

void main()
{
    char c, result[26], lib[26];
    int state = 0, i = 0;

    fp = fopen("input.c", "r");
    fpSymTab = fopen("symbol.txt", "a+");
    count = 0;

    do
    {
        if (feof(fp))
            break;
        c = fgetc(fp);

        switch (state)
        {
        case 0:
            if (isalpha(c))
            {
                state = 1;
                result[i++] = c;
            }
            else if (isdigit(c))
            {
                state = 3;
                result[i++] = c;
            }
            else if (isRelationalOp(c))
            {
                state = 5;
                result[i++] = c;
            }
            else if (isBinaryOp(c))
            {
                state = 7;
                result[i++] = c;
            }
            else if (c == '#')
            {
                state = 9;
            }
            break;

        case 1:
            if (isalnum(c))
            {
                result[i++] = c;
            }
            else
            {
                result[i] = '\0';
                if (strlen(result) > 0 && !findInSymbolTable(result))
                {
                    addToSymbolTable(result);
                }
                fseek(fp, -1L, SEEK_CUR);
                i = 0;
                clearString(result);
                state = 0;
            }
            break;

        case 3:
            if (isdigit(c))
            {
                result[i++] = c;
            }
            else if (isalpha(c))
            {
                i = 0;
                clearString(result);
                state = 0;
            }
            else
            {
                result[i] = '\0';
                printf("%s - Number\n", result);
                fseek(fp, -1L, SEEK_CUR);
                i = 0;
                clearString(result);
                state = 0;
            }
            break;

        case 5:
            result[i] = '\0';
            printf("%s - Relational Operator\n",
                   result);
            fseek(fp, -1L, SEEK_CUR);
            i = 0;
            clearString(result);
            state = 0;
            break;

        case 7:
            result[i] = '\0';
            printf("%s - Binary Operator\n",
                   result);
            fseek(fp, -1L, SEEK_CUR);
            i = 0;
            clearString(result);
            state = 0;
            break;

        case 9:
            i = 0;
            while (c != ' ' && c != '\n' && c != EOF)
            {
                result[i++] = c;
                c = fgetc(fp);
            }
            result[i] = '\0';
            fscanf(fp, "%s", lib);
            if (strcmp(result, "include") == 0)
            {
                printf("include - Preprocessor Directive\n");
                printf("%s - Library\n", lib);
                fprintf(fpSymTab, "preprocessor include\n");
                fprintf(fpSymTab, "library %s\n", lib);
            }
            clearString(result);
            clearString(lib);
            state = 0;
            break;
        }
    } while (1);

    fclose(fp);
    fclose(fpSymTab);
}
