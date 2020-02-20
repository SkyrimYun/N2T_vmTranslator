#include "Header.h"

Parser::Parser(string& s)
{
	ist = ifstream(s.c_str());
	if (!ist) throw runtime_error("cannot open input file");
}

bool Parser::hasMoreCommands()
{
	if (ist.eof()) return false;
	else return true;
}

void Parser::advance()
{
	if (hasMoreCommands())
	{
		getline(ist, current_line);
	}
	else
	{
		throw runtime_error("no more lines");
	}
}

const int Parser::commandType()
{
	stringstream ss(current_line);
	ss >> type;
	if (type == "push")
	{
		ss >> arg_1 >> arg_2;
		return C_PUSH;
	}
	else if (type == "pop")
	{
		ss >> arg_1 >> arg_2;
		return C_POP;
	}
	else if (type == "goto")
	{
		ss >> arg_1;
		return C_GOTO;
	}
	else if (type == "if-goto")
	{
		ss >> arg_1;
		return C_IF;
	}
	else if (type == "label")
	{
		ss >> arg_1;
		return C_LABEL;
	}
	else if (type == "call")
	{
		ss >> arg_1 >> arg_2;
		return C_CALL;
	}
	else if (type == "function")
	{
		ss >> arg_1 >> arg_2;
		return C_FUNCTION;
	}
	else if (type == "return")
	{
		return C_RETURN;
	}

	for (string s : arithmetic)
	{
		if (type == s) return C_ARITHMETIC;
	}

	throw runtime_error("undefined command type");
}

string Parser::arg1()
{
	if (commandType() == C_ARITHMETIC)
		return type;
	else if (commandType() == C_RETURN)
		throw runtime_error("no argument 1 for return");
	else
	{
		return arg_1;
	}
}

int Parser::arg2()
{
	if (commandType() == C_PUSH || commandType() == C_POP || commandType() == C_CALL || commandType() == C_FUNCTION)
		return arg_2;
	else throw runtime_error("no argument 2 for the type");
}