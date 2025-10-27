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
	printf("Grammar : \nE->TE'\nE'->+TE'|e\nT->FT'\nT'->*FT'|e\nF->(E)|id\n");
	printf("Enter the input (must end with $) : ");
	scanf("%s",input);
	E();
	if(error == 0 && input[i] == '$'){
		printf("Valid");
	}
	else{
		printf("Invalid");
	}
}

void E(){
	T();
	Eprime();
}

void Eprime(){
	if(input[i] == '+'){
		i++;
		T();
		Eprime();
	}
}

void T(){
	F();
	Tprime();
}

void Tprime(){
	if(input[i] == '*'){
		i++;
		F();
		Tprime();
	}
}

void F(){
	if(input[i] == 'i' && input[i+1] == 'd'){
		i+=2;
	}
	else{
		if(input[i] == '(' ){
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
}

