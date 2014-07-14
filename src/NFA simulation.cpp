/*
 * Author : Kartik Kukreja
 * Description :        Program to simulate a given NFA on an input string. It reads in the
                        NFA from "NFA.txt" and the input strings from the console and prints out
                        whether the strings are accepted or rejected on the console.
 
                        "NFA.txt" must have the following format:
                                N M
                                F a1 a2 ... af
                                T
                                s1 y1 T1 t1 t2 ... tt1
                                s2 y2 T2 t1 t2 ... tt2
                                :
                                :
                        Here,   N -> No. of states (states are numbered 0 ... N-1). 0 is the start state
                                M -> Size of input alphabet (input symbols are
                                        numbered 1 ... M and 0 is reserved for epsilon)
                                F -> No. of final states, followed by F states ( 0 <= ai <= N-1)
                                T -> No. of transitions, followed by T lines
                                si -> Previous state ( 0 <= si <= N-1)
                                yi -> Input symbol or epsilon ( 0 <= yi <= M)
                                Ti -> No. of states the NFA moves from si on input yi, followed by Ti states
 */
 
#include <cstdio>
#include <fstream>
#include <cstdlib>
#include <set>
#include <bitset>
#include <cstring>
#define MAX_NFA_STATES 10
#define MAX_ALPHABET_SIZE 10
using namespace std;
 
// Representation of an NFA state
class NFAstate {
    public:
        int transitions[MAX_ALPHABET_SIZE][MAX_NFA_STATES];
        NFAstate()  {
            for(int i=0; i<MAX_ALPHABET_SIZE; i++)
                for(int j=0; j<MAX_NFA_STATES; j++)
                    transitions[i][j] = -1;
        }
} *NFAstates;
 
set <int> NFA_finalStates;
bitset <MAX_NFA_STATES> currentStates;
char input_string[100000];
int N;
 
// finds the epsilon closure of the NFA state "state" and stores it into "closure"
void epsilonClosure(int state, bitset<MAX_NFA_STATES> &closure )    {
    for(int i=0; i<N && NFAstates[state].transitions[0][i] != -1; i++)
        if(closure[NFAstates[state].transitions[0][i]] == 0)    {
            closure[NFAstates[state].transitions[0][i]] = 1;
            epsilonClosure(NFAstates[state].transitions[0][i], closure);
        }
}
 
// finds the epsilon closure of a set of NFA states "state" and stores it into "closure"
void epsilonClosure(bitset<MAX_NFA_STATES> state, bitset<MAX_NFA_STATES> &closure) {
    for(int i=0; i<N; i++)
        if(state[i] == 1)
            epsilonClosure(i, closure);
}
 
// returns a bitset representing the set of states the NFA could be in after moving
// from state X on input symbol A
void NFAmove(int X, int A, bitset<MAX_NFA_STATES> &Y)   {
    for(int i=0; i<N && NFAstates[X].transitions[A][i] != -1; i++)
        Y[NFAstates[X].transitions[A][i]] = 1;
}
 
// returns a bitset representing the set of states the NFA could be in after moving
// from the set of states X on input symbol A
void NFAmove(bitset<MAX_NFA_STATES> X, int A, bitset<MAX_NFA_STATES> &Y)   {
    bitset<MAX_NFA_STATES> tmp;
    for(int i=0; i<N; i++)
        if(X[i] == 1)
            NFAmove(i, A, tmp);
    Y = tmp;
}
 
int main()  {
    int M, F, X, Y, A, i, j, T, symbol;
    char* p;
 
    // read in the underlying NFA
    ifstream fin("NFA.txt");
    fin >> N >> M;
    NFAstates = new NFAstate[N];
    fin >> F;
    for(i=0; i<F; i++)    {
        fin >> X;
        NFA_finalStates.insert(X);
    }
    fin >> T;
    while(T--)   {
        fin >> X >> A >> Y;
        for(i=0; i<Y; i++)  {
            fin >> j;
            NFAstates[X].transitions[A][i] = j;
        }
    }
    fin.close();
 
    // simulate the NFA
    printf("Enter a string ('.' to exit) : ");
    scanf("%[^\n]%*c", input_string);
    while(input_string[0] != '.')   {
        currentStates[0] = 1;
        epsilonClosure(currentStates, currentStates);
 
        p = strtok(input_string, " ");
        while(p)    {
            symbol = atoi(p);
            if(symbol <= 0 || symbol > M)   {
                printf("Invalid input symbol %d.\n", symbol);
                return -1;
            } else {
                NFAmove(currentStates, symbol, currentStates);
                epsilonClosure(currentStates, currentStates);
            }
            p = strtok(NULL, " ");
        }
 
        for(i = 0; i < N; i++)
            if(currentStates[i] == 1 && NFA_finalStates.find(i) != NFA_finalStates.end())
                break;
 
        if(i < N)
            printf("String accepted.\n");
        else
            printf("String rejected.\n");
 
        printf("Enter a string ('.' to exit) : ");
        scanf("%[^\n]%*c", input_string);
    }
 
    return 0;
}