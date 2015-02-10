/*
 * Author : Kartik Kukreja
 * Description :	Program to simulate a given DFA on an input string. It reads in the 
			DFA from "DFA.txt" and the input strings from the console and prints out
			whether the strings are accepted or rejected on the console.
					
			"DFA.txt" must have the following format:
				N M
				F a1 a2 ... af
				s1 y1 t1
				s2 y2 y2
				:
				:
			Here, 	N -> No. of states in the DFA (states are numbered 0 ... N-1)
				M -> Size of input alphabet (input symbols are numbered 1 ... M)
				F -> No. of final states followed by F states ( 0 <= ai <= N-1)
				si -> Previous state
				yi -> Input symbol
				ti -> Next state
			Each line until the end of file contains one transition ( si yi ti ).
 *			'0' is the start state.
 *			The input strings contain space-separated input symbols (1 ... M).
 */

#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#define MAX_DFA_STATES 10
#define MAX_ALPHABET_SIZE 10
using namespace std;

int transitions[MAX_DFA_STATES][MAX_ALPHABET_SIZE];
bool finalStates[MAX_DFA_STATES];
char input_string[100000];

int main()  {
    int N, M, F, X, Y, A, state, symbol, i, j;
    char* p;

    // read in the underlying DFA
    ifstream fin("DFA.txt");
    fin >> N >> M >> F;
    for(i=0; i<F; i++)  {
        fin >> X;
        finalStates[X] = true;
    }
    memset(transitions, -1, MAX_DFA_STATES * MAX_ALPHABET_SIZE * sizeof(int));
    while(!fin.eof())   {
        fin >> X >> A >> Y;
        transitions[X][A] = Y;
    }
    fin.close();

    // check if the DFA is well defined
    for(i=0; i<N; i++)
        for(j=1; j<=M; j++)
            if(transitions[i][j] < 0 || transitions[i][j] >= N) {
                printf("DFA not defined properly.\n");
                return -1;
            }

    // simulate the DFA
    printf("Enter a string ('.' to exit) : ");
    scanf("%[^\n]%*c", input_string);
    while(input_string[0] != '.')   {
        state = 0;
        p = strtok(input_string, " ");
        while(p)    {
            symbol = atoi(p);
            if(symbol <= 0 || symbol > M)   {
                printf("Invalid input symbol %d.\n", symbol);
                return -1;
            } else {
                state = transitions[state][symbol];
            }
            p = strtok(NULL, " ");
        }
        if(finalStates[state])
            printf("String accepted.\n");
        else
            printf("String rejected.\n");

        printf("Enter a string ('.' to exit) : ");
        scanf("%[^\n]%*c", input_string);
    }

    return 0;
}
