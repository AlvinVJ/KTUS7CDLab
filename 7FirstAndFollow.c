#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 50

// Store grammar productions
char productions[20][20];

// FIRST and FOLLOW sets for each non-terminal
char first[26][50], follow[26][50];
int numProductions;

// Flags to mark if FIRST or FOLLOW of a non-terminal is already computed
int doneFirst[26], doneFollow[26];


// Function declarations
void findFIRST(char, char[]);
void findFOLLOW(char);
void computeFIRST(char);
void computeFOLLOW(char);

// Adds a single character to a set (avoids duplicates)
void addToSet(char set[], char c)
{
    if (c == '\0'){
        return;
    }
    if (!strchr(set, c))
    {
        int len = strlen(set);
        set[len] = c;
        set[len + 1] = '\0';
    }
}

// Add all characters from src to dest (like union)
void addSet(char dest[], char src[])
{
    for (int i = 0; src[i]; i++)
        addToSet(dest, src[i]);
}

// ---- FIRST computation ----
/*
 * computeFIRST(nonTerminal)
 * -------------------------
 * Recursively computes the FIRST set of a given non-terminal.
 * 
 * Rules followed:
 * 1. If RHS starts with a terminal, add that terminal to FIRST.
 * 2. If RHS starts with a non-terminal, add its FIRST (except epsilon).
 * 3. If FIRST of a symbol contains epsilon, continue to the next symbol.
 * 4. If all symbols in RHS can derive epsilon, add epsilon.
 */

void computeFIRST(char nonTerminal)
{
    int index = nonTerminal - 'A';
    if (doneFirst[index])
        return;
    doneFirst[index] = 1;

    for (int i = 0; i < numProductions; i++)
    {
        if (productions[i][0] != nonTerminal)
            continue;// skip productions of other non-terminals

        int j = 2; // skip A=        
        
        // Case 1: A -> e (epsilon)
        if (productions[i][j] == 'e')
        {
            addToSet(first[index], 'e');
            continue;
        }

        // Case 2: A -> X Y Z ...
        while (productions[i][j] != '\0')
        {
            char temp[50] = "";
            findFIRST(productions[i][j], temp);

            int hasEpsilon = 0;

            // Add all symbols from temp to FIRST(A)
            for (int k = 0; temp[k]; k++)
            {
                if (temp[k] == 'e')
                    hasEpsilon = 1; // if epsilon found, continue to next symbol
                else
                    addToSet(first[index], temp[k]);
            }

            if (!hasEpsilon)
                break;
            j++;

            // If we reached end of RHS and all had epsilon, add epsilon to FIRST(A)
            if (productions[i][j] == '\0')
                addToSet(first[index], 'e');
        }
    }
}

/*
 * findFIRST(symbol, result)
 * -------------------------
 * Helper function:
 * If symbol is a terminal, FIRST = { symbol }.
 * If symbol is a non-terminal, compute its FIRST and return.
 */
void findFIRST(char c, char result[])
{
    if (!isupper(c))
    {
        addToSet(result, c);
        return;
    }
    if (!doneFirst[c - 'A'])
        computeFIRST(c);
    addSet(result, first[c - 'A']);
}

// ---- FOLLOW computation ----
/*
 * computeFOLLOW(nonTerminal)
 * --------------------------
 * Recursively computes the FOLLOW set of a given non-terminal.
 * 
 * Rules followed:
 * 1. Place '$' in FOLLOW(start symbol).
 * 2. If production has A -> αBβ, everything in FIRST(β) except epsilon
 *    is placed in FOLLOW(B).
 * 3. If production has A -> αB or A -> αBβ where FIRST(β) contains epsilon,
 *    everything in FOLLOW(A) is added to FOLLOW(B).
 */

void computeFOLLOW(char nonTerminal)
{
    int index = nonTerminal - 'A';
    if (doneFollow[index])
        return;
    doneFollow[index] = 1;

    // Rule 1: Add '$' to FOLLOW(start symbol)
    if (nonTerminal == productions[0][0])
        addToSet(follow[index], '$');


    // Loop through all productions to find where this non-terminal appears
    for (int i = 0; i < numProductions; i++)
    {
        int len = strlen(productions[i]);
        for (int j = 2; j < len; j++)
        {
            if (productions[i][j] != nonTerminal)
                continue;   // skip if not found in RHS

            // Case 2: A -> αBβ (B followed by some symbol)
            if (productions[i][j + 1] != '\0')
            {
                int k = j + 1;
                int hasEpsilon = 1;

                // Process symbols after B one by one
                while (productions[i][k] != '\0' && hasEpsilon)
                {
                    hasEpsilon = 0;
                    char temp[50] = "";
                    findFIRST(productions[i][k], temp); // find FIRST(β)

                    for (int t = 0; temp[t]; t++)
                    {
                        if (temp[t] == 'e')
                            hasEpsilon = 1; // continue if epsilon found
                        else
                            addToSet(follow[index], temp[t]);
                    }
                    k++;
                }

                // If β can derive epsilon, add FOLLOW(A) to FOLLOW(B)
                if (hasEpsilon && productions[i][0] != nonTerminal)
                {
                    findFOLLOW(productions[i][0]);
                    addSet(follow[index], follow[productions[i][0] - 'A']);
                }
            }
            
            // Case 3: A -> αB (B is last in RHS)
            else
            { // nonTerminal at end
                if (productions[i][0] != nonTerminal)
                {
                    findFOLLOW(productions[i][0]);
                    addSet(follow[index], follow[productions[i][0] - 'A']);
                }
            }
        }
    }
}

void findFOLLOW(char c)
{
    if (!doneFollow[c - 'A'])
        computeFOLLOW(c);
}

// ---- MAIN ----
int main()
{
    printf("Enter number of productions: ");
    scanf("%d", &numProductions);

    printf("Enter productions (use e for epsilon):\n");
    for (int i = 0; i < numProductions; i++)
        scanf("%s", productions[i]);

    // Compute FIRST
    for (int i = 0; i < numProductions; i++)
        computeFIRST(productions[i][0]);

    printf("\nFIRST sets:\n");
    int printed[26] = {0};
    for (int i = 0; i < numProductions; i++)
    {
        char nonTerminal = productions[i][0];
        if (!printed[nonTerminal - 'A'])
        {
            printf("FIRST(%c) = { ", nonTerminal);
            for (int j = 0; first[nonTerminal - 'A'][j]; j++)
                printf("%c ", first[nonTerminal - 'A'][j]);
            printf("}\n");
            printed[nonTerminal - 'A'] = 1;
        }
    }

    // Compute FOLLOW
    for (int i = 0; i < numProductions; i++)
        computeFOLLOW(productions[i][0]);

    printf("\nFOLLOW sets:\n");
    memset(printed, 0, sizeof(printed));
    for (int i = 0; i < numProductions; i++)
    {
        char nonTerminal = productions[i][0];
        if (!printed[nonTerminal - 'A'])
        {
            printf("FOLLOW(%c) = { ", nonTerminal);
            for (int j = 0; follow[nonTerminal - 'A'][j]; j++)
                printf("%c ", follow[nonTerminal - 'A'][j]);
            printf("}\n");
            printed[nonTerminal - 'A'] = 1;
        }
    }

    return 0;
}