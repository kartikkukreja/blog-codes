/* 	Author : Kartik Kukreja
	Roll No: 2K10/CO/049
	Description :	Program to tokenize a given program. It reads in the tokens from "tokens.txt"
			and a program from console and prints out its tokenization to console.

			Each line of "tokens.txt" contains a lexeme and its token class. Each token in
			the input program is delimited by spaces.
 */

#include <cstdio>
#include <map>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

map < string, string > classOf;

int main()	{
	string lexeme, tokenClass;
	char program[10000];

	// read in the underlying tokens
	ifstream fin("tokens.txt");
	while(!fin.eof())	{
		fin >> lexeme >> tokenClass;
		classOf[lexeme] = tokenClass;
	}
	fin.close();

	// read in a program and tokenize it
	printf("Enter a valid program :\n");
	scanf("%[^\n]", program);
	stringstream ss(program);
	printf("\nTokenization of the given program :\n");
	while(!ss.eof())	{
		ss >> lexeme;
		if(classOf.find(lexeme) == classOf.end())	{
			cout << "Invalid token : " << lexeme << "\n";
			return -1;
		}
		cout << "< " << lexeme << ", " << classOf[lexeme] << " >\n";
	}
	return 0;
}