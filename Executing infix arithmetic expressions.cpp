/* Author : Kartik Kukreja
 * Description :    Program to implement a calculator that reads in expressions from
 *                  the console and writes out the results to the console.
 *
 *                  It requires all tokens to be separated by space. The only recognized
 *                  tokens are (, ), +, -, /, * and integers.
 *             
 *                  Sample expressions :
 *                  ( 10 + 11 ) * 12
 *                  ( 10 * 12 ) + ( 12 * 2 )
 */
 
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stack>
#include <cctype>
using namespace std;
 
bool error;
 
// returns true if op2 has lower or equal priority than op1, false otherwise
bool hasLowerPriority(char op1, char op2)   {
    switch (op1) {
        case '(': return false;
        case '-': return op2 == '-';
        case '+': return op2 == '-' || op2 == '+';
        case '*': return op2 != '/';
        case '/': return true;
        default : error = true; return false;
    }
}
 
stack < char > operators;
stack < int > operands;
 
// perform the operation 'op' on the two operands on top of the stack
void operation(char op) {
    if(operands.size() < 2) {
        error = true; return;
    }
    int op2 = operands.top(); operands.pop();
    int op1 = operands.top(); operands.pop();
    switch(op)  {
        case '+': operands.push(op1 + op2); break;
        case '-': operands.push(op1 - op2); break;
        case '*': operands.push(op1 * op2); break;
        case '/': operands.push(op1 / op2); break;
        default : error = true; return;
    }
}
 
int main()  {
    char exp[1000], *p;
    int len;
 
    printf("\t\t\t\tCALCULATOR\n");
    while(true) {
        printf("\nEnter an expression (. to exit):\n");
        scanf("%[^\n]%*c", exp);
        if(exp[0] == '.') break;
 
        len = strlen(exp);
        if(len == 0) { getchar(); continue; }
        exp[len] = ' '; exp[len + 1] = ')'; exp[len + 2] = '\0';
        error = false;
        operators.push('(');
 
        p = strtok(exp, " ");
        while(p && !error)    {
            if(isdigit(p[0]))
                operands.push(atoi(p));
            else switch(p[0])    {
                case '(' :  operators.push('('); break;
                case ')' :  while (!operators.empty() && !error && operators.top() != '(') {
				operation(operators.top()); operators.pop();
			    }
			    if (!operators.empty())
				operators.pop();
			    else
				error = true;
			    break;
                default  :  while(!operators.empty() && !error && hasLowerPriority(operators.top(), p[0])) {
                                operation(operators.top()); operators.pop();
                            }
                            operators.push(p[0]);
            }
            p = strtok(NULL, " ");
        }
        if(error || !operators.empty() || operands.size() != 1) {
            printf("ERROR\n");
            while(!operands.empty())
                operands.pop();
            while(!operators.empty())
                operators.pop();
        } else    {
            printf("%d\n", operands.top()); operands.pop();
        }
    }
    return 0;
}