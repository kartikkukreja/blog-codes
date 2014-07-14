/*
 * Author : Kartik Kukreja
 * Description :    Program to find the FIRST set for each grammar symbol for a CFG.
 *                  It reads in the CFG from "CFG.txt" and writes out the FIRST set
 *                  for each variable/non-terminal to the console.
 *
 *                  "CFG.txt" must have the following format :
 *                      P
 *                      Xi x1x2...xn
 *                      :
 *                      :
 *                  Here,   P -> No. of productions. Each production has the form Xi x1x2...xn
 *                                  which means Xi -> x1x2...xn where Xi is a variable and x1,x2,...,xn
 *                                  are variables or terminals
 *                          The variables are A, B, ..., Z and the terminals are a, b, ..., z.
 *                          S is the starting variable and e is reserved for epsilon.
 */

 #include <cstdio>
 #include <iostream>
 #include <fstream>
 #include <cctype>
 #include <vector>
 #include <map>
 #define SIZE 26
 using namespace std;

 typedef map<char,vector<string> > table;

 table productions;
 int FIRST[SIZE];

 int main() {
    int P, i, j, prev, next;
    char X;
    string right;
    bool complete = false;

    // read in the CFG
    ifstream fin("CFG.txt");
    fin >> P;
    for(i=0; i<P; i++)  {
        fin >> X >> right;
    // if the first symbol on the right side of a production is a terminal,
    // include it in the FIRST set of the variable on the left side of the production.
        if(islower(right[0]))
            FIRST[X - 'A'] |= 1 << (right[0] - 'a');
        else if(productions.find(X) != productions.end())
            productions[X].push_back(right);
        else    {
            vector <string> vec;
            vec.push_back(right);
            productions[X] = vec;
        }
    }
    fin.close();

    // iterate until no more elements can be added to FIRST set of any variable
    while(!complete)    {
        complete = true;
        for(table::iterator it=productions.begin(); it != productions.end(); it++)  {
            prev = next = FIRST[it->first - 'A'];
            for(vector<string>::iterator jt=(it->second).begin(); jt != (it->second).end(); jt++)   {
                for(i=0; i<jt->length(); i++)   {
                    X = (*jt)[i];
                    if(islower(X))  {
                        next |= 1 << (X - 'a'); break;
                    }
                    if((FIRST[X - 'A'] & (1 << 'e' - 'a')) == 0)    {
                        next |= FIRST[X - 'A']; break;
                    } else
                        next |= ~(~FIRST[X - 'A'] | (1 << 'e' - 'a'));
                }
                if(i == jt->length())
                    next |= (1 << 'e' - 'a');
            }
            if(prev != next)    {
                FIRST[it->first - 'A'] = next; complete = false;
            }
        }
    }

    // print out the FIRST set of each variable
    for(i=0; i<26; i++)
        if(FIRST[i])    {
            printf("FIRST(%c) = { ", i + 'A');
            for(j=0; j<26; j++)
                if(FIRST[i] & (1 << j))
                    printf("%c ", j + 'a');
            printf("}\n");
        }

    return 0;
 }