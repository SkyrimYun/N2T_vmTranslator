#pragma once

#include <iostream>
#include <string>
#include <fstream> 
#include <vector>
#include <sstream> 
#include <io.h>
using namespace std;

const enum C_TYPE { C_PUSH, C_POP, C_ARITHMETIC, C_GOTO, C_IF, C_LABEL, C_CALL, C_FUNCTION, C_RETURN };
const vector<string> arithmetic = { "add","sub","neg","eq","gt","lt","and","or","not" };

class Parser
{
	ifstream ist;
	string current_line;
	string type;
	string arg_1;
	int arg_2 = -1;

public:
	Parser(string& s);

	//are there more commands in the input
	bool hasMoreCommands();

	//reads the next command from the input and makes it the current command
	//should only be called if hasMoreCommands() is true
	//initially there is no current command
	void advance();

	//returns a constant representing the type of the current type
	const int commandType();

	//return the first argument of the current command
	//in case of C_ARITHMETIC, the command itself is returned 
	string arg1();

	//return the second argument of the current command
	//should be called only if the current command is C_PUSH, C_POP
	int arg2();
};

class CodeWriter
{
	ofstream ost;
	string prefix;
	const string spPlus = "@SP\nM=M+1\n";
	const string spMinus = "@SP\nM=M-1\n";
	const string spEqualD = "@SP\nA=M\nM=D\n";
	const string DEqualsp = "@SP\nA=M\nD=M\n";
	const string storeToR = "@13\nM=D\n";
	const string pop2 = spMinus + DEqualsp + storeToR + spMinus + DEqualsp;

	int lableCount = 0;
	int retCount = 0;

public:
	CodeWriter(string& s);

	//write the output file the assembly code 
	//that implements the given arithmetic command
	void writeArithmetic(string s);

	//write the output file the assembly code 
	//that implements the given command, where command is either C_PUSH or C_POP
	void WritePushPop(const int n, string segment, int index);

	//writes the assembly instructions that effect the bootstrap code that initializes the VM
	//this code must be placed at the beginning of the generated *.asm file
	void writeInit();

	void writeLabel(string label);

	void writeGoto(string label);

	void writeIf(string label);

	void writeFunction(string functionName, int numVars);

	void writeCall(string functionName, int numArgs);

	void writeReturn();

	//close the output file
	void close();
};