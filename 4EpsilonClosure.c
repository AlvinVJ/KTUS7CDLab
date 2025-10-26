#include<stdio.h>
#include<stdlib.h>

#define maxStates 20

int transitionMatrix[maxStates][maxStates] = {0};
int numStates, numTransitions;
int visited[maxStates];


/*
Epsilon Closuretransition matrix looks like this:
[
0   [2, 3, 4], this means 2 epsilon transitions from 0. when input is epsilon to q0, it goes to q3, q4
1   [],
2   [], ...
]
*/

// Recursive DFS to find epsilon closure
void epsilonClosure(int state, int closure[]) {
    if (visited[state])
        return;
    visited[state] = 1;
    closure[state] = 1;

    // Number of epsilon transitions from this state
    int count = transitionMatrix[state][0];
    for (int i = 1; i <= count; i++) {
        int next = transitionMatrix[state][i];
        epsilonClosure(next, closure);
    }
}

void printEpsilonClosure() {
    for (int i = 0; i < numStates; i++) {
        int closure[maxStates] = {0};
        for (int j = 0; j < numStates; j++)
            visited[j] = 0;

        epsilonClosure(i, closure);

        printf("ε-closure(q%d) = { ", i);
        for (int j = 0; j < numStates; j++) {
            if (closure[j])
                printf("q%d ", j);
        }
        printf("}\n");
    }
}

void main(){
    printf("Enter number of states: ");
    scanf("%d", &numStates);
    printf("Enter number of transitions: ");
    scanf("%d", &numTransitions);
    printf("Enter transitions in format <from_state> <input_symbol> <to_state>, use e for epsilon\n");
    for(int i=0;i<numTransitions;i++){
        int fromState, toState;
        char inputSymbol;
        scanf("%d %c %d", &fromState, &inputSymbol, &toState);
        if (inputSymbol == 'e'){
            transitionMatrix[fromState][++transitionMatrix[fromState][0]] = toState;
        }
    }

    printEpsilonClosure();
}