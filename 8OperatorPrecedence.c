#include <stdio.h>
#include <string.h>

int get_terminal_index(char c,char* terminals,int n)    //Finding the index of the terminal to find them in the precedence table
{      
    for(int i=0;i<n;i++) {
        if(terminals[i] == c) {
            return i;
        }
    }
    return -1;
}

void main() {
    char terminals[10],input[50],stack[50],table[10][10];
    int i,j,n,stack_top = 0,inp_ptr = 0;
    printf("Enter terminals: \n");
    scanf("%s",terminals);
    n = strlen(terminals);
    printf("Enter the precedence table: \n");
    for(i=0;i<n;i++) {
        for(j=0;j<n;j++) {
            scanf(" %c",&table[i][j]);
        }
    }
    printf("Enter the input string: \n");
    scanf("%s",input);

    stack[0] = '$';
    stack[1] = '\0';
    printf("\nSTACK\t\tINPUT\t\tACTION\n------\t\t------\t\t------\n");
    while(1) {
        char stack_char = stack[stack_top];
        char input_char = input[inp_ptr];
        int stack_idx = get_terminal_index(stack_char,terminals,n);  //Get index of the terminal at stack and at input 
        int input_idx = get_terminal_index(input_char,terminals,n);

        if (stack_char == '$' && input_char == '$') {                //Final accepted case when stack and input have only $ 
            printf("%-15s %-15s Accepted!\n",stack,input+inp_ptr);
            break;
        }
        char reln = table[stack_idx][input_idx];
        if(reln == '<' || reln =='=') {
            printf("%-15s %-15s Shift %c\n",stack,input+inp_ptr,input_char);
            stack[++stack_top] = input_char;
            stack[stack_top + 1] = '\0';
            inp_ptr++;
        }
        else if(reln == '>') {
            printf("%-15s %-15s Reduce\n",stack,input + inp_ptr);
            stack[stack_top--] = '\0';
        }
        else {
            printf("Error\n");
            break;
        }
        
    }
   
}