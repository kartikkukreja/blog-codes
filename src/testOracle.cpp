#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <windows.h>
using namespace std;

int main() {
	// define constants
	string correct_program = "correct.exe";
	string incorrect_program = "incorrect.exe";
	string input_file = "input.txt";
	string correct_output = "correct_output.txt";
	string incorrect_output = "incorrect_output.txt";
	int num_testcases = 10;
	
	// generate test cases and write to input file
	// create small, random test cases, consistent with the problem description
	vector<int> testCases(num_testcases);
	for (int i = 0; i < num_testcases; ++i)
		testCases[i] = rand();
	ofstream input_f;
	input_f.open(input_file.c_str());
	for (int i = 0; i < num_testcases; ++i)
		input_f << testCases[i] << "\n";
	input_f.close();
	
	// execute the two programs, resulting in them generating the output files
	system(correct_program.c_str());
	system(incorrect_program.c_str());
	
	// compare the two output files and find a point of difference
	ifstream correct_f, incorrect_f;
	correct_f.open(correct_output.c_str());
	incorrect_f.open(incorrect_output.c_str());
	for (int i = 0; i < num_testcases; ++i) {
		int correct, incorrect;
		correct_f >> correct;
		incorrect_f >> incorrect;
		if (correct != incorrect)
			cout << testCases[i] << "\ncorrect = " << correct << "\nincorrect = " << incorrect << "\n";
	}
	correct_f.close();
	incorrect_f.close();	

	return 0;
}
