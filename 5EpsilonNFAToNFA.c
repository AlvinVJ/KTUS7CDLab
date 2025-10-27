#include <stdio.h>
#include <stdlib.h>

#define MAX_STATES 20
#define MAX_SYMBOLS 5

int epsilonTransTable[MAX_STATES][MAX_STATES] = {0};
int regularTransTable[MAX_STATES][MAX_SYMBOLS][MAX_STATES] = {0};
int numStates, numTransitions, numSymbols;
char symbols[MAX_SYMBOLS];
int visited[MAX_STATES];

// Recursive epsilon closure
void epsilonClosure(int state, int closure[])
{
    if (visited[state])
        return;
    visited[state] = 1;
    closure[state] = 1;

    int count = epsilonTransTable[state][0];
    for (int i = 1; i <= count; i++)
    {
        int next = epsilonTransTable[state][i];
        epsilonClosure(next, closure);
    }
}

// Compute and print epsilon closure of all states
void printEpsilonClosures()
{
    printf("\n--- EPSILON CLOSURES ---\n");
    for (int i = 0; i < numStates; i++)
    {
        int closure[MAX_STATES] = {0};
        for (int j = 0; j < numStates; j++)
            visited[j] = 0;
        epsilonClosure(i, closure);

        printf("ε-closure(q%d) = { ", i);
        for (int j = 0; j < numStates; j++)
            if (closure[j])
                printf("q%d ", j);
        printf("}\n");
    }
}

// Compute epsilon closure for a specific state (helper)
void getEpsilonClosure(int state, int closure[])
{
    for (int j = 0; j < numStates; j++)
        visited[j] = 0;
    epsilonClosure(state, closure);
}

// Convert ε-NFA to NFA
void convertToNFA()
{
    printf("\n--- NFA TRANSITIONS (after removing ε) ---\n");

    for (int i = 0; i < numStates; i++)
    {
        for (int s = 0; s < numSymbols; s++)
        {
            int reachable[MAX_STATES] = {0};

            // 1. Get ε-closure of current state
            int closure[MAX_STATES] = {0};
            getEpsilonClosure(i, closure);

            // 2. For each state in closure, see transitions on symbol
            for (int j = 0; j < numStates; j++)
            {
                if (closure[j])
                {
                    int count = regularTransTable[j][s][0];
                    for (int k = 1; k <= count; k++)
                    {
                        int to = regularTransTable[j][s][k];

                        // 3. Add ε-closure of 'to'
                        int epsilonClosure[MAX_STATES] = {0};
                        getEpsilonClosure(to, epsilonClosure);
                        for (int x = 0; x < numStates; x++)
                            if (epsilonClosure[x])
                                reachable[x] = 1;
                    }
                }
            }

            // Print result
            printf("δ'(q%d, %c) = { ", i, symbols[s]);
            for (int j = 0; j < numStates; j++)
                if (reachable[j])
                    printf("q%d ", j);
            printf("}\n");
        }
    }
}

void main()
{
    printf("Enter number of states: ");
    scanf("%d", &numStates);

    printf("Enter number of input symbols (excluding epsilon): ");
    scanf("%d", &numSymbols);
    printf("Enter symbols: ");
    for (int i = 0; i < numSymbols; i++)
        scanf(" %c", &symbols[i]);

    printf("Enter number of transitions: ");
    scanf("%d", &numTransitions);

    printf("Enter transitions in format <from_state> <symbol> <to_state>\n");
    printf("Use 'e' for epsilon transitions.\n");

    for (int i = 0; i < numTransitions; i++)
    {
        int from, to;
        char sym;
        scanf("%d %c %d", &from, &sym, &to);

        if (sym == 'e')
        {
            epsilonTransTable[from][++epsilonTransTable[from][0]] = to;
        }
        else
        {
            // find symbol index
            int symIndex = -1;
            for (int s = 0; s < numSymbols; s++)
            {
                if (symbols[s] == sym)
                {
                    symIndex = s;
                    break;
                }
            }
            if (symIndex != -1)
                regularTransTable[from][symIndex][++regularTransTable[from][symIndex][0]] = to;
        }
    }

    printEpsilonClosures();
    convertToNFA();
}
