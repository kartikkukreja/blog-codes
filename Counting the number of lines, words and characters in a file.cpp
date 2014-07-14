/* 	Author : Kartik Kukreja
	Roll No: 2K10/CO/049
	Description :	Program to count the number of character, words and lines in a given file.
                    The input file is named "input.txt" and the output is written to console.
 */

#include <cstdio>
#include <fstream>
#include <cstring>
using namespace std;

int main()  {
    int countC = 0, countW = 0, countL = 0;
    char str[10000], *p;

    ifstream fin("input.txt");
    while(!fin.eof())   {
        fin.getline(str, 10000);
        countL++;
        p = strtok(str, " ");
        while(p)    {
            countW++;
            countC += strlen(p);
            p = strtok(NULL, " ");
        }
    }
    fin.close();

    printf("No. of characters : %d\n", countC);
    printf("No. of words : %d\n", countW);
    printf("No. of lines : %d\n", countL);

    return 0;
}