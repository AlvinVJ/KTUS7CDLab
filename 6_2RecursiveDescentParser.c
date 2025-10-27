#include<stdio.h>

void E();
void T();
void Eprime();
void Tprime();
void F();

int error = 0;
char input[30];
int i=0;

int main(){
    printf("Grammar : \nE->TE'\nE'->+TE'|-TE'|e\nT->FT'\nT'->*FT'|/FT'|e\nF->(E)|id|var\n");
    printf("Enter the input (must end with $) : ");
    scanf("%s",input);
    E();
    if(error == 0 && input[i] == '$'){
        printf("Valid\n");
    }
    else{
        printf("Invalid\n");
    }
}

// E -> T E'
void E(){
    T();
    Eprime();
}

// E' -> + T E' | - T E' | e
void Eprime(){
    if(input[i] == '+'){
        i++;
        T();
        Eprime();
    }
    else if(input[i] == '-'){
        i++;
        T();
        Eprime();
    }
}

// T -> F T'
void T(){
    F();
    Tprime();
}

// T' -> * F T' | / F T' | e
void Tprime(){
    if(input[i] == '*'){
        i++;
        F();
        Tprime();
    }
    else if(input[i] == '/'){
        i++;
        F();
        Tprime();
    }
}

// F -> id | single variable | (E)
void F(){
    // case: id
    if(input[i] == 'i' && input[i+1] == 'd'){
        i+=2;
    }
    // case: single variable (like a, b, c, ...)
    else if((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z')){
        i++;
    }
    // case: (E)
    else if(input[i] == '(' ){
        i++;
        E();
        if(input[i] == ')'){
            i++;
        }
        else{
            error = 1;
        }
    }
    else{
        error = 1;
    }
}
